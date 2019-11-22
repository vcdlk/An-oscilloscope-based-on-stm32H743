#include "DAC.h"

DAC_HandleTypeDef DAC1_Handler;			//DAC���
DAC_ChannelConfTypeDef DACCH1_Config;	//DAC1ͨ��1
static DAC_ChannelConfTypeDef sConfig;
static DMA_HandleTypeDef  hdma_dac1;
//DAC1 ��ʼ��
void DacInit(){
		DAC1_Handler.Instance = DAC1;
		HAL_DAC_Init(&DAC1_Handler);               	//��ʼ��DAC
		
		sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
		sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
		HAL_DAC_ConfigChannel(&DAC1_Handler, &sConfig, DAC_CHANNEL_1);
}
void dac1_InitForDMA(uint32_t _BufAddr, uint32_t _Count, uint32_t _DacFreq)
{	
		uint16_t usPeriod;
		uint16_t usPrescaler;
		uint32_t uiTIMxCLK;
		
		HAL_DAC_Stop_DMA(&DAC1_Handler,DAC_CHANNEL_1);// �ر�DAC��DMA


}








void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac){
    GPIO_InitTypeDef GPIO_Initure;
 /*##-1- Enable peripherals and GPIO Clocks #################################*/		
    __HAL_RCC_DAC12_CLK_ENABLE();      //ʹ��DACʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
		 __HAL_RCC_DMA1_CLK_ENABLE();			//����DMA1
	/*##-2- Configure peripheral GPIO ##########################################*/
    GPIO_Initure.Pin=GPIO_PIN_4;            //PA4
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	 /*##-3- Configure the DMA ##########*/
		HAL_DMA_DeInit(&hdma_dac1);
		hdma_dac1.Instance = DMA1_Stream5;
		hdma_dac1.Init.Request  = DMA_REQUEST_DAC1;
		hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_dac1.Init.Mode = DMA_CIRCULAR;
		hdma_dac1.Init.Priority = DMA_PRIORITY_LOW;

		HAL_DMA_Init(&hdma_dac1);
	/* Associate the initialized DMA handle to the DAC handle */
	
		__HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac1);	
}
void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
{
		__HAL_RCC_DAC12_FORCE_RESET();
		__HAL_RCC_DAC12_RELEASE_RESET();
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
		HAL_DMA_DeInit(hdac->DMA_Handle1);
}