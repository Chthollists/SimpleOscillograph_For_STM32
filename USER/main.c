/************************SimpleOscillograph For STM32******************************
 * �ļ���  ��main.c
 * ����    ��SimpleOscillograph For STM32      ʾ����STM32����
 * ƽ̨    ��STM32f103C8���İ�
 * ��汾  ��ST3.5.0
 *
 * ����    ����˧
 * ����    ��shuai132@outlook.com
 
 * ����    ��2015/10/24
 * �޸�    ��2016/01/26
 * �޸�    ��2016/01/10
 * �޸�    ��2016/04/01  ����USB
 * �޸�    ��2016/04/14  �����Զ�������Ϊһֱ���� ���ӱ��ش���
 * �޸�    ��2016/05/09  �������� ��ӵ��δ�����stm32f10x_it.c��
 * �޸�    ��2016/07/20  ���Ĳ��ֺ����� & ��������
 *
 * ע��    ���ж����ȼ�������0-0.0 > ��ʱ��2-1.0 > �ⲿ�ж�PB01-2.0
**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"
#include "exti.h"
#include "led.h"
#include "timer.h"
#include "mydef.h"
#include "HW_Config.h"
#include "usb_mem.h"

/* Private function prototypes -----------------------------------------------*/
void scan(void);
void send(void);
void on_send_finish(void);


//interval_time ����ʱ������1/FS
volatile u32 FS         = 100000u;  //��ʼ��ʾ100KHz
//abort flag
volatile u8 is_abort    = 0;        //1���������η���
// scan mode
volatile u8 scan_mode   = 0;        //0��ʵʱ����������ģʽ�� 1���ɼ���ͬʱ�������� �ʺϵ�Ƶ�ź�
volatile u16 value_i    = 0;        //��¼���ڲɼ������εڼ�����  ��Χ��0��(number_of_data-1)
volatile u8 SCAN_IS_OK  = 0;        //����Ƿ�Ϊ�ɼ����
// TriggerMode
volatile u8 trigger_mode= 0;        //0����������Զ����� 1 2 3�ֱ�Ϊ���� �½� ���ش���
volatile EXTITrigger_TypeDef TriggerMode;  //���ڼ�¼�ж�ģʽ(����ģʽ)


// ������ �Ӵ�����ʼ���Բ���Ƶ��FS�洢num_of_data���㣬֮��һ����������
volatile u16 C1_value[num_of_data];
volatile u16 C2_value[num_of_data];
volatile u16 C3_value[num_of_data];

// ADCת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern volatile uint16_t ADC_ConvertedValue[3];


/*
 * ������
 */
int main(void)
{ 
  #if USE_USB_VIRTUAL_COM
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
  TIM_NVIC_Configuration();
  TIM_Configuration(FS);  //��ʼFS(100KHz)
  
  /* USART1 config */
  USART1_Config();
  
  /* enable adc1 and config adc1 to dma mode */
  ADC1_Init();
  
  /* exti line config */
  EXTI_PB01_Init();
  
  //��ʼ���رմ����ж�
  EXTI_DISABLE;
  
  PRINTF_LABVIEW("\r\n------ SimpleOscillograph For STM32 ------\r\n");
  
  //����ADC��Ϣ������λ������adԭʼֵ
  PRINTF_LABVIEW("VC:%f\r\n", VCC_V);  //VCC
  PRINTF_LABVIEW("AD:%d\r\n", ADC_B);  //adcλ��
  
  //��ʼһ�βɼ������ڵ���
  START_NEW_SCAN;
  while (1)
  {
    if (scan_mode == 1)
    {
      //ʵʱ����ʵʱ��ʾ
      PRINTF_LABVIEW("C1:%d\r\nC2:%d\r\nC3:%d\r\n", ADC_ConvertedValue[0], ADC_ConvertedValue[1], ADC_ConvertedValue[2]);
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
 * ���ݷ��ͺ���
 * ֱ�ӷ���ԭʼֵ��������λ������
 * ������Ϻ������ⲿ�ж��������ٴδ���
 */
void send(void)
{
  u16 i = 0;
  
  //����ADC��Ϣ������λ������adԭʼֵ
  PRINTF_LABVIEW("VC:%f\r\n", VCC_V);       //VCC
  PRINTF_LABVIEW("AD:%d\r\n", ADC_B);       //adcλ��
  PRINTF_LABVIEW("FS:%d\r\n", FS);          //������
  PRINTF_LABVIEW("NM:%d\r\n", num_of_data); //���ݸ���(���ڻ�ͼ��
  
  is_abort = 0;

  do
  {
#if USE_CHANNEL_C1
        PRINTF_LABVIEW("C1:%d\r\n", C1_value[i]);
#endif

#if USE_CHANNEL_C2
        PRINTF_LABVIEW("C2:%d\r\n", C2_value[i]);
#endif

#if USE_CHANNEL_C3
        PRINTF_LABVIEW("C3:%d\r\n", C3_value[i]);
#endif
  }
  while((++i < num_of_data) && (is_abort == 0) && (scan_mode == 0));
  
  PRINTF_LABVIEW("FN:OK\r\n");
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
      //�Զ���ʼ�µĲɼ�
      START_NEW_SCAN;
    }
    else if(trigger_mode == 4)
    {
      EXTI_DISABLE;
    }
    else
    {
      EXTI_ENABLE;
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
  
  #if USE_USB_VIRTUAL_COM
    static unsigned char tmp[1];
    tmp[0] = ch;
    USB_TxWrite(tmp, 1);
  #else
    USART_SendData(USART1, (unsigned char) ch);
    while (!(USART1->SR & USART_FLAG_TXE));
  #endif
  
  return (ch);
}

