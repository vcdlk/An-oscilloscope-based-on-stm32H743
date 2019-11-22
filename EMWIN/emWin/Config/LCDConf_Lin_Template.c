#include "LCDConf.h"
#include "GUI_Private.h"
#include "sys.h"
#include "led.h" 
#include "ltdc.h"
#include "malloc.h"
#include "tftlcd.h"
#include "GUIDRV_Lin.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//STemWin LCD�ײ��������� 	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/15
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

#define XSIZE_PHYS 1024     //���֧��1024*768
#define YSIZE_PHYS 768

#define NUM_BUFFERS  3      //ʹ�ö໺��ʱ�Ļ�������
#define NUM_VSCREENS 1      //ʹ��������Ļ������

#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 1    //����

#define COLOR_CONVERSION_0 GUICC_M565
#define DISPLAY_DRIVER_0   GUIDRV_LIN_16

#if (GUI_NUM_LAYERS > 1)
  #define COLOR_CONVERSION_1 GUICC_M1555I
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
#endif

#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif

//LCD����Ļ�������Ҫ����ʵ����ʹ�õ���Ļ�ֱ����Լ�ÿ����ʹ�õ���ɫ��ʽ�����㣬
//����ȷ����ѵĻ�������С,����Ĭ���������ֱ���Ϊ1024*600����ɫ��ʽΪ8888��
//��ôÿ������Ҫ�Ļ���������1024*600*4=2457600=0X258000,ע��mlloc.h��malloc.c
//�е��ⲿSDRAM�ڴ����ص�ַ�ʹ�СҲ��Ҫ���޸ģ�������
#define LCD_LAYER0_FRAME_BUFFER  ((uint32_t)0xC0000000)     //��һ�㻺����
#define LCD_LAYER1_FRAME_BUFFER  ((uint32_t)0xC0300000)     //�ڶ��㻺����
  

#define DEFINEDMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                             \
static void Color2IndexBulk_##PFIX##DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  DMA2D_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                         \
}                                                                                                                   \
static void Index2ColorBulk_##PFIX##DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  DMA2D_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);  \
}
   
static LCD_LayerPropTypedef     layer_prop[GUI_NUM_LAYERS];
static const LCD_API_COLOR_CONV *apColorConvAPI[]= 
{
  COLOR_CONVERSION_0,
#if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};

static U32 aBufferDMA2D [XSIZE_PHYS * sizeof(U32)];
static U32 aBuffer_FG   [XSIZE_PHYS * sizeof(U32)];
static U32 aBuffer_BG   [XSIZE_PHYS * sizeof(U32)];

static uint32_t LCD_LL_GetPixelformat(uint32_t LayerIndex);
static void     DMA2D_CopyBuffer(uint32_t LayerIndex, void * pSrc, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);
static void     DMA2D_FillBuffer(uint32_t LayerIndex, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);
static void     LCD_LL_Init(void); 
static void     LCD_LL_LayerInit(uint32_t LayerIndex); 

static void     CUSTOM_CopyBuffer(int32_t LayerIndex, int32_t IndexSrc, int32_t IndexDst);
static void     CUSTOM_CopyRect(int32_t LayerIndex, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t xSize, int32_t ySize);
static void     CUSTOM_FillRect(int32_t LayerIndex, int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t PixelIndex);

static void     DMA2D_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat);
static void     DMA2D_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat);

static void LCD_DrawBitmap8bpp(int32_t LayerIndex, int32_t x, int32_t y, U8 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine);
static void LCD_DrawBitmap16bpp(int32_t LayerIndex, int32_t x, int32_t y, U16 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine);
static void DMA2D_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems);
static void DMA2D_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems);
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex);
static LCD_COLOR DMA2D_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);
static void LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens);

DEFINEDMA2D_COLORCONVERSION(M8888I, LTDC_PIXEL_FORMAT_ARGB8888)
DEFINEDMA2D_COLORCONVERSION(M888,   LTDC_PIXEL_FORMAT_ARGB8888)
//DEFINEDMA2D_COLORCONVERSION(M565,   LTDC_PIXEL_FORMAT_RGB565)
DEFINEDMA2D_COLORCONVERSION(M1555I, LTDC_PIXEL_FORMAT_ARGB1555)
DEFINEDMA2D_COLORCONVERSION(M4444I, LTDC_PIXEL_FORMAT_ARGB4444)

static uint32_t GetBufferSize(uint32_t LayerIndex);
 
//LTDC�жϷ�����
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&LTDC_Handler);
}

