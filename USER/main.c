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
 ALIENTEK STM32开发板STemWin实验
 STemwin RADIO控件使用
 
 UCOSIII中以下优先级用户程序不能使用，ALIENTEK
 将这些优先级分配给了UCOSIII的5个系统内部任务
 优先级0：中断服务服务管理任务 OS_IntQTask()
 优先级1：时钟节拍任务 OS_TickTask()
 优先级2：定时任务 OS_TmrTask()
 优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
 优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com  
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//任务优先级
#define START_TASK_PRIO				3

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				6
//任务堆栈大小
#define TOUCH_STK_SIZE				256
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 				7
//任务堆栈大小
#define LED0_STK_SIZE				256
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0任务
void led0_task(void *p_arg);
/*
*********************************************************************************************************
*                                       静态全局变量
*********************************************************************************************************
*/
// DSO任务定义
OS_TCB  *AppTaskDsoTCB;
static  CPU_STK  *AppTaskDsoStk;
//开始任务
static  OS_TCB   *AppTaskStartTCB;
static  CPU_STK  *AppTaskStartStk;
//通讯任务
static  OS_TCB   *AppTaskMsgProTCB;
static  CPU_STK  *AppTaskMsgProStk;
//GUi任务
static  OS_TCB   *AppTaskGUITCB;
static  CPU_STK  *AppTaskGUIStk;

/*
*********************************************************************************************************
*                                       任务堆栈大小
*********************************************************************************************************
*/
#define  APP_CFG_TASK_DSO_STK_SIZE  										1024u
//#define  APP_CFG_TASK_START_STK_SIZE                    512u
#define  APP_CFG_TASK_MsgPro_STK_SIZE                   512u
#define  APP_CFG_TASK_GUI_STK_SIZE                      1024u

/*
*********************************************************************************************************
*                                       任务优先级
*********************************************************************************************************
*/
#define  APP_CFG_TASK_DSO_PRIO       													2U
//#define  APP_CFG_TASK_START_PRIO                              3u
#define  APP_CFG_TASK_MsgPro_PRIO                             4u
#define  APP_CFG_TASK_GUI_PRIO                        (OS_CFG_PRIO_MAX - 4u)
/*
*********************************************************************************************************
*                                      函数声明
*********************************************************************************************************
*/
static  void  AppTaskDSO            (void     *p_arg);
static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskMsgPro         (void     *p_arg);
static  void  DispTaskInfo          (void);
static  void  App_Printf (CPU_CHAR *format, ...);
/*
*******************************************************************************************************
*                               变量
*******************************************************************************************************
*/
OS_SEM   SEM_SYNCH;	            /* 用于同步 */
static  OS_SEM   AppPrintfSemp;	/* 用于printf互斥 */

arm_rfft_fast_instance_f32 S;
uint32_t fftSize = 2048; 
uint32_t ifftFlag = 0; 


//emwindemo_task任务
static void AppTaskGUI(void *p_arg);
void MollcInit(void);

