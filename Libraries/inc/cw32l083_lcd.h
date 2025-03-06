/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CW32L083_LCD_H
#define __CW32L083_LCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "base_types.h"
#include "cw32l083.h"


/* Exported types ------------------------------------------------------------*/

/**
 * @brief  LCD Init structure definition
 */

typedef struct
{
    uint32_t LCD_ClockSource;
    uint32_t LCD_ScanFreq;
    uint32_t LCD_Duty;
    uint32_t LCD_Bias;
    uint32_t LCD_VoltageSource;
} LCD_InitTypeDef;


/* Exported constants --------------------------------------------------------*/
/******** LCD CR0 ********/

/**
 * @brief 内部分压时对比度调节
 *
 */
#define LCD_Contrast_Level_0               ((uint32_t)0x00000000) /*!< 对比度最大 */
#define LCD_Contrast_Level_1               ((uint32_t)0x00000001) /*!<  */
#define LCD_Contrast_Level_2               ((uint32_t)0x00000002) /*!<    */
#define LCD_Contrast_Level_3               ((uint32_t)0x00000003) /*!<    */
#define LCD_Contrast_Level_4               ((uint32_t)0x00000004) /*!<    */
#define LCD_Contrast_Level_5               ((uint32_t)0x00000005) /*!<    */
#define LCD_Contrast_Level_6               ((uint32_t)0x00000006) /*!<    */
#define LCD_Contrast_Level_7               ((uint32_t)0x00000007)
#define LCD_Contrast_Level_8               ((uint32_t)0x00000008)
#define LCD_Contrast_Level_9               ((uint32_t)0x00000009)
#define LCD_Contrast_Level_10              ((uint32_t)0x0000000A)
#define LCD_Contrast_Level_11              ((uint32_t)0x0000000B)
#define LCD_Contrast_Level_12              ((uint32_t)0x0000000C)
#define LCD_Contrast_Level_13              ((uint32_t)0x0000000D)
#define LCD_Contrast_Level_14              ((uint32_t)0x0000000E)
#define LCD_Contrast_Level_15              ((uint32_t)0x0000000F) /*!< 对比度最小 */

#define IS_LCD_CONTRAST(CONTRAST)          ((CONTRAST) < 0x00000010)

/**
 * @brief 内部分压时功耗调节
 *
 */
#define LCD_INRS_LEVEL_0                   (((uint32_t)0x00000004))   /*!< 功耗最小 */
#define LCD_INRS_LEVEL_1                   (((uint32_t)0x00000002))   /*!< 功耗较小 */
#define LCD_INRS_LEVEL_2                   (((uint32_t)0x00000001))   /*!< 功耗较大 */
#define LCD_INRS_LEVEL_3                   (((uint32_t)0x00000007))   /*!< 功耗最大 */
#define IS_LCD_INRS(INRS)                  (((INRS) == LCD_INRS_LEVEL_0) || \
                                           ((INRS) == LCD_INRS_LEVEL_1) || \
                                           ((INRS) == LCD_INRS_LEVEL_2) || \
                                           ((INRS) == LCD_INRS_LEVEL_3))
/**
* @brief LCD_Voltage_Source
*
*/

#define LCD_VoltageSource_Internal         (((uint32_t)0x00000001) << 9) /*!< Internal voltage source for the LCD */
#define LCD_VoltageSource_External         ((uint32_t)0x00000000)    /*!< External voltage source for the LCD */

#define IS_LCD_VOLTAGE_SOURCE(SOURCE)      (((SOURCE) == LCD_VoltageSource_Internal) || \
                                           ((SOURCE) == LCD_VoltageSource_External))

/**
 * @brief LCD_Duty
 *
 */
#define LCD_Duty_Static                    ((uint32_t)0x00000000)        /*!< Static duty */
#define LCD_Duty_1_2                       (((uint32_t)0x00000001) << 6) /*!< 1/2 duty    */
#define LCD_Duty_1_3                       (((uint32_t)0x00000002) << 6) /*!< 1/3 duty    */
#define LCD_Duty_1_4                       (((uint32_t)0x00000003) << 6) /*!< 1/4 duty    */
#define LCD_Duty_1_6                       (((uint32_t)0x00000005) << 6) /*!< 1/6 duty    */
#define LCD_Duty_1_8                       (((uint32_t)0x00000007) << 6) /*!< 1/8 duty    */