//LTDC���¼��ص�����
//LTDC_Handler:LTDC���
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *LTDC_Handler)
{
    uint32_t Addr;
    uint32_t layer=0;

    for (layer=0;layer<GUI_NUM_LAYERS;layer++) 
    {
        if(layer_prop[layer].pending_buffer>=0) 
        {
            //������ʾ��֡��ַ
            Addr=layer_prop[layer].address + \
                 layer_prop[layer].xSize * layer_prop[layer].ySize * layer_prop[layer].pending_buffer * layer_prop[layer].BytesPerPixel;
            HAL_LTDC_SetAddress(LTDC_Handler,Addr,layer);//���õ�ַ
            __HAL_LTDC_RELOAD_CONFIG(LTDC_Handler);
            GUI_MULTIBUF_ConfirmEx(layer,layer_prop[layer].pending_buffer);
            layer_prop[layer].pending_buffer=-1;
        }
    }
    HAL_LTDC_ProgramLineEvent(LTDC_Handler,0);
}

//���ó���,���ڴ�����ʾ��������,������ɫת���������ʾ�ߴ�
void LCD_X_Config(void) 
{
    uint32_t i;
  
    LCD_LL_Init ();                                 //LCD�ײ�����(LTDC�ж����ú�DMA2D��ʼ��)
#if (NUM_BUFFERS>1)                                 //�໺��
    for (i=0;i<GUI_NUM_LAYERS; i++) 
    {
        GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
    }
#endif
    //���õ�һ��
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0,COLOR_CONVERSION_0,0,0);//������ʾ��������
    GUI_SelectLayer(0);     //ѡ�е�0��
    if(lcddev.dir==0)//����
    {
        LCD_SetSizeEx(0,lcddev.height,lcddev.width);    //���ÿɼ����ߴ�
        LCD_SetVSizeEx(0,lcddev.height,lcddev.width*NUM_VSCREENS);   //����������ʾ���ߴ�
        GUI_SetOrientation(GUI_SWAP_XY|GUI_MIRROR_Y);                   //����Ϊ����
    }else            //����
    {
    	LCD_SetSizeEx(0,lcddev.width,lcddev.height);    //���ÿɼ����ߴ�
        LCD_SetVSizeEx(0,lcddev.width,lcddev.height*NUM_VSCREENS);   //����������ʾ���ߴ�
    }
    GUI_TOUCH_Calibrate(GUI_COORD_X,0,lcddev.width,0,lcddev.width-1);   
    GUI_TOUCH_Calibrate(GUI_COORD_Y,0,lcddev.height,0,lcddev.height-1);
#if (GUI_NUM_LAYERS>1)
  
    //���õڶ���
    GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1,COLOR_CONVERSION_1,0,1);
    GUI_SelectLayer(1);     //ѡ�е�1��
    if(lcddev.dir==0)//����
    {
        LCD_SetSizeEx(1,lcddev.height,lcddev.width);    //���ÿɼ����ߴ�
        LCD_SetVSizeEx(1,lcddev.height,lcddev.width*NUM_VSCREENS);   //����������ʾ���ߴ�
        GUI_SetOrientation(GUI_SWAP_XY|GUI_MIRROR_Y);                   //����Ϊ����
    }else            //����
    {
    	LCD_SetSizeEx(1,lcddev.width,lcddev.height);    //���ÿɼ����ߴ�
        LCD_SetVSizeEx(1,lcddev.width,lcddev.height*NUM_VSCREENS);   //����������ʾ���ߴ�
    }
    GUI_TOUCH_Calibrate(GUI_COORD_X,0,lcddev.width,0,lcddev.width-1);   
    GUI_TOUCH_Calibrate(GUI_COORD_Y,0,lcddev.height,0,lcddev.height-1);
#endif
  
    layer_prop[0].address=LCD_LAYER0_FRAME_BUFFER;           //������
#if (GUI_NUM_LAYERS>1)
    layer_prop[1].address=LCD_LAYER1_FRAME_BUFFER;     
