/************************SimpleOscillograph For STM32******************************
 * �ļ���  ��main.c
 * ����    ��SimpleOscillograph For STM32      
 * ƽ̨    ��STM32���İ�
 * ��汾  ��ST3.5.0
 *
 * ����    ����˧
 * ����    ��2015-10-24
 * �޸�    ��2016-01-26
 * �޸�    ��2016/01/10
 * �޸�    ��2016/04/01  ����USB
 * �޸�    ��2016/04/14  �����Զ�������Ϊһֱ���� ���ӱ��ش���
 * ע��    ���ж����ȼ�������0-0.0 > ��ʱ��2-1.0 > �ⲿ�ж�PB01-2.0
**********************************************************************************/

#include <string.h>
#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"
#include "exti.h"
#include "led.h"
#include "Time.h"
#include "mydef.h"
#include "HW_Config.h"
#include "usb_mem.h"

/* Private function prototypes -----------------------------------------------*/
void scan(void);
void send(void);
void on_send_finish(void);


// interval_time ����ʱ������1/FS
volatile u32 FS = 100000u;				//��ʼ��ʾ100KHz
//channel
volatile u8 is_one_channel = 0;
//abort flag
volatile u8 is_abort = 0;         //1���������η���
// scan mode
volatile u8 scan_mode 	= 0;		  //0��ʵʱ����������ģʽ�� 1���ɼ���ͬʱ�������� �ʺϵ�Ƶ�ź�
volatile u16 value_i		= 0;	  	//��¼���ڲɼ������εڼ�����  ��Χ��0��(number_of_data-1)
volatile u8 SCAN_IS_OK	= 0;		  //����Ƿ�Ϊ�ɼ����
// TriggerMode
volatile u8 trigger_mode 	= 0;		  //0����������Զ����� 1 2 3�ֱ�Ϊ���� �½� ���ش���
volatile EXTITrigger_TypeDef TriggerMode;  //���ڼ�¼�ж�ģʽ(����ģʽ)

// ������ �Ӵ�����ʼ���Բ���Ƶ��FS�洢num_of_data���㣬֮��һ����������
__IO u16 C1_value[num_of_data];
__IO u16 C2_value[num_of_data];
__IO u16 C3_value[num_of_data];

// ADCת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[3];

/*
 * ������
 */
int main(void)
{
	// �ֲ����������ڱ���ת�������ĵ�ѹֵ			 
	u16 ADC_ConvertedValueLocal;
	
	#if USE_VIRTUAL_COM
	  //��ʼ��USB�����豸�����⴮�ڣ�
	  USB_Config();
	#endif
	
	/* LED CONFIG */
	//PC13
	LED_GPIO_Config();
	LED(ON);
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* TIM2 ��ʱ���� */
	TIM2_NVIC_Configuration();
  TIM2_Configuration(FS);	//��ʼFS(100KHz)
	
	/* USART1 config */
	USART1_Config();
	
	/* enable adc1 and config adc1 to dma mode */
	ADC1_Init();
	
	/* exti line config */
	PB1_Config();
	
	//��ʼ���رմ����ж�
	EXTI_DISABLE;
	
	PRINTF_LABVIEW("\r\n------ SimpleOscillograph For STM32 ------\r\n");
	
	//����ADC��Ϣ������λ������adԭʼֵ
	PRINTF_LABVIEW("VC:%f\r\n", VCC_V);	//VCC
	PRINTF_LABVIEW("AD:%d\r\n", ADC_B);	//adcλ��
	
	//��ʼһ�βɼ������ڵ���
	START_TIMER_NEW_SCAN;
	while (1)
	{
		if (scan_mode == 1)
		{
			//ʵʱ����ʵʱ��ʾ
			ADC_ConvertedValueLocal = ADC_ConvertedValue[0]; // ��ȡת����ADֵ
			PRINTF_LABVIEW("C1:%d\r\n", ADC_ConvertedValueLocal); 
		
			ADC_ConvertedValueLocal = ADC_ConvertedValue[1]; // ��ȡת����ADֵ
			PRINTF_LABVIEW("C2:%d\r\n", ADC_ConvertedValueLocal); 
		
			ADC_ConvertedValueLocal = ADC_ConvertedValue[2]; // ��ȡת����ADֵ
			PRINTF_LABVIEW("C3:%d\r\n", ADC_ConvertedValueLocal);
		}
		if(SCAN_IS_OK == 1)
		{
			SCAN_IS_OK = 0;
			send();
		}
	}
	//return 0;
}

/*
 * ���ݷ��ͺ�����ע�⿪ͷ�ͽ�βҪ��һ�����ࣨ�����λ���Ļ�������
 * ֱ�ӷ���ԭʼֵ��������λ������
 * ������Ϻ������ⲿ�ж��������ٴδ���
 */
void send(void)
{
	u16 i = 0;
	
	//����ADC��Ϣ������λ������adԭʼֵ
	PRINTF_LABVIEW("VC:%f\r\n", VCC_V);	//VCC
	PRINTF_LABVIEW("AD:%d\r\n", ADC_B);	//adcλ��
	PRINTF_LABVIEW("FS:%d\r\n", FS);
	PRINTF_LABVIEW("NM:%d\r\n", num_of_data);
	
	is_abort = 0;

	do
	{
		PRINTF_LABVIEW("C1:%d\r\n", C1_value[i]);
		PRINTF_LABVIEW("C2:%d\r\n", C2_value[i]);
		PRINTF_LABVIEW("C3:%d\r\n", C3_value[i]);
	}
	while((++i < num_of_data) && (is_abort == 0) && (scan_mode == 0));
	
	PRINTF_LABVIEW("FN:OK\r\n");
	PRINTF_LABVIEW("SEND_FINISHED!!!\r\n");	//��������
	on_send_finish();
}


//���������Ҫ����һЩ����������
void on_send_finish(void)
{
	LED(OFF);
  if(scan_mode == 0)
	{
		if(trigger_mode == 0)
		{
			//��������Զ�����
			//��ʼһ�βɼ������ڵ���
	    START_TIMER_NEW_SCAN;
		}
		else
		{
	    EXTI_ENABLE;	//send finish enable_exti
		}
	}
}


/*
 * ��������fputc
 * ����  ���ض���c�⺯��printf
 * ����  ����
 * ���  ����
 * ����  ����printf����
 */
int fputc(int ch, FILE *f)
{
	/* ��Printf���ݷ������� */
	
	#if USE_VIRTUAL_COM
	  static unsigned char tmp[1];
	  tmp[0] = ch;
	  USB_TxWrite(tmp, 1);
	#else
	  USART_SendData(USART1, (unsigned char) ch);
	  while (!(USART1->SR & USART_FLAG_TXE));
	#endif
	
	return (ch);
}

