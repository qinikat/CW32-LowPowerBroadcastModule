/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    interrupts.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 CW.
 * All rights reserved.</center></h2>
 *
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "../inc/interrupts_cw32l083.h"
#include "message.h"
#include "rtc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

uint16_t BTIM1_Interrupt_times = 0;

extern uint8_t Sleep_Status;
extern uint8_t Message_Status;
extern uint8_t Flage_Receive_UART3;
extern uint8_t Receive_Counted;
extern uint8_t MessageReceiveBuffer[MESSAGE_RECEIVE_BUFFER_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern void RTC_IRQHandlerCallBack(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0P Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    /* USER CODE BEGIN NonMaskableInt_IRQn */

    /* USER CODE END NonMaskableInt_IRQn */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    /* USER CODE BEGIN HardFault_IRQn */

    /* USER CODE END HardFault_IRQn */
    while (1)
    {
        /* USER CODE BEGIN W1_HardFault_IRQn */

        /* USER CODE END W1_HardFault_IRQn */
    }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
    /* USER CODE BEGIN SVCall_IRQn */

    /* USER CODE END SVCall_IRQn */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
    /* USER CODE BEGIN PendSV_IRQn */

    /* USER CODE END PendSV_IRQn */
}

/******************************************************************************/
/* CW030 Peripheral Interrupt Handlers                                        */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_ps030.s).                        */
/******************************************************************************/

/**
 * @brief This funcation handles WDT
 */
void WDT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles LVD
 */
void LVD_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles RTC
 */
void RTC_IRQHandler(void)
{
    /* USER CODE BEGIN */
    RTC_IRQHandlerCallBack();

    /* USER CODE END */
}

/**
 * @brief This funcation handles FLASHRAM
 */
void FLASHRAM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles RCC
 */
void SYSCTRL_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOA
 */
void GPIOA_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOB
 */
void GPIOB_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOC
 */
void GPIOC_GPIOD_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOF
 */
void GPIOE_GPIOF_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles DMACH1
 */
void DMACH1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles DMACH23
 */
void DMACH23_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles DMA34
 */
void DMACH45_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles ADC
 */
void ADC_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles ATIM
 */
void ATIM_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles VC1
 */
void VC1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles VC2
 */
void VC2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM1
 */
void GTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM2
 */
void GTIM2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM3
 */
void GTIM3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles GTIM4
 */
void GTIM4_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM1
 */
void BTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */
    // 0.1s中断一次,5秒中断50次后并打印时间

    if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
    {
        BTIM1_Interrupt_times++;
        if (BTIM1_Interrupt_times == 50)
        {
            // printf("sleep_status = %d\r\n", Sleep_Status);
            // printf("Message_Status = %d\r\n", Message_Status);
            BTIM1_Interrupt_times = 0;
            ShowTime();
        }
        BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
    }

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM2
 */
void BTIM2_LPTIM1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles BTIM3
 */
void BTIM3_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles I2C1
 */
void I2C1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles I2C2
 */
void I2C2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SPI1
 */
void SPI1_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles SPI2
 */
void SPI2_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART1
 */
void UART1_UART4_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART2
 */
void UART2_UART5_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles UART3
 */
void UART3_UART6_IRQHandler(void)
{
    /* USER CODE BEGIN */
    if (Receive_Counted == 0)
    {
        // 标志位变更
        Flage_Receive_UART3 = 1;
    }
    if (UART_GetITStatus(CW_UART3, UART_IT_RC) != RESET)
    {
        // UART_SendData_8bit(CW_UART3, UART_ReceiveData_8bit(CW_UART3));
        MessageReceiveBuffer[Receive_Counted++] = UART_ReceiveData_8bit(CW_UART3);
        UART_ClearITPendingBit(CW_UART3, UART_IT_RC);
    }
    /* USER CODE END */
}

/**
 * @brief This funcation handles AUTOTRIM and LCD
 */
void AUTOTRIM_LCD_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/**
 * @brief This funcation handles FAULT
 */
void FAULT_IRQHandler(void)
{
    /* USER CODE BEGIN */

    /* USER CODE END */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT CW *****END OF FILE****/