//分配内存，参照的是内存分配实验
void MollcInit(void){
  my_mem_init(SRAMIN);			        //初始化内部内存池(AXI)
	my_mem_init(SRAMEX);			        //初始化外部内存池(SDRAM)
	my_mem_init(SRAM12);			        //初始化SRAM12内存池(SRAM1+SRAM2)
	my_mem_init(SRAM4);				        //初始化SRAM4内存池(SRAM4)
	my_mem_init(SRAMDTCM);			        //初始化DTCM内存池(DTCM)
	my_mem_init(SRAMITCM);			        //初始化ITCM内存池(ITCM)
	//开始任务的任务堆栈和控制块
	AppTaskStartTCB   = (OS_TCB *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskStartStk = (CPU_STK *)mymalloc(SRAMIN,sizeof(CPU_STK) *APP_CFG_TASK_START_STK_SIZE);
	//DSO数据处理任务的任务堆栈和控制块
	AppTaskDsoTCB  = (OS_TCB *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskDsoStk  = (CPU_STK *)mymalloc(SRAMIN, 
	                                    sizeof(CPU_STK)*APP_CFG_TASK_DSO_STK_SIZE);
	//通讯任务任务堆栈和控制块
	AppTaskMsgProTCB  = (OS_TCB  *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskMsgProStk  = (CPU_STK *)mymalloc(SRAMIN, 
	                                    sizeof(CPU_STK)*APP_CFG_TASK_MsgPro_STK_SIZE);
	//GUI任务堆栈和控制块
	AppTaskGUITCB     = (OS_TCB  *)mymalloc(SRAMIN,sizeof(OS_TCB));
	AppTaskGUIStk     = (CPU_STK *)mymalloc(SRAMIN, 
	                                    sizeof(CPU_STK)*APP_CFG_TASK_GUI_STK_SIZE);
		/* 申请示波器通道1动态内存 */
    g_DSO1 = (DSO_T *)mymalloc(SRAMIN, sizeof(DSO_T));
	/* 申请示波器通道2动态内存 */
    g_DSO2 = (DSO_T *)mymalloc(SRAMIN, sizeof(DSO_T));
	/* 申请游标测量结构体变量动态内存 */
    g_Cursors = (CURSORS_T *)mymalloc(SRAMIN, sizeof(CURSORS_T));
	/* 申请标志位结构体变量动态内存 */
		g_Flag = (FLAG_T *)mymalloc(SRAMIN, sizeof(FLAG_T));
	/* 申请触发结构体变量动态内存 */
		g_TrigVol = (TRIVOLTAGE_T *)mymalloc(SRAMIN, sizeof(TRIVOLTAGE_T));
	/* 申请FFT动态内存 */
		testInput_fft_2048  = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*2048);
		testOutput_fft_2048 = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*2048);
	/* 申请RMS动态内存 */	
		g_RMSBUF = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*600);
	/* 申请FIR动态内存 */
  FirDataInput = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*FIR_LENGTH_SAMPLES);
  FirDataOutput = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*FIR_LENGTH_SAMPLES);
  firStateF32 = (float32_t *)mymalloc(SRAMIN, sizeof(float32_t)*FIR_StateBufSize);
}
int main(void)
{
  OS_ERR err;
	CPU_SR_ALLOC();
    
  Write_Through();                //Cahce强制透写
  MPU_Memory_Protection();        //保护相关存储区域
  Cache_Enable();                 //打开L1-Cache
	HAL_Init();				        			//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);    //设置时钟,400Mhz 
	delay_init(400);			    			//延时初始化
	uart_init(115200);			    		//串口初始化
	LED_Init();					    				//初始化LED
	KEY_Init();					    				//初始化按键
  MY_ADC_Init();                  //初始化ADC 
  PCF8574_Init();                 //初始化PCF8574
	SDRAM_Init();                   //初始化SDRAM
	TFTLCD_Init();				    			//初始化LCD
	tp_dev.init();				   				//触摸屏初始化 
	MollcInit();										//初始化内存
  OSInit(&err);		            		//初始化UCOSIII
	OS_CRITICAL_ENTER();            //进入临界区
	//创建开始任务
	OSTaskCreate(	 (OS_TCB 	* )AppTaskStartTCB,		//任务控制块
								 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )AppTaskStart, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )APP_CFG_TASK_START_PRIO,     //任务优先级
                 (CPU_STK  *)&AppTaskStartStk[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)APP_CFG_TASK_START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY)0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT    )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	            //退出临界区	 
	OSStart(&err);                  //开启UCOSIII
	return (0);						     
}

//开始任务函数
void AppTaskStart(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	//使能时间片轮转调度功能,设置默认的时间片长度
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟

	
	
	OS_CRITICAL_ENTER();	//进入临界区
	
	/* 
	   创建信号量数值为1的时候可以实现互斥功能，也就是只有一个资源可以使用 
	   本例程是将串口1的打印函数作为保护的资源。防止串口打印的时候被其它任务抢占
	   造成串口打印错乱。
	*/
	//对应AppObjectCreate()任务
	OSSemCreate((OS_SEM    *)&AppPrintfSemp,
							(CPU_CHAR  *)"AppPrintfSemp",
							(OS_SEM_CTR )1,
							(OS_ERR    *)&err);	
	
	/* 创建计数值为0，用于实现任务同步功能 */
	OSSemCreate((OS_SEM    *)&SEM_SYNCH,
							(CPU_CHAR  *)"SEM_SYNCH",
							(OS_SEM_CTR )0,
							(OS_ERR    *)&err);


	//对应AppTaskGuiTcb任务	
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
	//触摸屏任务
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
	//LED0任务
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
	//创建DSO任务  对应AppTaskDsoTcb任务		 
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
	/**************创建MsgPro任务*********************/
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
	OS_TaskSuspend((OS_TCB*)AppTaskStartTCB,&err);		//挂起开始任务
								 
	OS_CRITICAL_EXIT();	//退出临界区
}

