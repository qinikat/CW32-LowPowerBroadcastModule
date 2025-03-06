/**
 * @file cw32l083_lcd.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-01-20
 *
 * @copyright Copyright (c) 2022
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "cw32l083_lcd.h"
#include "cw32l083_rcc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  LCD 去初始化，使LCD的寄存器恢复为默认复位值
 * @param  None
 * @retval None
 */
void LCD_DeInit(void)
{
    /* Enable LCD reset state */
    CW_SYSCTRL->APBRST1_f.LCD = 0;
    /* Release LCD from reset state */
    CW_SYSCTRL->APBRST1_f.LCD = 1;

}

/**
 * @brief 初始化LCD的参数
 *
 * @param LCD_InitStruct LCD初始化结构体的指针
 */
void LCD_Init(LCD_InitTypeDef* LCD_InitStruct)
{
    /* Check function parameters */
    assert_param(IS_LCD_CLOCK(LCD_InitStruct->LCD_ClockSource));
    assert_param(IS_LCD_DUTY(LCD_InitStruct->LCD_Duty));
    assert_param(IS_LCD_BIAS(LCD_InitStruct->LCD_Bias));
    assert_param(IS_LCD_VOLTAGE_SOURCE(LCD_InitStruct->LCD_VoltageSource));
    assert_param(IS_LCD_SCAN_FREQ(LCD_InitStruct->LCD_ScanFreq));

    CW_LCD->CR0 = (CW_LCD->CR0 & (~(LCD_CR0_INRS_Msk | LCD_CR0_DUTY_Msk | LCD_CR0_BIAS_Msk))) | \
                  (LCD_InitStruct->LCD_VoltageSource | LCD_InitStruct->LCD_Duty | LCD_InitStruct->LCD_Bias);
    CW_LCD->CR1 = (CW_LCD->CR1 & (~(LCD_CR1_LCDFS_Msk | LCD_CR1_CLKCS_Msk))) | \
                  (LCD_InitStruct->LCD_ScanFreq | LCD_InitStruct->LCD_ClockSource);
}


/**
 * @brief 使能或禁能LCD控制器
 *
 * @param NewState DISABLE ENABLE
 */
void LCD_Cmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    CW_LCD->CR0_f.EN = (uint32_t)NewState;
}

/**
 * @brief 调节LCD的显示对比度，仅当Bias电压来源为内部电阻分压时，有效。
 *
 * @param LCD_ContrastLevel LCD_Contrast_Level_0~LCD_Contrast_Level_15
 *                          LCD_Contrast_Level_0对比度最大
 *                          LCD_Contrast_Level_15对比度最小
 */
void LCD_ContrastConfig(uint32_t LCD_ContrastLevel)
{
    assert_param(IS_LCD_CONTRAST(LCD_ContrastLevel));

    CW_LCD->CR0_f.CONTRAST = LCD_ContrastLevel;
}

/**
 * @brief 调节LCD的驱动电压，仅当LCD Bias电压为内部电路提供时有效
 *
 * @param LCD_INRS_Level LCD_INRS_LEVEL_0~LCD_INRS_LEVEL_3
 *                       LCD_INRS_LEVEL_0 驱动能力最弱，功耗最低
 *                       LCD_INRS_LEVEL_3 驱动能力最强，功耗最高
 */
void LCD_DriveVoltageConfig(uint32_t LCD_INRS_Level)
{
    assert_param(IS_LCD_INRS(LCD_INRS_Level));

    CW_LCD->CR0_f.INRS = LCD_INRS_Level;
}

/**
 * @brief Bias为外部电容电路提供时，电荷泵的打开与关闭控制
 *
 * @param NewState ENABLE DISABLE
 */
void LCD_ChargeBumpCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    CW_LCD->CR0_f.BUMP = (uint32_t)NewState;
}

/**
 * @brief 设置电荷泵的工作频率
 *
 * @param LCD_ChargeClock LCD_CHARGER_CLOCK_2KHZ LCD_CHARGER_CLOCK_4KHZ
 *                        LCD_CHARGER_CLOCK_8KHZ LCD_CHARGER_CLOCK_16KHZ
 */
void LCD_ChargeClockConfig(uint32_t LCD_ChargeClock)
{
    assert_param(IS_LCD_CHARGE_CLOCK(LCD_ChargeClock));

    CW_LCD->CR0_f.CPCLK = LCD_ChargeClock;
}

/**
 * @brief 打开或关闭LCD中断触发DMA的功能
 *
 * @param NewState ENABLE DISABLE
 */
void LCD_DMACmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    CW_LCD->CR1_f.DMAEN = (uint32_t)NewState;
}

/**
 * @brief 修改LCD的扫描频率
 *
 * @param LCD_ScanFreq LCD_SCAN_FREQ_64HZ LCD_SCAN_FREQ_128HZ LCD_SCAN_FREQ_256HZ LCD_SCAN_FREQ_512HZ
 */
void LCD_ScanFreqConfig(uint32_t LCD_ScanFreq)
{
    assert_param(IS_LCD_SCAN_FREQ(LCD_ScanFreq));

    REGBITS_MODIFY(CW_LCD->CR1, LCD_CR1_LCDFS_Msk, LCD_ScanFreq);
}

/**
 * @brief 修改LCD的驱动时钟源
 *
 * @param LCD_ClockSource LCD_CLOCK_SOURCE_LSI 内部低速RC时钟
 *                        LCD_CLOCK_SOURCE_LSE 外部低速晶振
 */