#endif
    
    for (i=0;i<GUI_NUM_LAYERS;i++) 
    {
        layer_prop[i].pColorConvAPI=(LCD_API_COLOR_CONV *)apColorConvAPI[i];
        layer_prop[i].pending_buffer=-1;
        LCD_SetVRAMAddrEx(i,(void *)(layer_prop[i].address));
        layer_prop[i].BytesPerPixel=LCD_GetBitsPerPixelEx(i) >> 3;
        LCD_SetDevFunc(i,LCD_DEVFUNC_COPYBUFFER,(void(*)(void))CUSTOM_CopyBuffer);
        LCD_SetDevFunc(i,LCD_DEVFUNC_COPYRECT,(void(*)(void))CUSTOM_CopyRect);
        if (LCD_LL_GetPixelformat(i)<=LTDC_PIXEL_FORMAT_ARGB4444) 
        {
            LCD_SetDevFunc(i,LCD_DEVFUNC_FILLRECT,(void(*)(void))CUSTOM_FillRect);
            LCD_SetDevFunc(i,LCD_DEVFUNC_DRAWBMP_8BPP,(void(*)(void))LCD_DrawBitmap8bpp);
        }
        if(LCD_LL_GetPixelformat(i)==LTDC_PIXEL_FORMAT_RGB565) 
        {
           LCD_SetDevFunc(i,LCD_DEVFUNC_DRAWBMP_16BPP,(void(*)(void))LCD_DrawBitmap16bpp);    
        }
        GUICC_M1555I_SetCustColorConv(Color2IndexBulk_M1555IDMA2D,Index2ColorBulk_M1555IDMA2D); 
        //GUICC_M565_SetCustColorConv(Color2IndexBulk_M565DMA2D,Index2ColorBulk_M565DMA2D);  
        GUICC_M4444I_SetCustColorConv(Color2IndexBulk_M4444IDMA2D,Index2ColorBulk_M4444IDMA2D); 
        GUICC_M888_SetCustColorConv(Color2IndexBulk_M888DMA2D,Index2ColorBulk_M888DMA2D);   
        GUICC_M8888I_SetCustColorConv(Color2IndexBulk_M8888IDMA2D,Index2ColorBulk_M8888IDMA2D);
        GUI_SetFuncAlphaBlending(DMA2D_AlphaBlending);                                            
        GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
        GUI_SetFuncMixColors(DMA2D_MixColors);
        GUI_SetFuncMixColorsBulk(LCD_MixColorsBulk);
    }
}

//��ʾ�����ص�����
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) 
{
    int32_t r = 0;
    uint32_t addr;
    int32_t xPos, yPos;
    uint32_t Color;
    
    switch (Cmd) 
    {
      case LCD_X_INITCONTROLLER: 
            LCD_LL_LayerInit(LayerIndex); 
            break;
        case LCD_X_SETORG: 
            addr=layer_prop[LayerIndex].address+((LCD_X_SETORG_INFO *)pData)->yPos*layer_prop[LayerIndex].xSize*layer_prop[LayerIndex].BytesPerPixel;
            HAL_LTDC_SetAddress(&LTDC_Handler,addr,LayerIndex);
            break;
        case LCD_X_SHOWBUFFER: 
            layer_prop[LayerIndex].pending_buffer=((LCD_X_SHOWBUFFER_INFO *)pData)->Index;
            break;
        case LCD_X_SETLUTENTRY: 
            HAL_LTDC_ConfigCLUT(&LTDC_Handler,(uint32_t *)&(((LCD_X_SETLUTENTRY_INFO *)pData)->Color),1,LayerIndex);
            break;
        case LCD_X_ON: 
            __HAL_LTDC_ENABLE(&LTDC_Handler);
            break;
        case LCD_X_OFF: 
            __HAL_LTDC_DISABLE(&LTDC_Handler);
            break;
        case LCD_X_SETVIS:
            if(((LCD_X_SETVIS_INFO *)pData)->OnOff==ENABLE )
            {
                __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,LayerIndex); 
            }
            else
            {
                __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,LayerIndex); 
            }
            __HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler); 
            break;
        case LCD_X_SETPOS: 
            HAL_LTDC_SetWindowPosition(&LTDC_Handler, 
                                      ((LCD_X_SETPOS_INFO *)pData)->xPos, 
                                      ((LCD_X_SETPOS_INFO *)pData)->yPos, 
                                      LayerIndex);
            break;
        case LCD_X_SETSIZE:
            GUI_GetLayerPosEx(LayerIndex,(int*)&xPos,(int*)&yPos);
            layer_prop[LayerIndex].xSize=((LCD_X_SETSIZE_INFO *)pData)->xSize;
            layer_prop[LayerIndex].ySize=((LCD_X_SETSIZE_INFO *)pData)->ySize;
            HAL_LTDC_SetWindowPosition(&LTDC_Handler,xPos,yPos,LayerIndex);
            break;
        case LCD_X_SETALPHA:
            HAL_LTDC_SetAlpha(&LTDC_Handler,((LCD_X_SETALPHA_INFO *)pData)->Alpha,LayerIndex);
            break;
        case LCD_X_SETCHROMAMODE:
            if(((LCD_X_SETCHROMAMODE_INFO *)pData)->ChromaMode != 0)
            {
                HAL_LTDC_EnableColorKeying(&LTDC_Handler,LayerIndex);
            }
            else
            {
                HAL_LTDC_DisableColorKeying(&LTDC_Handler,LayerIndex);      
            }
            break;
        case LCD_X_SETCHROMA:
            Color=((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0xFF0000) >> 16) |\
                    (((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x00FF00) |\
                    ((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x0000FF) << 16);
    
            HAL_LTDC_ConfigColorKeying(&LTDC_Handler,Color,LayerIndex);
            break;
        default:
            r = -1;
    }
    return r;
}

