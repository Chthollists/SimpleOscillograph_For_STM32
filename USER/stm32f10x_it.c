/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include <string.h>
#include "stm32f10x_it.h"
#include "mydef.h"	//num_of_data
#include "Time.h"
#include "led.h"
#include "usart1.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern u32 interval_time;
extern u32 FS;
extern u8 is_abort;
extern u8 scan_mode;
extern volatile u8 trigger_mode;

/* Private function prototypes -----------------------------------------------*/
extern void EXTI_PB01_Config(EXTITrigger_TypeDef TM);
extern void EXTI_PB01_control(FunctionalState state);
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    //TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval : None
  */
extern volatile u16 value_i;
extern __IO uint16_t ADC_ConvertedValue[3];
extern __IO u16 C1_value[num_of_data];
extern __IO u16 C2_value[num_of_data];
extern __IO u16 C3_value[num_of_data];
extern void send(void);
extern volatile u8 SCAN_IS_OK;

void TIM2_IRQHandler(void)
{
    if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
    {
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
        
        C1_value[value_i] = ADC_ConvertedValue[0];	// ��ȡת����ADֵ
        C2_value[value_i] = ADC_ConvertedValue[1];	// ��ȡת����ADֵ
        C3_value[value_i] = ADC_ConvertedValue[2];	// ��ȡת����ADֵ
        
        if(value_i++ == num_of_data-1)
        {
            STOP_TIMER;
            value_i = 0;
            SCAN_IS_OK = 1;		//send
					  LED(OFF);
        }
    }
}

/* I/O���жϣ��ж���ΪPB01 */
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) //ȷ���Ƿ������EXTI Line�ж�
    {
        EXTI_DISABLE;														//��ֹ������������ɺ����abort(��scan_mode == 0)���ٴδ�
        EXTI_ClearITPendingBit(EXTI_Line1);     //����жϱ�־λ (ע��λ�ã�
        
        //ʹ�ö�ʱ����ȷ������֧����ͨ��
        START_TIMER_NEW_SCAN;
    }
}

void start_new_scan(void)
{
	is_abort = 1;
	value_i = 0;
	C1_value[0] = ADC_ConvertedValue[0];	// �Ȳɼ���һ������
  C2_value[0] = ADC_ConvertedValue[1];	// �Ȳɼ���һ������
  C3_value[0] = ADC_ConvertedValue[2];	// �Ȳɼ���һ������
  value_i++;
	
	START_TIMER;
	LED(ON);
}

/*
 *���ܣ������жϽ�����λ���������
 *˵����
 */
void process(u8 * rx);
void USART1_IRQHandler(void)
{
    static u8 i, UART1_BUFF[10];
    static u8 c;	//Ϊ���ٶ�
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    { 	
        c = USART1->DR;
        UART1_BUFF[i++] = c;
			
				//���������ֹ�����ո��Զ���ģ���
        if(c == ' ')
        {
            UART1_BUFF[i] = '\0';
            i = 0;
            
            process(UART1_BUFF);
        }
    }
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

/*******************һЩ��������������������by ��˧******************************/
/*
 *���ܣ����ô��ڽ��յ��ڱ�����ֵ
 *˵����
 */
void process(u8 * rx)
{
    u8 num;
    //int nunber = 0;
    char t[3];
    t[0] = rx[0];
    t[1] = rx[1];
    t[2] = '\0';	//��λ��ԭ��Ϊð��
    
    num = rx[3] - 48;	//ð�ź����һ��ֵ
    
    if(!strcmp(t, "FS"))
    {
        STOP_TIMER;
        //���ò�����
        switch (num)
        {
        case 0 : FS = 100;  	TIM2_Configuration(FS);break;//��СΪ100Hz
        case 1 : FS = 1000; 	TIM2_Configuration(FS);break;
        case 2 : FS = 10000;  TIM2_Configuration(FS);break;
        case 3 : FS = 100000; TIM2_Configuration(FS);break;
        case 4 : FS = 250000; TIM2_Configuration(FS);break;
				case 5 : FS = 500000; TIM2_Configuration(FS);break;
        case 6 : FS = 500000; TIM2_Configuration(FS);break;
        }
				
				START_TIMER_NEW_SCAN	
    }
    else if(!strcmp(t, "TM"))
    {
			  //������ʽ
			  trigger_mode = num;
        
        switch (num)
        {
					case 0 : START_TIMER_NEW_SCAN;        break;
					case 1 : EXTI_CONFIG_Rising;					break;	//up
					case 2 : EXTI_CONFIG_Falling; 				break;	//dowm
					case 3 : EXTI_CONFIG_Rising_Falling; 	break;	//auto
        }
				if(scan_mode == 0)
				{
					//EXTI_ENABLE;
					is_abort = 1;
				}
    }
    else if(!strcmp(t, "SM"))
    {
        //scan_mode
        scan_mode = num;
        if (scan_mode == 1)
        {
            //ʵʱ�ɼ���ʾģʽ
            EXTI_DISABLE;
            is_abort = 1;
            value_i = 0;
					  PRINTF_LABVIEW("FS:0\r\n");
        }
        else
        {
						//�ɼ�һ��(������ɻ��Զ����ж�)
						START_TIMER_NEW_SCAN;		//������ʱ���ɼ�����
        }
    }
    else if(!strcmp(t, "AB"))
    {
			  if (scan_mode == 0)
				{
					//�������β��������¿�ʼ�ɼ�
					STOP_TIMER;
					is_abort = 1;
          value_i = 0;
				}
        
    }
}
