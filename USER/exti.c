#include "exti.h"

/*
 * ��������NVIC_Configuration
 * ����  ������Ƕ�������жϿ�����NVIC
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����P[A|B|C|D|E]1Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void PB1_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  
  /* config the NVIC(PB1) */
  NVIC_Configuration();
  
  /* config the extiline(PB1) clock and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
  
  /* EXTI line(PB1) mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
  
  /* EXTI line gpio config(PB1) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������ģʽ
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

extern EXTITrigger_TypeDef TriggerMode;
//ע�⣺���ú󼴹ر����ж�
void EXTI_PB01_Config(EXTITrigger_TypeDef TM)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  
  /* EXTI line(PB1) mode config */
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = TM;          //�жϴ���ģʽ
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  TriggerMode = TM;  //����ȫ�ֱ���
}

/*
 * ��������EXTI_PB01_control
 * ����  ���򿪻�ر��ж�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void EXTI_PB01_control(FunctionalState state)
{
   if(state == ENABLE)
     EXTI->IMR|=1<<1;     //������line1�ϵ��ж�
   
   else
     EXTI->IMR&=~(1<<1);  //����line1�ϵ��ж�  
}

/*
//�뿴EXTI_Init()�ڲ�ʵ�֣��˴�����ʹ������Ч�ʸ���
void EXTI_PB01_control(FunctionalState state)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  
  // EXTI line(PB1) mode config  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = TriggerMode;
  EXTI_InitStructure.EXTI_LineCmd = state;
  EXTI_Init(&EXTI_InitStructure);
}
*/

