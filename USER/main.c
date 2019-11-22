#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "sdram.h"
#include "mpu.h"
#include "timer.h"
#include "usmart.h"
#include "malloc.h"
#include "touch.h"
#include "usmart.h"
#include "pcf8574.h"
#include "adc.h"
#include "rtc.h"
#include "touch.h"
#include "GUI.h"
#include "includes.h"
#include "WM.h"
#include "DIALOG.h"
#include "radiodemo.h"
#include "MainTask.h"
#include  <os.h>
/************************************************
 ALIENTEK STM32������STemWinʵ��
 STemwin RADIO�ؼ�ʹ��
 
 UCOSIII���������ȼ��û�������ʹ�ã�ALIENTEK
 ����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
 ���ȼ�0���жϷ������������� OS_IntQTask()
 ���ȼ�1��ʱ�ӽ������� OS_TickTask()
 ���ȼ�2����ʱ���� OS_TmrTask()
 ���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
 ���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�������ȼ�
#define START_TASK_PRIO				3

//TOUCH����
//�����������ȼ�
#define TOUCH_TASK_PRIO				6
//�����ջ��С
#define TOUCH_STK_SIZE				256
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch����
void touch_task(void *p_arg);

//LED0����
//�����������ȼ�
#define LED0_TASK_PRIO 				7
//�����ջ��С
#define LED0_STK_SIZE				256
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0����
void led0_task(void *p_arg);
/*
*********************************************************************************************************
*                                       ��̬ȫ�ֱ���
*********************************************************************************************************
*/
// DSO������
OS_TCB  *AppTaskDsoTCB;
static  CPU_STK  *AppTaskDsoStk;
//��ʼ����
static  OS_TCB   *AppTaskStartTCB;
static  CPU_STK  *AppTaskStartStk;
//ͨѶ����
static  OS_TCB   *AppTaskMsgProTCB;
static  CPU_STK  *AppTaskMsgProStk;
//GUi����
static  OS_TCB   *AppTaskGUITCB;
static  CPU_STK  *AppTaskGUIStk;

/*
*********************************************************************************************************
*                                       �����ջ��С
*********************************************************************************************************
*/
#define  APP_CFG_TASK_DSO_STK_SIZE  										1024u
//#define  APP_CFG_TASK_START_STK_SIZE                    512u
#define  APP_CFG_TASK_MsgPro_STK_SIZE                   512u
#define  APP_CFG_TASK_GUI_STK_SIZE                      1024u

/*
*********************************************************************************************************
*                                       �������ȼ�
*********************************************************************************************************
*/
#define  APP_CFG_TASK_DSO_PRIO       													2U
//#define  APP_CFG_TASK_START_PRIO                              3u
#define  APP_CFG_TASK_MsgPro_PRIO                             4u
#define  APP_CFG_TASK_GUI_PRIO                        (OS_CFG_PRIO_MAX - 4u)
/*
*********************************************************************************************************
*                                      ��������
*********************************************************************************************************
*/
static  void  AppTaskDSO            (void     *p_arg);
static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskMsgPro         (void     *p_arg);
static  void  DispTaskInfo          (void);
static  void  App_Printf (CPU_CHAR *format, ...);
/*
*******************************************************************************************************
*                               ����
*******************************************************************************************************
*/
OS_SEM   SEM_SYNCH;	            /* ����ͬ�� */
static  OS_SEM   AppPrintfSemp;	/* ����printf���� */

arm_rfft_fast_instance_f32 S;
uint32_t fftSize = 2048; 
uint32_t ifftFlag = 0; 


//emwindemo_task����
static void AppTaskGUI(void *p_arg);
void MollcInit(void);

