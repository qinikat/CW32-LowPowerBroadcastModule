
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "log.h"
#include "cw32l083_gpio.h"
#include "cw32l083_rcc.h"
#include "cw32l083_uart.h"
#include "system_cw32l083.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SerialInit(uint32_t BaudRate);
static void SerialSend(uint8_t Data);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint8_t const pow2_table[] = {0, 1, 2, 3, 4, 5, 6, 7};

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

void LogInit(void)
{
    SerialInit(LOG_SERIAL_BPS);
}

static void SerialInit(uint32_t BaudRate)
{
    uint32_t PCLK_Freq;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    UART_InitTypeDef UART_InitStructure = {0};

    PCLK_Freq = SystemCoreClock >> pow2_table[CW_SYSCTRL->CR0_f.HCLKPRS];
    PCLK_Freq >>= pow2_table[CW_SYSCTRL->CR0_f.PCLKPRS];

    // 调试串口使用UART4
    //  PA4->TX
    //  PB5<-RX
    // 时钟使能yyyyy
    __RCC_GPIOB_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();
    //__RCC_UART5_CLK_ENABLE();
    __RCC_UART4_CLK_ENABLE();

    // 先设置UART TX RX 复用，后设置GPIO的属性，避免口线上出现毛刺
    // PB08_AFx_UART5TXD();
    // PB09_AFx_UART5RXD();
    PA04_AFx_UART4TXD();
    PB05_AFx_UART4RXD();

    GPIO_InitStructure.Pins = GPIO_PIN_4;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    // GPIO_InitStructure.Pins = GPIO_PIN_8;
    // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    // GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    // GPIO_InitStructure.Pins = GPIO_PIN_9;
    // GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    // GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    UART_InitStructure.UART_BaudRate = BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = PCLK_Freq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    // UART_Init(CW_UART5, &UART_InitStructure);
    UART_Init(CW_UART4, &UART_InitStructure);

    // 使能UARTx RC中断
    UART_ITConfig(CW_UART4, UART_IT_RC, ENABLE);
}

static void SerialSend(uint8_t Data)
{

    UART_SendData_8bit(CW_UART4, Data);
    while (UART_GetFlagStatus(CW_UART4, UART_FLAG_TXE) == RESET)
        ;
}

int fputc(int ch, FILE *f)
{
    SerialSend(ch);
    return ch;
}

size_t __write(int handle, const unsigned char *buffer, size_t size)
{
    size_t nChars = 0;

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return 0;
    }

    for (/* Empty */; size != 0; --size)
    {
        SerialSend(*buffer++);
        ++nChars;
    }

    return nChars;
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