//LTDC������
static void LCD_LL_LayerInit(uint32_t LayerIndex) 
{
    uint32_t i;
    static uint32_t LUT[256];
    LTDC_LayerCfgTypeDef layer_cfg;
  
    if (LayerIndex<GUI_NUM_LAYERS) 
    {
        layer_cfg.WindowX0=0;                                   //������ʼX����
        layer_cfg.WindowY0=0;                                   //������ʼY����
        layer_cfg.WindowX1=lcdltdc.pwidth;                      //������ֹX����
        layer_cfg.WindowY1=lcdltdc.pheight;                     //������ֹY����
        layer_cfg.PixelFormat=LCD_LL_GetPixelformat(LayerIndex);//���ظ�ʽ
        layer_cfg.Alpha=255;                                    //Alphaֵ���ã�0~255,255Ϊ��ȫ��͸��
        layer_cfg.Alpha0=0;;                                    //Ĭ��Alphaֵ
        layer_cfg.Backcolor.Red=0;                              //������ɫ��ɫ����
        layer_cfg.Backcolor.Green=0;                            //������ɫ��ɫ����
        layer_cfg.Backcolor.Blue=0;                             //������ɫ��ɫ����       
        layer_cfg.FBStartAdress=layer_prop[LayerIndex].address; //���ò���ɫ֡������ʼ��ַ
        layer_cfg.BlendingFactor1=LTDC_BLENDING_FACTOR1_PAxCA;  //���ò���ϵ��
        layer_cfg.BlendingFactor2=LTDC_BLENDING_FACTOR2_PAxCA;  //���ò���ϵ��
        layer_cfg.ImageWidth=lcdltdc.pwidth;                    //������ɫ֡�������Ŀ��  
        layer_cfg.ImageHeight=lcdltdc.pheight;                  //������ɫ֡�������ĸ߶�
        HAL_LTDC_ConfigLayer(&LTDC_Handler,&layer_cfg,LayerIndex);//������ѡ�еĲ�  
        if (LCD_GetBitsPerPixelEx(LayerIndex)<=8) 
        {
            HAL_LTDC_EnableCLUT(&LTDC_Handler,LayerIndex);
        } 
        else 
        {
            if (layer_prop[LayerIndex].pColorConvAPI==GUICC_88666I) 
            {
                HAL_LTDC_EnableCLUT(&LTDC_Handler,LayerIndex);
                for (i=0;i<256;i++) 
                {
                    LUT[i]=LCD_API_ColorConv_8666.pfIndex2Color(i);
                }
                HAL_LTDC_ConfigCLUT(&LTDC_Handler,LUT,256,LayerIndex);
            }
        }
    }
}

//LCD�ײ�����,LTDC�ж����ã�DMA2D��ʼ��
static void LCD_LL_Init(void) 
{
    //LTDC�жϣ���ռ���ȼ�1�������ȼ�1
    HAL_NVIC_SetPriority(LTDC_IRQn,1,1);    
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
    HAL_LTDC_ProgramLineEvent(&LTDC_Handler,0);//����LTDC�����ж�
  
    //DMA2DĬ������
    DMA2D_Handler.Instance=DMA2D; 
    DMA2D_Handler.Init.Mode=DMA2D_R2M;          //�ڴ浽�洢��ģʽ
    DMA2D_Handler.Init.ColorMode=DMA2D_INPUT_RGB565;  //RGB565ģʽ
    DMA2D_Handler.Init.OutputOffset=0x0;        //���ƫ��Ϊ0    
    HAL_DMA2D_Init(&DMA2D_Handler);
}

//���ز����ɫ��ʽ
//����ֵ:1��ʧ�ܣ���������ɫ��ʽ
static uint32_t LCD_LL_GetPixelformat(uint32_t LayerIndex)
{
    const LCD_API_COLOR_CONV * pColorConvAPI;
    if (LayerIndex>=GUI_NUM_LAYERS) return 0;
    pColorConvAPI=layer_prop[LayerIndex].pColorConvAPI;
    if(pColorConvAPI==GUICC_M8888I) return LTDC_PIXEL_FORMAT_ARGB8888;
    else if (pColorConvAPI==GUICC_M888) return LTDC_PIXEL_FORMAT_RGB888;
    else if (pColorConvAPI==GUICC_M565) return LTDC_PIXEL_FORMAT_RGB565;
    else if (pColorConvAPI==GUICC_M1555I) return LTDC_PIXEL_FORMAT_ARGB1555;
    else if (pColorConvAPI==GUICC_M4444I) return LTDC_PIXEL_FORMAT_ARGB4444;
    return 1;
}