//�����ڴ棬���յ����ڴ����ʵ��
void MollcInit(void){
  my_mem_init(SRAMIN);			        //��ʼ���ڲ��ڴ��(AXI)
	my_mem_init(SRAMEX);			        //��ʼ���ⲿ�ڴ��(SDRAM)
	my_mem_init(SRAM12);			        //��ʼ��SRAM12�ڴ��(SRAM1+SRAM2)
	my_mem_init(SRAM4);				        //��ʼ��SRAM4�ڴ��(SRAM4)
	my_mem_init(SRAMDTCM);			        //��ʼ��DTCM�ڴ��(DTCM)
	my_mem_init(SRAMITCM);			        //��ʼ��ITCM�ڴ��(ITCM)
	//��ʼ����������ջ�Ϳ��ƿ�
	AppTaskStartTCB   = (OS_TCB *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskStartStk = (CPU_STK *)mymalloc(SRAMIN,sizeof(CPU_STK) *APP_CFG_TASK_START_STK_SIZE);
	//DSO���ݴ�������������ջ�Ϳ��ƿ�
	AppTaskDsoTCB  = (OS_TCB *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskDsoStk  = (CPU_STK *)mymalloc(SRAMIN, 
	                                    sizeof(CPU_STK)*APP_CFG_TASK_DSO_STK_SIZE);
	//ͨѶ���������ջ�Ϳ��ƿ�
	AppTaskMsgProTCB  = (OS_TCB  *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskMsgProStk  = (CPU_STK *)mymalloc(SRAMIN, 
	                                    sizeof(CPU_STK)*APP_CFG_TASK_MsgPro_STK_SIZE);
	//GUI�����ջ�Ϳ��ƿ�
	AppTaskGUITCB     = (OS_TCB  *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskGUIStk     = (CPU_STK *)mymalloc(SRAMIN, 
	                                    sizeof(CPU_STK)*APP_CFG_TASK_GUI_STK_SIZE);
		/* ����ʾ����ͨ��1��̬�ڴ� */
    g_DSO1 = (DSO_T *)mymalloc(SRAMIN, sizeof(DSO_T));
	/* ����ʾ����ͨ��2��̬�ڴ� */
    g_DSO2 = (DSO_T *)mymalloc(SRAMIN, sizeof(DSO_T));
	/* �����α�����ṹ�������̬�ڴ� */
    g_Cursors = (CURSORS_T *)mymalloc(SRAMIN, sizeof(CURSORS_T));
	/* �����־λ�ṹ�������̬�ڴ� */
		g_Flag = (FLAG_T *)mymalloc(SRAMIN, sizeof(FLAG_T));
	/* ���봥���ṹ�������̬�ڴ� */
		g_TrigVol = (TRIVOLTAGE_T *)mymalloc(SRAMIN, sizeof(TRIVOLTAGE_T));
	/* ����FFT��̬�ڴ� */
		testInput_fft_2048  = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*2048);
		testOutput_fft_2048 = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*2048);
	/* ����RMS��̬�ڴ� */	
		g_RMSBUF = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*600);
	/* ����FIR��̬�ڴ� */
  FirDataInput = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*FIR_LENGTH_SAMPLES);
  FirDataOutput = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*FIR_LENGTH_SAMPLES);
  firStateF32 = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*FIR_StateBufSize);
}
int main(void)
{
  OS_ERR err;
	CPU_SR_ALLOC();
    
  Write_Through();                //Cahceǿ��͸д
  MPU_Memory_Protection();        //������ش洢����
  Cache_Enable();                 //��L1-Cache
	HAL_Init();				        			//��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);    //����ʱ��,400Mhz 
	delay_init(400);			    			//��ʱ��ʼ��
	uart_init(115200);			    		//���ڳ�ʼ��
	LED_Init();					    				//��ʼ��LED
	KEY_Init();					    				//��ʼ������
  MY_ADC_Init();                  //��ʼ��ADC 
  PCF8574_Init();                 //��ʼ��PCF8574
	SDRAM_Init();                   //��ʼ��SDRAM
	TFTLCD_Init();				    			//��ʼ��LCD
	tp_dev.init();				   				//��������ʼ�� 
	MollcInit();										//��ʼ���ڴ�
  OSInit(&err);		            		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();            //�����ٽ���
	//������ʼ����
	OSTaskCreate(	 (OS_TCB 	* )AppTaskStartTCB,		//������ƿ�
								 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )AppTaskStart, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )APP_CFG_TASK_START_PRIO,     //�������ȼ�
                 (CPU_STK  *)&AppTaskStartStk[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY)0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	            //�˳��ٽ���	 
	OSStart(&err);                  //����UCOSIII
	return (0);						     
}