void LCD_ClockSourceConfig(uint32_t LCD_ClockSource)
{
    assert_param(IS_LCD_CLOCK(LCD_ClockSource));

    REGBITS_MODIFY(CW_LCD->CR1, LCD_CR1_CLKCS_Msk, LCD_ClockSource);
}


/**
* @brief LCD控制器中断和闪屏间隔配置，具体间隔时间的计算请参阅用户手册
*
* @param LCD_IntDuration 取值范围0~63
*/
void LCD_IntOrBlinkDurationConfig(uint32_t LCD_Duration)
{
    /* Check the parameters */
    assert_param(IS_LCD_INT_DURATION(LCD_Duration));

    CW_LCD->CR1_f.BLINKCNT = LCD_Duration;
}

/**
 * @brief LCD控制器的闪屏功能控制
 *
 * @param NewState ENABLE DISABLE
 */
void LCD_BlinkCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    CW_LCD->CR1_f.BLINKEN = (uint32_t)NewState;
}

/**
 * @brief 向LCD的RAM写数据，写1的bit COM和SEG的交点显示，写0的bit COM和SEG的交点消隐
 *
 * @param LCD_RAMRegister LCD_RAMRegister_0~LCD_RAMRegister_13
 * @param LCD_Data
 */
void LCD_Write(uint32_t LCD_RAMRegister, uint32_t LCD_Data)
{
    /* Check the parameters */
    assert_param(IS_LCD_RAM_REGISTER(LCD_RAMRegister));

    /* Copy data bytes to RAM register */

    CW_LCD->RAM[LCD_RAMRegister] = (uint32_t)LCD_Data;
}


/**
 * @brief 打开或关闭LCD的中断
 *
 * @param NewState ENABLE DISABLE
 */
void LCD_ITCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    CW_LCD->CR1_f.IE = (uint32_t)NewState;

}

/**
 * @brief 获取LCD的中断标志状态
 *
 * @return ITStatus
 */
ITStatus LCD_GetITStatus(void)
{
    return (CW_LCD->CR1 & LCD_CR1_INTF_Msk) ? SET : RESET;
}

/**
 * @brief 清除LCD的中断标志状态
 *
 */
void LCD_ClearITPendingBit(void)
{
    CW_LCD->INTCLR_f.INTF = 0;
}

/**
 * @brief 配置COM的输出使能与禁能
 *
 * @param LCD_COM_x LCD_COM0~LCD_COM7 可通过与运算同时操作多个COM
 */
void LCD_COMConfig(uint32_t LCD_COM_x, FunctionalState NewState)
{
    assert_param(IS_LCD_COM(LCD_COM_x));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState)
    {
        CW_LCD->PINEN1 |= ((LCD_COM_x) & 0xF0000000);
        CW_LCD->PINEN2 |= ((LCD_COM_x) & 0x0F000000);
    }
    else
    {
        CW_LCD->PINEN1 &= ~((LCD_COM_x) & 0xF0000000);
        CW_LCD->PINEN2 &= ~((LCD_COM_x) & 0x0F000000);
    }
}

/**
 * @brief 配置SEG0~23的输出使能
 *
 * @param LCD_SEG_x  LCD_SEG0~LCD_SEG23 可通过与运算同时操作多个SEG
 * @param NewState ENABLE DISABLE
 */
void LCD_SEG0to23Config(uint32_t LCD_SEG_x, FunctionalState NewState)
{
    assert_param(IS_LCD_SEGX(LCD_SEG_x));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState)
    {
        CW_LCD->PINEN1 |= ((LCD_SEG_x) & 0x00FFFFFF);
    }
    else
    {
        CW_LCD->PINEN1 &= ~((LCD_SEG_x) & 0x00FFFFFF);
    }
}

/**
 * @brief 配置SEG32~55的输出使能
 *
 * @param LCD_SEG_y LCD_SEG32~LCD_SEG55 可通过与运算同时操作多个SEG
 * @param NewState ENABLE DISABLE
 */
void LCD_SEG32to55Config(uint32_t LCD_SEG_y, FunctionalState NewState)
{
    assert_param(IS_LCD_SEGY(LCD_SEG_y));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    if (NewState)
    {
        CW_LCD->PINEN2 |= ((LCD_SEG_y) & 0x00FFFFFF);
    }
    else
    {
        CW_LCD->PINEN2 &= ~((LCD_SEG_y) & 0x00FFFFFF);
    }
}


/**
 * @brief 配置SEG24~27的输出使能
 *
 * @param LCD_SEG_v  LCD_SEG24~LCD_SEG27 可通过与运算同时操作多个SEG
 * @param NewState ENABLE DISABLE
 */
void LCD_SEG24to27Config(uint32_t LCD_SEG_v, FunctionalState NewState)
{
    assert_param(IS_LCD_SEGV(LCD_SEG_v));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState)
    {
        CW_LCD->PINEN2_f.MUX = 1;
        CW_LCD->PINEN1 |= ((LCD_SEG_v) & 0x0F000000);
    }
    else
    {
        CW_LCD->PINEN1 &= ~((LCD_SEG_v) & 0x0F000000);
        if ((CW_LCD->PINEN1 & 0x0F000000) == 0x00)
        {
            CW_LCD->PINEN2_f.MUX = 0;
        }
    }
}


/************************ END OF FILE************************/