#define IS_LCD_DUTY(DUTY)                  (((DUTY) == LCD_Duty_Static) || \
                                           ((DUTY) == LCD_Duty_1_2) || \
                                           ((DUTY) == LCD_Duty_1_3) || \
                                           ((DUTY) == LCD_Duty_1_4) || \
                                           ((DUTY) == LCD_Duty_1_6) || \
                                           ((DUTY) == LCD_Duty_1_8))

/**
 * @brief LCD_Bias
 *
 */
#define LCD_Bias_1_2                       ((uint32_t)0x00000010)         /*!< 1/2 Bias */
#define LCD_Bias_1_3                       ((uint32_t)0x00000000)         /*!< 1/3 Bias */

#define IS_LCD_BIAS(BIAS)                  (((BIAS) == LCD_Bias_1_2) || \
                                           ((BIAS) == LCD_Bias_1_3))

#define LCD_BUMP_ENABLE                    (((uint32_t)0x00000001) << 3)
#define LCD_BUMP_DISABLE                   ((uint32_t)0x00000000)

#define LCD_CHARGER_CLOCK_2KHZ             ((uint32_t)0x00000000)
#define LCD_CHARGER_CLOCK_4KHZ             (((uint32_t)0x00000001))
#define LCD_CHARGER_CLOCK_8KHZ             (((uint32_t)0x00000002))
#define LCD_CHARGER_CLOCK_16KHZ            (((uint32_t)0x00000003))
#define IS_LCD_CHARGE_CLOCK(CLOCK)         (((CLOCK) == LCD_CHARGER_CLOCK_2KHZ) || \
                                           ((CLOCK) == LCD_CHARGER_CLOCK_4KHZ) || \
                                           ((CLOCK) == LCD_CHARGER_CLOCK_8KHZ) || \
                                           ((CLOCK) == LCD_CHARGER_CLOCK_16KHZ) )

/******** LCD CR1 ********/

#define LCD_SCAN_FREQ_64HZ                 (((uint32_t)0x00000003) << 8)
#define LCD_SCAN_FREQ_128HZ                ((uint32_t)0x00000000)
#define LCD_SCAN_FREQ_256HZ                (((uint32_t)0x00000001) << 8)
#define LCD_SCAN_FREQ_512HZ                (((uint32_t)0x00000002) << 8)
#define IS_LCD_SCAN_FREQ(FREQ)             (((FREQ) == LCD_SCAN_FREQ_64HZ) || \
                                           ((FREQ) == LCD_SCAN_FREQ_128HZ) || \
                                           ((FREQ) == LCD_SCAN_FREQ_256HZ) || \
                                           ((FREQ) == LCD_SCAN_FREQ_512HZ))

#define LCD_CLOCK_SOURCE_LSI               ((uint32_t)0x00000000)
#define LCD_CLOCK_SOURCE_LSE               (((uint32_t)0x00000001) << 7)
#define IS_LCD_CLOCK(CLOCK)                (((CLOCK) == LCD_CLOCK_SOURCE_LSI) || \
                                           ((CLOCK) == LCD_CLOCK_SOURCE_LSE))


/** @defgroup LCD_BlinkMode
* @{
*/

#define LCD_BlinkMode_Off                  ((uint32_t)0x00000000)        /*!< Blink disabled            */
#define LCD_BlinkMode_On                   (((uint32_t)0x00000001) << 6) /*!< Blink disabled            */

#define IS_LCD_INT_DURATION(DURATION)      ((DURATION) < 64)


#define LCD_RAMRegister_0                  ((uint32_t)0x00000000) /*!< LCD RAM Register 0  */
#define LCD_RAMRegister_1                  ((uint32_t)0x00000001) /*!< LCD RAM Register 1  */
#define LCD_RAMRegister_2                  ((uint32_t)0x00000002) /*!< LCD RAM Register 2  */
#define LCD_RAMRegister_3                  ((uint32_t)0x00000003) /*!< LCD RAM Register 3  */
#define LCD_RAMRegister_4                  ((uint32_t)0x00000004) /*!< LCD RAM Register 4  */
#define LCD_RAMRegister_5                  ((uint32_t)0x00000005) /*!< LCD RAM Register 5  */
#define LCD_RAMRegister_6                  ((uint32_t)0x00000006) /*!< LCD RAM Register 6  */
#define LCD_RAMRegister_7                  ((uint32_t)0x00000007) /*!< LCD RAM Register 7  */
#define LCD_RAMRegister_8                  ((uint32_t)0x00000008) /*!< LCD RAM Register 8  */
#define LCD_RAMRegister_9                  ((uint32_t)0x00000009) /*!< LCD RAM Register 9  */
#define LCD_RAMRegister_10                 ((uint32_t)0x0000000A) /*!< LCD RAM Register 10 */
#define LCD_RAMRegister_11                 ((uint32_t)0x0000000B) /*!< LCD RAM Register 11 */
#define LCD_RAMRegister_12                 ((uint32_t)0x0000000C) /*!< LCD RAM Register 12 */
#define LCD_RAMRegister_13                 ((uint32_t)0x0000000D) /*!< LCD RAM Register 13 */