//EMWINDEMO任务
void AppTaskGUI(void *p_arg)
{
		GUI_Init();  						//STemWin初始化
		WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
		while(1){
		MainTask();
		}	
}

//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
    tp_dev.scan(0);
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

//LED0任务
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskDSO
*	功能说明: 双通道示波器数据处理任务。
*	形    参: 无
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
static void AppTaskDSO (void *p_arg)
{
	OS_ERR     		 err;
	CPU_TS	   		 ts;
	void			 	   *p_msg;
	OS_MSG_SIZE		 msg_size;
	uint32_t	  	 *ucReceive,
	
	/* 实数序列FFT长度 */
	fftSize = 2048; 
	
	/* 正变换 */
   ifftFlag = 0; 
	
	/* 初始化结构体S中的参数 */
 	arm_rfft_fast_init_f32(&S, fftSize);
	
    while(1)
    {
		/* 接受数据 */
		p_msg = OSTaskQPend(0,
							OS_OPT_PEND_BLOCKING,
							&msg_size,   /* 此参数是接收到数据个数 */
							&ts,
							&err);
		
		if(err == OS_ERR_NONE)
		{
			ucReceive = (uint32_t *)p_msg;
			
			switch (*ucReceive)
            {
				/* 双通道波形处理 */
				case DspFFT2048Pro_15:
					/* 读取的是ADC1的位置 */
					g_DSO1->usCurPos = 10240 - DMA2_Stream1->NDTR;
				
					/* 读取的是ADC3的位置 */
					g_DSO2->usCurPos = 10240 - DMA1_Stream1->NDTR;
				
					DSO2_WaveTrig(g_DSO2->usCurPos);
					DSO1_WaveTrig(g_DSO1->usCurPos);
					DSO2_WaveProcess();
					DSO1_WaveProcess();
					break;

				/* 用于简单的ADC数据采集 */
				case DspMultiMeterPro_14:
					g_uiAdcAvgSample = ADC_GetSampleAvgN();
					break;
				
				/* 仅用于调试目的，打印任务的执行情况，默认不使用 */
				case DspTaskInfo_13:
					DispTaskInfo();
					break;
				
				/* 其它位暂未使用 */
				default:
					App_Printf("*ucReceive = %x\r\n", *ucReceive);
					break;
            }
		}
    }
}

/*
*********************************************************************************************************
*	函 数 名: App_Printf
*	功能说明: 线程安全的printf方式		  			  
*	形    参: 同printf的参数。
*             在C中，当无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
*	返 回 值: 无
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

	/* 互斥操作 */
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
*	函 数 名: DispTaskInfo
*	功能说明: 将uCOS-III任务信息通过串口打印出来
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispTaskInfo(void)
{
//	OS_TCB      *p_tcb;	        /* 定义一个任务控制块指针, TCB = TASK CONTROL BLOCK */
//	float CPU = 0.0f;
//	CPU_SR_ALLOC();

//	CPU_CRITICAL_ENTER();
//  p_tcb = OSTaskDbgListPtr;
//    CPU_CRITICAL_EXIT();
//	
//	/* 打印标题 */
//	App_Printf("===============================================================\r\n");
//	App_Printf(" 优先级 使用栈 剩余栈 百分比 利用率   任务名\r\n");
//	App_Printf("  Prio   Used  Free   Per    CPU     Taskname\r\n");

//	/* 遍历任务控制块列表(TCB list)，打印所有的任务的优先级和名称 */
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
*	函 数 名: AppTaskMsgPro
*	功能说明: 实现截图功能，将图片以BMP格式保存到SD卡中
*	形    参: p_arg 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 4
*********************************************************************************************************
*/
static void AppTaskMsgPro(void *p_arg)
{



}
