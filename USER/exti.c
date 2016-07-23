/************************SimpleOscillograph For STM32******************************
 * �ļ���  ��exti.c
 * ����    ��SimpleOscillograph For STM32      
 * ƽ̨    ��STM32���İ�
 * ��汾  ��ST3.5.0
 *
 * ����    ����˧
 * ����    ��2015-10-24
 * �޸�    ��2016/04/14  ���ӱ��ش���
 * �޸�    ��2016/07/20  ����EXTI_Control()ʹ��NVIC_EnableIRQ() & ��������
 *
 * ע��    ���ж����ȼ�������0-0.0 > ��ʱ��2-1.0 > �ⲿ�ж�PB01-2.0
**********************************************************************************/

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


void EXTI_PB01_Init(void)
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
void EXTI_Config(EXTITrigger_TypeDef TM)
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
 * ��������EXTI_Control
 * ����  ���򿪻�ر��ж�
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void EXTI_Control(FunctionalState state)
{
   if(state == ENABLE)
     NVIC_EnableIRQ(EXTI1_IRQn);    //������line1�ϵ��ж�
   
   else
     NVIC_DisableIRQ(EXTI1_IRQn);   //����line1�ϵ��ж�  
}