#define IS_LCD_RAM_REGISTER(REGISTER)      (((REGISTER) == LCD_RAMRegister_0) || \
                                           ((REGISTER) == LCD_RAMRegister_1) || \
                                           ((REGISTER) == LCD_RAMRegister_2) || \
                                           ((REGISTER) == LCD_RAMRegister_3) || \
                                           ((REGISTER) == LCD_RAMRegister_4) || \
                                           ((REGISTER) == LCD_RAMRegister_5) || \
                                           ((REGISTER) == LCD_RAMRegister_6) || \
                                           ((REGISTER) == LCD_RAMRegister_7) || \
                                           ((REGISTER) == LCD_RAMRegister_8) || \
                                           ((REGISTER) == LCD_RAMRegister_9) || \
                                           ((REGISTER) == LCD_RAMRegister_10) || \
                                           ((REGISTER) == LCD_RAMRegister_11) || \
                                           ((REGISTER) == LCD_RAMRegister_12) || \
                                           ((REGISTER) == LCD_RAMRegister_13) )

#define LCD_FLAG_INTF                      (((uint32_t) 0x00000001) << 12)
#define IS_LCD_GET_FLAG(LCD_FLAG)          ((LCD_FLAG) == LCD_FLAG_INTF)
#define IS_LCD_CLEAR_FLAG(LCD_FLAG)        ((LCD_FLAG) == LCD_FLAG_INTF)

#define LCD_COM0                           (((uint32_t)0x00000001) << 24)
#define LCD_COM1                           (((uint32_t)0x00000001) << 25)
#define LCD_COM2                           (((uint32_t)0x00000001) << 26)
#define LCD_COM3                           (((uint32_t)0x00000001) << 27)
#define LCD_COM4                           (((uint32_t)0x00000001) << 28)
#define LCD_COM5                           (((uint32_t)0x00000001) << 29)
#define LCD_COM6                           (((uint32_t)0x00000001) << 30)
#define LCD_COM7                           (((uint32_t)0x00000001) << 31)
#define IS_LCD_COM(COM)                    (((COM) & 0xFF000000) <= 0xFF000000)


#define LCD_SEG0                           (((uint32_t)0x00000001))
#define LCD_SEG1                           (((uint32_t)0x00000001) << 1)
#define LCD_SEG2                           (((uint32_t)0x00000001) << 2)
#define LCD_SEG3                           (((uint32_t)0x00000001) << 3)
#define LCD_SEG4                           (((uint32_t)0x00000001) << 4)
#define LCD_SEG5                           (((uint32_t)0x00000001) << 5)
#define LCD_SEG6                           (((uint32_t)0x00000001) << 6)
#define LCD_SEG7                           (((uint32_t)0x00000001) << 7)
#define LCD_SEG8                           (((uint32_t)0x00000001) << 8)
#define LCD_SEG9                           (((uint32_t)0x00000001) << 9)
#define LCD_SEG10                          (((uint32_t)0x00000001) << 10)
#define LCD_SEG11                          (((uint32_t)0x00000001) << 11)
#define LCD_SEG12                          (((uint32_t)0x00000001) << 12)
#define LCD_SEG13                          (((uint32_t)0x00000001) << 13)
#define LCD_SEG14                          (((uint32_t)0x00000001) << 14)
#define LCD_SEG15                          (((uint32_t)0x00000001) << 15)
#define LCD_SEG16                          (((uint32_t)0x00000001) << 16)
#define LCD_SEG17                          (((uint32_t)0x00000001) << 17)
#define LCD_SEG18                          (((uint32_t)0x00000001) << 18)
#define LCD_SEG19                          (((uint32_t)0x00000001) << 19)
#define LCD_SEG20                          (((uint32_t)0x00000001) << 20)
#define LCD_SEG21                          (((uint32_t)0x00000001) << 21)
#define LCD_SEG22                          (((uint32_t)0x00000001) << 22)
#define LCD_SEG23                          (((uint32_t)0x00000001) << 23)
#define IS_LCD_SEGX(SEGX)                  ((SEGX) <= 0x00FFFFFF && (SEGX) > 0x00000000)


