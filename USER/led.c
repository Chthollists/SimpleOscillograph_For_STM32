#include "led.h"

void LED_GPIO_Config(void)
{
  //����ṹ��
  GPIO_InitTypeDef GPIO_InitStruct;
  //����ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  //ѡ������
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
  //�������
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
  //������������
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  //��ʼ��gpioc
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  //�ر�led
  GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}
