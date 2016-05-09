/********************** ��ֲʱҪ�ر��ע�� *******************************
* ���ߣ���˧
* ���ڣ�2015/10/31
* �޸ģ�2016/01/10
* �޸ģ�2016/04/01  add USB
************************************************************************/
#ifndef __MYDEF_H
#define __MYDEF_H

#include <stdarg.h>

#define VCC_V 3.3      //float  ȡ����ѹ���ֵ
#define ADC_B 12      //int     ADCλ��

//�Ƿ�ʹ�����⴮��
#define USE_VIRTUAL_COM 0

//�ϴ�����
#define PRINTF_LABVIEW(format, ...)  printf(format, ##__VA_ARGS__)


#define num_of_data 1024

//�����ж�����
#define EXTI_ENABLE    EXTI_PB01_control(ENABLE)
//�����жϽ�ֹ
#define EXTI_DISABLE  EXTI_PB01_control(DISABLE)

//������ʱ��
#define START_TIMER            TIM_Cmd(TIM2, ENABLE)     //������ʱ��
//�رն�ʱ��
#define STOP_TIMER             TIM_Cmd(TIM2, DISABLE)

#define START_TIMER_NEW_SCAN   start_new_scan();

#define EXTI_CONFIG_Rising_Falling    EXTI_PB01_Config(EXTI_Trigger_Rising_Falling)
#define EXTI_CONFIG_Rising            EXTI_PB01_Config(EXTI_Trigger_Rising)
#define EXTI_CONFIG_Falling           EXTI_PB01_Config(EXTI_Trigger_Falling)


extern void start_new_scan(void);

#endif
