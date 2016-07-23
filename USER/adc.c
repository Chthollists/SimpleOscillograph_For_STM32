/***********************************************
 * �ļ���  ��adc.c
 * Ӳ�����ӣ�----------------------------------------------------
 *          |                                                    |
 *          | PA2 PA3 PA4
 *          |                                                    |
 *           ----------------------------------------------------
 * ��汾  ��ST3.5.0 
 * ˵��    ��ʹ��ADC����ͨ��
 * �޸�    ��2016/07/20  ��������
**************************************************/
#include "adc.h"

#define ADC1_DR_Address     ((uint32_t)0x40012400+0x4c)
#define ADC2_DR_Address     ((uint32_t)0x40012800+0x4c)
#define ADC3_DR_Address     ((uint32_t)0x40013C00+0x4c)

volatile uint16_t ADC_ConvertedValue[3];

/*
 * ��������ADC1_GPIO_Config
 * ����  ��ʹ��ADC1��DMA1��ʱ�ӣ���ʼ��PA 1 2 3
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOA clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configure PA  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		// PA,����ʱ������������
}


/* ��������ADC1_Mode_Config
 * ����  ������ADC1�Ĺ���ģʽΪMDAģʽ
 * ����  : ��
 * ���  ����
 * ����  ���ڲ�����
 */
static void ADC1_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	DMA_InitTypeDef DMA_InitStructure;
	
	/*DMA_channel1 configuration*/ 
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;     //ADC��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue; //�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 3;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;         //�ڴ��ַinc
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                 //ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 	*/
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 configuration *****************************************************************************************/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //����ADCģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	                    //����ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	                //��������ת��ģʽ������ͣ�ؽ���ADCת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//��ʹ���ⲿ����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //�ɼ������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 3;                             //Ҫת����ͨ����Ŀ3
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*����ADCʱ�� 12MHz*/
	//��ʱ������Ϊ12MHzʱ ת��ʱ�� = 14/12 = 1.167us.
	//�����ǰ�ϵͳʱ������Ϊ56MHz��ʱ�򣬾���4div����Ϊ14MHz����ʱ���ܴﵽ1us�Ĳ��������ǲ��Է��������������ٶ�ת��ADC������Ǻ�׼ȷ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*����ADC1��ͨ��11Ϊ55.	1���������ڣ�����Ϊ1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_1Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*��λУ׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	/*�ȴ�У׼�Ĵ�����λ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADCУ׼ */
	ADC_StartCalibration(ADC1);
	/* �ȴ�У׼���*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * ��������ADC1_Init
 * ����  ����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}