//ʹ��DMA2D�����ݴ�һ��buffer��������һ��buffer��
//LayerIndex:������
//pSrc:Դbuffer
//pDst:Ŀ��buffer
//xSize:X���С(ÿ��������)
//ySize:Y���С(����)
//OffLineSrc:Դbufferƫ�Ƶ�ַ
//OffLineDst:Ŀ��fufferƫ�Ƶ�ַ
static void DMA2D_CopyBuffer(uint32_t LayerIndex,void * pSrc,void * pDst,uint32_t xSize,uint32_t ySize,uint32_t OffLineSrc,uint32_t OffLineDst)
{
    uint32_t PixelFormat;
    u32 timeout=0;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);  //��ȡ���ظ�ʽ
    DMA2D->CR=0x00000000UL|(1<<9);                  //�洢�����洢��
    DMA2D->FGMAR=(uint32_t)pSrc;                    //����ǰ����洢����ַ                   
    DMA2D->OMAR=(uint32_t)pDst;                     //��������洢����ַ    
    DMA2D->FGOR=OffLineSrc;                         //����ǰ����ƫ��
    DMA2D->OOR=OffLineDst;                          //�������ƫ��
    DMA2D->FGPFCCR=PixelFormat;                     //������ɫģʽ
    DMA2D->NLR=(uint32_t)(xSize<<16)|(U16)ySize;    //����ÿ��������������
    DMA2D->CR|=DMA2D_CR_START;                      //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)                 //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	                //��ʱ�˳�
    }
}

//ʹ��ָ������ɫ���һ�����ο�
//LayerIndex:������
//pDst:Ŀ��buffer��Ҫ���ľ���
//xSize:X���С(ÿ��������)
//ySize:Y���С(����)
//OffLineSrc:Դbufferƫ�Ƶ�ַ
//OffLineDst:Ŀ��fufferƫ�Ƶ�ַ
//ColorIndex:Ҫ������ɫֵ
static void DMA2D_FillBuffer(uint32_t LayerIndex,void * pDst,uint32_t xSize,uint32_t ySize,uint32_t OffLine,uint32_t ColorIndex) 
{
    uint32_t PixelFormat;
    u32 timeout=0;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);  //��ȡ���ظ�ʽ
    DMA2D->CR=0x00030000UL|(1<<9);                  //�Ĵ������洢�� 
    DMA2D->OCOLR=ColorIndex;                        //����Ҫ������ɫ                   
    DMA2D->OMAR=(uint32_t)pDst;                     //��������Ĵ�����ַ
    DMA2D->OOR=OffLine;                             //�������ƫ��
    DMA2D->OPFCCR=PixelFormat;                      //������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(xSize << 16)|(U16)ySize;  //����ÿ��������������
    DMA2D->CR|=DMA2D_CR_START;                      //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)                 //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	                //��ʱ�˳�
    }
}

//��ȡbuffer��С
//����ֵ:buffer��С
static uint32_t GetBufferSize(uint32_t LayerIndex) 
{
    uint32_t BufferSize;
    BufferSize = layer_prop[LayerIndex].xSize*layer_prop[LayerIndex].ySize*layer_prop[LayerIndex].BytesPerPixel;
    return BufferSize;
}

//�û��Զ���Ļ��������ƺ���
//LayerIndex:������
//IndexSrc:�����Ƶ�Դ֡������������
//IndexDst:�����ǵ�Ŀ��֡������������
static void CUSTOM_CopyBuffer(int32_t LayerIndex,int32_t IndexSrc,int32_t IndexDst) 
{
    uint32_t BufferSize, AddrSrc, AddrDst;

    BufferSize=GetBufferSize(LayerIndex);
    AddrSrc=layer_prop[LayerIndex].address+BufferSize*IndexSrc;
    AddrDst=layer_prop[LayerIndex].address+BufferSize*IndexDst;
    DMA2D_CopyBuffer(LayerIndex,(void *)AddrSrc,(void *)AddrDst,layer_prop[LayerIndex].xSize,layer_prop[LayerIndex].ySize,0,0);
    layer_prop[LayerIndex].buffer_index=IndexDst;
}

//�û��Զ���Ľ���Ļ��һ�����������Ƶ�Ŀ��λ��
//LayerIndex:������
//x0:Դ��������ߵ�����
//y0:Դ�������ϱߵ�����
//x1:Ŀ���������ߵ�����
//y1:Ŀ��������ϱߵ�����
//xSize:���ε�X�ߴ�
//ySize:���ε�Y�ߴ�
static void CUSTOM_CopyRect(int32_t LayerIndex, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t xSize, int32_t ySize) 
{
    uint32_t BufferSize, AddrSrc, AddrDst;

    BufferSize=GetBufferSize(LayerIndex);
    AddrSrc=layer_prop[LayerIndex].address+BufferSize*layer_prop[LayerIndex].pending_buffer+(y0*layer_prop[LayerIndex].xSize+x0)*layer_prop[LayerIndex].BytesPerPixel;
    AddrDst=layer_prop[LayerIndex].address+BufferSize*layer_prop[LayerIndex].pending_buffer+(y1*layer_prop[LayerIndex].xSize+x1)*layer_prop[LayerIndex].BytesPerPixel;
    DMA2D_CopyBuffer(LayerIndex,(void *)AddrSrc,(void *)AddrDst,xSize,ySize,layer_prop[LayerIndex].xSize-xSize,0);
}

