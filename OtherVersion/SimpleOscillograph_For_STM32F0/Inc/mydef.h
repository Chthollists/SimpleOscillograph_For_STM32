/********************** ��ֲʱҪ�ر��ע�� *******************************
* ���ߣ���˧
* ���ڣ�2015/10/31
* �޸ģ�2016/01/10
* �޸ģ�2016/04/01  add USB
* �޸ģ�2016/07/20  ���Ĳ��ֺ����� & ��������
* ���ڣ�2016/07/23  ��ֲ��STM32F0  ʹ��HAL����ӷ���STM32ϵ�е���ֲ
************************************************************************/
#ifndef __MYDEF_H
#define __MYDEF_H

#include <stdarg.h>
#include <stdint.h>

#define VCC_V                           3.3                         //float  ȡ����ѹ���ֵ
#define ADC_B                           12                          //int     ADCλ��

//�Ƿ�ʹ�����⴮��
#define USE_USB_VIRTUAL_COM             0                           //0 ʹ��Ӳ������  1 ʹ��USB����Ĵ���

//�ϴ�����
#define PRINTF_LABVIEW(format, ...)     printf(format, ##__VA_ARGS__)

#define num_of_data 1024

#define USE_CHANNEL_C1                  1
#define USE_CHANNEL_C2                  0
#define USE_CHANNEL_C3                  0

//�����ж�����
#define EXTI_ENABLE                     EXTI_Control(ENABLE)
//�����жϽ�ֹ
#define EXTI_DISABLE                    EXTI_Control(DISABLE)

//������ʱ��
#define START_TIMER                     HAL_TIM_Base_Start_IT(&htim1)  //������ʱ��
//�رն�ʱ��
#define STOP_TIMER                      HAL_TIM_Base_Stop_IT(&htim1)

#define START_TIMER_NEW_SCAN            start_new_scan();

#define EXTI_CONFIG_Rising_Falling      EXTI_Config(GPIO_MODE_IT_RISING_FALLING)
#define EXTI_CONFIG_Rising              EXTI_Config(GPIO_MODE_IT_RISING)
#define EXTI_CONFIG_Falling             EXTI_Config(GPIO_MODE_IT_FALLING)


extern void start_new_scan(void);

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef uint32_t EXTITrigger_TypeDef;

#endif