//��ʼ������
void AppTaskStart(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	//ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//ʹ��CRCʱ��

	
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	
	/* 
	   �����ź�����ֵΪ1��ʱ�����ʵ�ֻ��⹦�ܣ�Ҳ����ֻ��һ����Դ����ʹ�� 
	   �������ǽ�����1�Ĵ�ӡ������Ϊ��������Դ����ֹ���ڴ�ӡ��ʱ������������ռ
	   ��ɴ��ڴ�ӡ���ҡ�
	*/
	//��ӦAppObjectCreate()����
	OSSemCreate((OS_SEM    *)&AppPrintfSemp,
							(CPU_CHAR  *)"AppPrintfSemp",
							(OS_SEM_CTR )1,
							(OS_ERR    *)&err);	
	
	/* ��������ֵΪ0������ʵ������ͬ������ */
	OSSemCreate((OS_SEM    *)&SEM_SYNCH,
							(CPU_CHAR  *)"SEM_SYNCH",
							(OS_SEM_CTR )0,
							(OS_ERR    *)&err);


	//��ӦAppTaskGuiTcb����	
	OSTaskCreate(  (OS_TCB*     )AppTaskGUITCB,		
				         (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )AppTaskGUI, 			
                 (void*       )0,					
                 (OS_PRIO	    )APP_CFG_TASK_GUI_PRIO,     
                 (CPU_STK*    )&AppTaskGUIStk[0],	
                 (CPU_STK_SIZE)APP_CFG_TASK_GUI_STK_SIZE/10,	
                 (CPU_STK_SIZE)APP_CFG_TASK_GUI_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	    )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR*     )&err);
	//����������
	OSTaskCreate(  (OS_TCB*     )&TouchTaskTCB,		
				         (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	    )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	    )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR*     )&err);			 
	//LED0����
	OSTaskCreate(	 (OS_TCB*     )&Led0TaskTCB,		
								 (CPU_CHAR*   )"Led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK*    )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR*     )&err);	 
	//����DSO����  ��ӦAppTaskDsoTcb����		 
	OSTaskCreate((OS_TCB       *)AppTaskDsoTCB,              
                 (CPU_CHAR     *)"App Task DSO",
                 (OS_TASK_PTR   )AppTaskDSO, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_DSO_PRIO,
                 (CPU_STK      *)&AppTaskDsoStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_DSO_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_DSO_STK_SIZE,
                 (OS_MSG_QTY    )5,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);		
	/**************����MsgPro����*********************/
	OSTaskCreate(  (OS_TCB       *)AppTaskMsgProTCB,             
                 (CPU_CHAR     *)"App Task MsgPro",
                 (OS_TASK_PTR   )AppTaskMsgPro, 
                 (void         *)0,
                 (OS_PRIO       )APP_CFG_TASK_MsgPro_PRIO,
                 (CPU_STK      *)&AppTaskMsgProStk[0],
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgPro_STK_SIZE / 10,
                 (CPU_STK_SIZE  )APP_CFG_TASK_MsgPro_STK_SIZE,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);							 
	OS_TaskSuspend((OS_TCB*)AppTaskStartTCB,&err);		//����ʼ����
								 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//EMWINDEMO����
void AppTaskGUI(void *p_arg)
{
		GUI_Init();  						//STemWin��ʼ��
		WM_MULTIBUF_Enable(1);  //����STemWin�໺��,RGB�����ܻ��õ�
		while(1){
		MainTask();
		}	
}

//TOUCH����
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
    tp_dev.scan(0);
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms
	}
}

