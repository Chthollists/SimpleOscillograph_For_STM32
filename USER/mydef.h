/********************** ��ֲʱҪ�ر��ע�� *******************************
* ���ߣ���˧
* ���ڣ�2015/10/31
* �޸ģ�2016/01/10
* �޸ģ�2016/04/01  add USB
* �޸ģ�2016/07/20  ���Ĳ��ֺ����� & ��������
* �޸ģ�2016/07/23  ����ͨ�����غ��
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
#define USE_CHANNEL_C2                  1
#define USE_CHANNEL_C3                  1

//�����ж�����
#define EXTI_ENABLE                     EXTI_Control(ENABLE)
//�����жϽ�ֹ
#define EXTI_DISABLE                    EXTI_Control(DISABLE)

//������ʱ��
#define START_TIMER                     TIM_Cmd(TIM2, ENABLE)       //������ʱ��
//�رն�ʱ��
#define STOP_TIMER                      TIM_Cmd(TIM2, DISABLE)

#define START_NEW_SCAN                  start_new_scan();

#define EXTI_CONFIG_Rising_Falling      EXTI_Config(EXTI_Trigger_Rising_Falling)
#define EXTI_CONFIG_Rising              EXTI_Config(EXTI_Trigger_Rising)
#define EXTI_CONFIG_Falling             EXTI_Config(EXTI_Trigger_Falling)


extern void start_new_scan(void);

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

#endif