#define LCD_SEG24                          (((uint32_t)0x00000001) << 24)
#define LCD_SEG25                          (((uint32_t)0x00000001) << 25)
#define LCD_SEG26                          (((uint32_t)0x00000001) << 26)
#define LCD_SEG27                          (((uint32_t)0x00000001) << 27)
#define IS_LCD_SEGV(SEGV)                  ((SEGV) == LCD_SEG24 ||\
                                            (SEGV) == LCD_SEG25 ||\
                                            (SEGV) == LCD_SEG26 ||\
                                            (SEGV) == LCD_SEG27)


#define LCD_SEG32                          (((uint32_t)0x00000001))
#define LCD_SEG33                          (((uint32_t)0x00000001) << 1)
#define LCD_SEG34                          (((uint32_t)0x00000001) << 2)
#define LCD_SEG35                          (((uint32_t)0x00000001) << 3)
#define LCD_SEG36                          (((uint32_t)0x00000001) << 4)
#define LCD_SEG37                          (((uint32_t)0x00000001) << 5)
#define LCD_SEG38                          (((uint32_t)0x00000001) << 6)
#define LCD_SEG39                          (((uint32_t)0x00000001) << 7)
#define LCD_SEG40                          (((uint32_t)0x00000001) << 8)
#define LCD_SEG41                          (((uint32_t)0x00000001) << 9)
#define LCD_SEG42                          (((uint32_t)0x00000001) << 10)
#define LCD_SEG43                          (((uint32_t)0x00000001) << 11)
#define LCD_SEG44                          (((uint32_t)0x00000001) << 12)
#define LCD_SEG45                          (((uint32_t)0x00000001) << 13)
#define LCD_SEG46                          (((uint32_t)0x00000001) << 14)
#define LCD_SEG47                          (((uint32_t)0x00000001) << 15)
#define LCD_SEG48                          (((uint32_t)0x00000001) << 16)
#define LCD_SEG49                          (((uint32_t)0x00000001) << 17)
#define LCD_SEG50                          (((uint32_t)0x00000001) << 18)
#define LCD_SEG51                          (((uint32_t)0x00000001) << 19)
#define LCD_SEG52                          (((uint32_t)0x00000001) << 20)
#define LCD_SEG53                          (((uint32_t)0x00000001) << 21)
#define LCD_SEG54                          (((uint32_t)0x00000001) << 22)
#define LCD_SEG55                          (((uint32_t)0x00000001) << 23)
#define IS_LCD_SEGY(SEGY)                  ((SEGY) <= 0x00FFFFFF && (SEGY) > 0x00000000)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void LCD_DeInit(void);
void LCD_Init(LCD_InitTypeDef* LCD_InitStruct);
void LCD_Cmd(FunctionalState NewState);
void LCD_ContrastConfig(uint32_t LCD_ContrastLevel);
void LCD_DriveVoltageConfig(uint32_t LCD_INRS_Level);
void LCD_ChargeBumpCmd(FunctionalState NewState);
void LCD_ChargeClockConfig(uint32_t LCD_ChargeClock);
void LCD_DMACmd(FunctionalState NewState);
void LCD_ScanFreqConfig(uint32_t LCD_ScanFreq);
void LCD_ClockSourceConfig(uint32_t LCD_ClockSource);
void LCD_IntOrBlinkDurationConfig(uint32_t LCD_Duration);
void LCD_BlinkCmd(FunctionalState NewState);
void LCD_Write(uint32_t LCD_RAMRegister, uint32_t LCD_Data);
void LCD_ITCmd(FunctionalState NewState);
ITStatus LCD_GetITStatus(void);
void LCD_ClearITPendingBit(void);
void LCD_COMConfig(uint32_t LCD_COM_x, FunctionalState NewState);
void LCD_SEG0to23Config(uint32_t LCD_SEG_x, FunctionalState NewState);
void LCD_SEG32to55Config(uint32_t LCD_SEG_y, FunctionalState NewState);
void LCD_SEG24to27Config(uint32_t LCD_SEG_v, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__CW32L083_LCD_H */
/**
  * @}
  */
