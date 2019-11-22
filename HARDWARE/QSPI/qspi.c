#include "qspi.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//QSPI��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/8/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

QSPI_HandleTypeDef QSPI_Handler;    //QSPI���
    
//QSPI��ʼ��
u8 QSPI_Init(void)
{
    QSPI_Handler.Instance=QUADSPI;                          //QSPI
    QSPI_Handler.Init.ClockPrescaler=1;                     //QPSI��Ƶ�ȣ�W25Q256���Ƶ��Ϊ104M��
                                                            //���Դ˴�Ӧ��Ϊ2��QSPIƵ�ʾ�Ϊ200/(1+1)=100MHZ
    QSPI_Handler.Init.FifoThreshold=4;                      //FIFO��ֵΪ4���ֽ�
    QSPI_Handler.Init.SampleShifting=QSPI_SAMPLE_SHIFTING_HALFCYCLE;//������λ�������(DDRģʽ��,��������Ϊ0)
    QSPI_Handler.Init.FlashSize=POSITION_VAL(0X2000000)-1;  //SPI FLASH��С��W25Q256��СΪ32M�ֽ�
    QSPI_Handler.Init.ChipSelectHighTime=QSPI_CS_HIGH_TIME_5_CYCLE;//Ƭѡ�ߵ�ƽʱ��Ϊ5��ʱ��(10*5=55ns),���ֲ������tSHSL����
    QSPI_Handler.Init.ClockMode=QSPI_CLOCK_MODE_0;          //ģʽ0
    QSPI_Handler.Init.FlashID=QSPI_FLASH_ID_1;              //��һƬflash
    QSPI_Handler.Init.DualFlash=QSPI_DUALFLASH_DISABLE;     //��ֹ˫����ģʽ
    if(HAL_QSPI_Init(&QSPI_Handler)==HAL_OK) return 0;      //QSPI��ʼ���ɹ�
    else return 1;
}

//QSPI�ײ�����,�������ã�ʱ��ʹ��
//�˺����ᱻHAL_QSPI_Init()����
//hqspi:QSPI���
void HAL_QSPI_MspInit(QSPI_HandleTypeDef *hqspi)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_QSPI_CLK_ENABLE();        //ʹ��QSPIʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();       //ʹ��GPIOBʱ��
    __HAL_RCC_GPIOF_CLK_ENABLE();       //ʹ��GPIOFʱ��
    
    //��ʼ��PB6 Ƭѡ�ź�
    GPIO_Initure.Pin=GPIO_PIN_6;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //����
    GPIO_Initure.Pull=GPIO_PULLUP;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;  //����
    GPIO_Initure.Alternate=GPIO_AF10_QUADSPI;   //����ΪQSPI
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //PF8,9
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Pull=GPIO_NOPULL;              
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;   //����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //PB2
    GPIO_Initure.Pin=GPIO_PIN_2;
    GPIO_Initure.Alternate=GPIO_AF9_QUADSPI;   //����ΪQSPI
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    //PF6,7
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}

//QSPI��������
//instruction:Ҫ���͵�ָ��
//address:���͵���Ŀ�ĵ�ַ
//dummyCycles:��ָ��������
//	instructionMode:ָ��ģʽ;QSPI_INSTRUCTION_NONE,QSPI_INSTRUCTION_1_LINE,QSPI_INSTRUCTION_2_LINE,QSPI_INSTRUCTION_4_LINE  
//	addressMode:��ַģʽ; QSPI_ADDRESS_NONE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_2_LINE,QSPI_ADDRESS_4_LINE
//	addressSize:��ַ����;QSPI_ADDRESS_8_BITS,QSPI_ADDRESS_16_BITS,QSPI_ADDRESS_24_BITS,QSPI_ADDRESS_32_BITS
//	dataMode:����ģʽ; QSPI_DATA_NONE,QSPI_DATA_1_LINE,QSPI_DATA_2_LINE,QSPI_DATA_4_LINE

void QSPI_Send_CMD(u32 instruction,u32 address,u32 dummyCycles,u32 instructionMode,u32 addressMode,u32 addressSize,u32 dataMode)
{
    QSPI_CommandTypeDef Cmdhandler;
    
    Cmdhandler.Instruction=instruction;                 	//ָ��
    Cmdhandler.Address=address;                            	//��ַ
    Cmdhandler.DummyCycles=dummyCycles;                     //���ÿ�ָ��������
    Cmdhandler.InstructionMode=instructionMode;				//ָ��ģʽ
    Cmdhandler.AddressMode=addressMode;   					//��ַģʽ
    Cmdhandler.AddressSize=addressSize;   					//��ַ����
    Cmdhandler.DataMode=dataMode;             				//����ģʽ
    Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;       	//ÿ�ζ�����ָ��
    Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE; //�޽����ֽ�
    Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;           	//�ر�DDRģʽ
    Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;

    HAL_QSPI_Command(&QSPI_Handler,&Cmdhandler,5000);
}

//QSPI����ָ�����ȵ�����
//buf:�������ݻ������׵�ַ
//datalen:Ҫ��������ݳ���
//����ֵ:0,����
//    ����,�������
u8 QSPI_Receive(u8* buf,u32 datalen)
{
    QSPI_Handler.Instance->DLR=datalen-1;                           //�������ݳ���
    if(HAL_QSPI_Receive(&QSPI_Handler,buf,5000)==HAL_OK) return 0;  //��������
    else return 1;
}

//QSPI����ָ�����ȵ�����
//buf:�������ݻ������׵�ַ
//datalen:Ҫ��������ݳ���
//����ֵ:0,����
//    ����,�������
u8 QSPI_Transmit(u8* buf,u32 datalen)
{
    QSPI_Handler.Instance->DLR=datalen-1;                            //�������ݳ���
    if(HAL_QSPI_Transmit(&QSPI_Handler,buf,5000)==HAL_OK) return 0;  //��������
    else return 1;
}