//�û��Զ������亯��
//LayerIndex:������
//x0:��Ļ�����д���������ߵ�����
//y0:��Ļ�����д��������ϱߵ�����
//x1:��Ļ�����д��������ұߵ�����
//y1:��Ļ�����д��������±�����
//PixelIndex:��������ɫ
static void CUSTOM_FillRect(int32_t LayerIndex,int32_t x0,int32_t y0,int32_t x1,int32_t y1,uint32_t PixelIndex) 
{
    uint32_t BufferSize, AddrDst;
    int32_t xSize, ySize;

    if (GUI_GetDrawMode()==GUI_DM_XOR) 
    {
        LCD_SetDevFunc(LayerIndex,LCD_DEVFUNC_FILLRECT,NULL);
        LCD_FillRect(x0,y0,x1,y1);
        LCD_SetDevFunc(LayerIndex,LCD_DEVFUNC_FILLRECT,(void(*)(void))CUSTOM_FillRect);
    }else 
    {
        xSize=x1-x0+1;
        ySize=y1-y0+1;
        BufferSize=GetBufferSize(LayerIndex);
        AddrDst=layer_prop[LayerIndex].address+BufferSize*layer_prop[LayerIndex].buffer_index+(y0 * layer_prop[LayerIndex].xSize+x0)*layer_prop[LayerIndex].BytesPerPixel;
        DMA2D_FillBuffer(LayerIndex,(void *)AddrDst,xSize,ySize,layer_prop[LayerIndex].xSize-xSize,PixelIndex);
    }
}

//DMA2D��CLUT����
//pColor:CLUT�洢����ַ
//NumItems:CLUT��С
static void DMA2D_LoadLUT(LCD_COLOR * pColor,uint32_t NumItems) 
{
    DMA2D->FGCMAR=(uint32_t)pColor; //����ǰ����CLUT�洢����ַ
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_RGB888|((NumItems-1)&0xFF)<<8;//������ɫģʽ��CLUT��С
    DMA2D->FGPFCCR|=(1<<5);         //��������
}

//����ָ����ַ��ɫ��RGB˳���ҷ�תalphaֵ�����µõ�����ɫֵ���Ƶ�Ŀ���ַ��
//pColorSrc:Դ��ַ
//pColorDst:Ŀ�ĵ�ַ
//NumItems:Ҫ���������
static void InvertAlpha_SwapRB(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
    uint32_t Color;
    do 
    {
        Color=*pColorSrc++;
        *pColorDst++=((Color&0x000000FF)<<16)           //������ɫ(R)����ɫ(B)��λ��
                    |(Color&0x0000FF00)                 //��ɫ(G)��λ�ò���
                    |((Color&0x00FF0000)>>16)           //������ɫ(R)����ɫ(B)��λ��
                    |((Color&0xFF000000)^0xFF000000);   //��תalphaֵ
    } while (--NumItems);
}


//��תalpha����ΪDMA2D����ɫ��ʽ��STemWin����ɫ��ʽ��ͬ�������Ҫ��תalphaֵʹ������STemWin
//pColorSrc:Դ��ַ
//pColorDst:Ŀ�ĵ�ַ
//NumItems:Ҫ���������
static void InvertAlpha(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
    uint32_t Color;
    do 
    {
        Color = *pColorSrc++;
        *pColorDst++=Color^0xFF000000;//��תalpha
    } while (--NumItems);
}

//alpha��ɫ��ʹ��DMA2D
//pColorFG:ǰ����洢����ַ 
//pColorBG:������洢����ַ
//pColorDst:����洢����ַ
//NumItems:����
static void DMA2D_AlphaBlendingBulk(LCD_COLOR *pColorFG,LCD_COLOR *pColorBG,LCD_COLOR *pColorDst,U32 NumItems) 
{
    u32 timeout=0;
    DMA2D->CR=0x00020000UL|(1<<9);              //�洢�����洢����ִ�л��
    DMA2D->FGMAR=(uint32_t)pColorFG;            //����ǰ����洢����ַ
    DMA2D->BGMAR=(uint32_t)pColorBG;            //���ñ�����洢����ַ
    DMA2D->OMAR=(uint32_t)pColorDst;            //��������洢����ַ
    DMA2D->FGOR=0;                              //����ǰ����ƫ��
    DMA2D->BGOR=0;                              //���ñ�����ƫ��
    DMA2D->OOR=0;                               //�������ƫ��
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;  //����ǰ������ɫ��ʽ
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;  //���ñ�������ɫ��ʽ
    DMA2D->OPFCCR =LTDC_PIXEL_FORMAT_ARGB8888;  //���������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //�������� 
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
}

