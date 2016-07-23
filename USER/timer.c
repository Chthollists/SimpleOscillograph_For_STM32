/************************SimpleOscillograph For STM32******************************
 * �ļ���  ��timer.c
 * ����    ��SimpleOscillograph For STM32      
 * ƽ̨    ��STM32���İ�
 * ��汾  ��ST3.5.0
 *
 * ����    ����˧
 * ����    ��2015/10/24
 * �޸�    ��2016/07/20  ��������
**********************************************************************************/

#include "timer.h"

/*
 * ��������TIM_NVIC_Configuration
 * ����  ��TIM2�ж����ȼ�����
 * ����  ����
 * ���  ����	
 */
void TIM_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 *1us�ж�һ��
 *TIM_Period--1000   TIM_Prescaler--71 -->�ж�����Ϊ1ms
**/
void TIM_Configuration(u32 frequency)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period=(uint16_t)(1000000ul/frequency - 1);   /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    /* �ۼ�TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				            /* ʱ��Ԥ��Ƶ�� 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;                   /* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;               /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);                                   /* �������жϱ�־ */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, DISABLE);                                                 /* �ر�ʱ�� */
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