//LED0����
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}
/*
*********************************************************************************************************
*	�� �� ��: AppTaskDSO
*	����˵��: ˫ͨ��ʾ�������ݴ�������
*	��    ��: ��
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************
*/
static void AppTaskDSO (void *p_arg)
{
	OS_ERR     		 err;
	CPU_TS	   		 ts;
	void			 	   *p_msg;
	OS_MSG_SIZE		 msg_size;
	uint32_t	  	 *ucReceive,
	
	/* ʵ������FFT���� */
	fftSize = 2048; 
	
	/* ���任 */
   ifftFlag = 0; 
	
	/* ��ʼ���ṹ��S�еĲ��� */
 	arm_rfft_fast_init_f32(&S, fftSize);
	
    while(1)
    {
		/* �������� */
		p_msg = OSTaskQPend(0,
							OS_OPT_PEND_BLOCKING,
							&msg_size,   /* �˲����ǽ��յ����ݸ��� */
							&ts,
							&err);
		
		if(err == OS_ERR_NONE)
		{
			ucReceive = (uint32_t *)p_msg;
			
			switch (*ucReceive)
            {
				/* ˫ͨ�����δ��� */
				case DspFFT2048Pro_15:
					/* ��ȡ����ADC1��λ�� */
					g_DSO1->usCurPos = 10240 - DMA2_Stream1->NDTR;
				
					/* ��ȡ����ADC3��λ�� */
					g_DSO2->usCurPos = 10240 - DMA1_Stream1->NDTR;
				
					DSO2_WaveTrig(g_DSO2->usCurPos);
					DSO1_WaveTrig(g_DSO1->usCurPos);
					DSO2_WaveProcess();
					DSO1_WaveProcess();
					break;

				/* ���ڼ򵥵�ADC���ݲɼ� */
				case DspMultiMeterPro_14:
					g_uiAdcAvgSample = ADC_GetSampleAvgN();
					break;
				
				/* �����ڵ���Ŀ�ģ���ӡ�����ִ�������Ĭ�ϲ�ʹ�� */
				case DspTaskInfo_13:
					DispTaskInfo();
					break;
				
				/* ����λ��δʹ�� */
				default:
					App_Printf("*ucReceive = %x\r\n", *ucReceive);
					break;
            }
		}
    }
}

/*
*********************************************************************************************************
*	�� �� ��: App_Printf
*	����˵��: �̰߳�ȫ��printf��ʽ		  			  
*	��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  App_Printf(CPU_CHAR *format, ...)
{
    CPU_CHAR  buf_str[80 + 1];
    va_list   v_args;
    OS_ERR    os_err;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

	/* ������� */
    OSSemPend((OS_SEM  *)&AppPrintfSemp,
              (OS_TICK  )0u,
              (OS_OPT   )OS_OPT_PEND_BLOCKING,
              (CPU_TS  *)0,
              (OS_ERR  *)&os_err);

    printf("%s", buf_str);

   (void)OSSemPost((OS_SEM  *)&AppPrintfSemp,
                   (OS_OPT   )OS_OPT_POST_1,
                   (OS_ERR  *)&os_err);

}
/*
*********************************************************************************************************
*	�� �� ��: DispTaskInfo
*	����˵��: ��uCOS-III������Ϣͨ�����ڴ�ӡ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispTaskInfo(void)
{
//	OS_TCB      *p_tcb;	        /* ����һ��������ƿ�ָ��, TCB = TASK CONTROL BLOCK */
//	float CPU = 0.0f;
//	CPU_SR_ALLOC();

//	CPU_CRITICAL_ENTER();
//  p_tcb = OSTaskDbgListPtr;
//    CPU_CRITICAL_EXIT();
//	
//	/* ��ӡ���� */
//	App_Printf("===============================================================\r\n");
//	App_Printf(" ���ȼ� ʹ��ջ ʣ��ջ �ٷֱ� ������   ������\r\n");
//	App_Printf("  Prio   Used  Free   Per    CPU     Taskname\r\n");

//	/* ����������ƿ��б�(TCB list)����ӡ���е���������ȼ������� */
//	while (p_tcb != (OS_TCB *)0) 
//	{
//		CPU = (float)p_tcb->CPUUsage / 100;
//		App_Printf("   %2d  %5d  %5d   %02d%%   %5.2f%%   %s\r\n", 
//		p_tcb->Prio, 
//		p_tcb->StkUsed, 
//		p_tcb->StkFree, 
//		(p_tcb->StkUsed * 100) / (p_tcb->StkUsed + p_tcb->StkFree),
//		CPU,
//		p_tcb->NamePtr);		
//	 	
//		CPU_CRITICAL_ENTER();
//        p_tcb = p_tcb->DbgNextPtr;
//        CPU_CRITICAL_EXIT();
//	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMsgPro
*	����˵��: ʵ�ֽ�ͼ���ܣ���ͼƬ��BMP��ʽ���浽SD����
*	��    ��: p_arg ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 4
*********************************************************************************************************
*/
static void AppTaskMsgPro(void *p_arg)
{



}