//���������ɫ,���ǰ��ɫ�ͱ���ɫ
//Color:ǰ��ɫ
//BkColor:����ɫ
//Intens:���ǿ��
//����ֵ:��Ϻ����ɫ
static LCD_COLOR DMA2D_MixColors(LCD_COLOR Color,LCD_COLOR BkColor,U8 Intens) 
{
    uint32_t ColorFG,ColorBG,ColorDst;
    u32 timeout=0;
    if((BkColor&0xFF000000)==0xFF000000)return Color;//����ɫ͸��������Ҫ��ɫ
    ColorFG=Color^0xFF000000;
    ColorBG=BkColor^0xFF000000;
  
    DMA2D->CR=0x00020000UL|(1<<9);              //�洢�����洢����ִ�л��  
    DMA2D->FGMAR=(uint32_t)&ColorFG;            //����ǰ����洢����ַ
    DMA2D->BGMAR=(uint32_t)&ColorBG;            //���ñ�����洢����ַ
    DMA2D->OMAR=(uint32_t)&ColorDst;            //��������洢����ַ
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888|(1UL<<16)|((uint32_t)Intens<<24);         //����ǰ������ɫ��ʽ
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888|(0UL<<16)|((uint32_t)(255-Intens)<<24);   //���ñ�������ɫ��ʽ
    DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;   //���ñ�������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(1<<16)|1;             //����ÿ�������������� 
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
    return (ColorDst^0xFF000000);               //���ػ�Ϻ����ɫ
}

//ʹ��DMA2D������ɫת��
//pSrc:Դ��ɫ(ǰ��ɫ)
//pDst:ת�������ɫ(�����ɫ)
//PixelFormatSrc:ǰ������ɫ��ʽ
//PixelFormatDst:�����ɫ��ʽ
//NumItems:ÿ��������
static void DMA2D_ConvertColor(void * pSrc,void * pDst,uint32_t PixelFormatSrc,uint32_t PixelFormatDst,uint32_t NumItems)
{
    u32 timeout=0;
    DMA2D->CR=0x00010000UL|(1<<9);              //�洢�����洢����ִ��FPC
    DMA2D->FGMAR=(uint32_t)pSrc;                //ǰ����洢����ַ
    DMA2D->OMAR=(uint32_t)pDst;                 //����洢����ַ
    DMA2D->FGOR=0;                              //������ƫ��
    DMA2D->OOR=0;                               //���ƫ��
    DMA2D->FGPFCCR=PixelFormatSrc;              //ǰ������ɫ��ʽ
    DMA2D->OPFCCR=PixelFormatDst;               //�����ɫ��ʽ
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //����ÿ��������������
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
}

static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal,const GUI_BITMAP GUI_UNI_PTR * pBitmap,int LayerIndex)
{
    void (* pFunc)(void);
    int32_t DoDefault = 0;

    if (pBitmap->BitsPerPixel == 8) 
    {
        pFunc=LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
        if(pFunc) 
        {
            if(pBitmap->pPal) 
            {
                if(pBitmap->pPal->HasTrans) DoDefault = 1;
            }else DoDefault = 1;  
        }else DoDefault = 1;  
    } 
    else DoDefault = 1;
    if (DoDefault) return LCD_GetpPalConvTable(pLogPal);
    InvertAlpha_SwapRB((U32 *)pLogPal->pPalEntries, aBufferDMA2D, pLogPal->NumEntries);
    DMA2D_LoadLUT(aBufferDMA2D, pLogPal->NumEntries);
    return aBufferDMA2D;
}

//ʹ��DMA2D������ɫ���
//pColorFG:ǰ��ɫ�洢����ַ
//pColorBG:����ɫ�洢����ַ
//pColorDst:����洢����ַ
//Intens:���ǿ��
//NumItems:ÿ��������
static void DMA2D_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, uint32_t NumItems)
{
    u32 timeout=0;
    DMA2D->CR=0x00020000UL|(1<<9);              //�洢�����洢����ִ�л��
    DMA2D->FGMAR=(uint32_t)pColorFG;            //����ǰ��ɫ�洢����ַ
    DMA2D->BGMAR=(uint32_t)pColorBG;            //���ñ���ɫ�洢����ַ
    DMA2D->OMAR=(uint32_t)pColorDst;            //��������洢����ַ
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888   //����ǰ��ɫ��ɫ��ʽ
                    |(1UL<<16)
                    |((uint32_t)Intens<<24);
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888   //���ñ���ɫ��ɫ��ʽ
                    |(0UL<<16)
                    |((uint32_t)(255-Intens)<<24);    
    DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;   //���������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //����ÿ��������������    
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
}

//ʹ��DMA2D����alpha��ɫ
//pColorFG:ǰ��ɫ
//pColorBG:����ɫ
//pColorDst:�����ɫ
//NumItems:ÿ��������
static void DMA2D_AlphaBlending(LCD_COLOR *pColorFG,LCD_COLOR *pColorBG,LCD_COLOR *pColorDst,U32 NumItems)
{
    InvertAlpha(pColorFG,aBuffer_FG,NumItems);
    InvertAlpha(pColorBG,aBuffer_BG,NumItems);
    DMA2D_AlphaBlendingBulk(aBuffer_FG,aBuffer_BG,aBufferDMA2D,NumItems);
    InvertAlpha(aBufferDMA2D,pColorDst,NumItems);
}

//��ɫ����ת��Ϊ��ɫֵ
static void DMA2D_Index2ColorBulk(void *pIndex,LCD_COLOR *pColor,uint32_t NumItems, U8 SizeOfIndex,uint32_t PixelFormat)
{
    DMA2D_ConvertColor(pIndex,aBufferDMA2D,PixelFormat,LTDC_PIXEL_FORMAT_ARGB8888,NumItems);
    InvertAlpha_SwapRB(aBufferDMA2D,pColor,NumItems);
}

//��ɫֵת��Ϊ��ɫ����
static void DMA2D_Color2IndexBulk(LCD_COLOR *pColor,void *pIndex,uint32_t NumItems,U8 SizeOfIndex,uint32_t PixelFormat) 
{
    InvertAlpha_SwapRB(pColor,aBufferDMA2D,NumItems);
    DMA2D_ConvertColor(aBufferDMA2D,pIndex,LTDC_PIXEL_FORMAT_ARGB8888,PixelFormat,NumItems);
}

static void LCD_MixColorsBulk(U32 *pFG,U32 *pBG,U32 *pDst,unsigned OffFG,unsigned OffBG,unsigned OffDest,unsigned xSize,unsigned ySize,U8 Intens)
{
    int32_t y;

    GUI_USE_PARA(OffFG);
    GUI_USE_PARA(OffDest);
    for(y=0;y<ySize;y++) 
    {
        InvertAlpha(pFG,aBuffer_FG,xSize);
        InvertAlpha(pBG,aBuffer_BG,xSize);
        DMA2D_MixColorsBulk(aBuffer_FG,aBuffer_BG,aBufferDMA2D,Intens,xSize);
        InvertAlpha(aBufferDMA2D,pDst,xSize);
        pFG+=xSize+OffFG;
        pBG+=xSize+OffBG;
        pDst+=xSize+OffDest;
    }
}

//ʹ��DMA2D����L8��ɫ��ʽ��λͼ
static void DMA2D_DrawBitmapL8(void * pSrc, void * pDst,  uint32_t OffSrc, uint32_t OffDst, uint32_t PixelFormatDst, uint32_t xSize, uint32_t ySize)
{
    u32 timeout=0;
    DMA2D->CR=0x00010000UL|(1<<9);    
    DMA2D->FGMAR=(uint32_t)pSrc;                    
    DMA2D->OMAR=(uint32_t)pDst;                       
    DMA2D->FGOR=OffSrc;                      
    DMA2D->OOR=OffDst;                          
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_L8;             
    DMA2D->OPFCCR=PixelFormatDst;           
    DMA2D->NLR=(uint32_t)(xSize<<16)|ySize;      
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
}

//����16bppλͼ
static void LCD_DrawBitmap16bpp(int32_t LayerIndex,int32_t x,int32_t y,U16 const * p,int32_t xSize,int32_t ySize,int32_t BytesPerLine)
{
    uint32_t BufferSize, AddrDst;
    int32_t OffLineSrc, OffLineDst;

    BufferSize=GetBufferSize(LayerIndex);
    AddrDst=layer_prop[LayerIndex].address+BufferSize*layer_prop[LayerIndex].buffer_index+(y*layer_prop[LayerIndex].xSize+x)*layer_prop[LayerIndex].BytesPerPixel;
    OffLineSrc=(BytesPerLine/2)-xSize;
    OffLineDst=layer_prop[LayerIndex].xSize-xSize;
    DMA2D_CopyBuffer(LayerIndex,(void *)p,(void *)AddrDst,xSize,ySize,OffLineSrc,OffLineDst);
}

//����8bppλͼ
static void LCD_DrawBitmap8bpp(int32_t LayerIndex, int32_t x, int32_t y, U8 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine)
{ 
    uint32_t BufferSize, AddrDst;
    int32_t OffLineSrc, OffLineDst;
    uint32_t PixelFormat;

    BufferSize=GetBufferSize(LayerIndex);
    AddrDst=layer_prop[LayerIndex].address+BufferSize*layer_prop[LayerIndex].buffer_index+(y*layer_prop[LayerIndex].xSize + x)*layer_prop[LayerIndex].BytesPerPixel;
    OffLineSrc=BytesPerLine-xSize;
    OffLineDst=layer_prop[LayerIndex].xSize-xSize;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);
    DMA2D_DrawBitmapL8((void *)p,(void *)AddrDst,OffLineSrc,OffLineDst,PixelFormat,xSize,ySize);
}
