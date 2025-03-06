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
#ifndef __CW32L083_TRNG_H
#define __CW32L083_TRNG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "base_types.h"
#include "cw32l083.h"

/******************************************************************************
 * type definitions ('typedef')
 ******************************************************************************/

/**
 * @brief  TRNG Init Structure definition
 */

typedef struct
{
    uint16_t TRNG_Source;               /*!< 线性反馈移位寄存器输入信号配置
                                           参数取值 @ref TRNG_Source */

    uint16_t TRNG_Shift;                /*!< 线性反馈移位寄存器移位次数
                                           参数取值 @ref TRNG_Shift */

    uint16_t TRNG_RandomSourceEn;       /*!< 模拟随机源使能控制
                                           参数取值 @ref TRNG_Random_Source_En */

    uint32_t* TRNG_RandomNumber;         /*!< AES随机数 */
} TRNG_InitTypeDef;

/******************************************************************************
 * pre-processor symbols/macros ('#define')
 ******************************************************************************/

/** @defgroup TRNG_Source
  * @{
  */

#define TRNG_Source_FEEDBACK                      ((uint16_t)0x0000) //线性反馈信号
#define TRNG_Source_RANDOM                        ((uint16_t)0x0001) //模拟随机源输出的信号
#define TRNG_Source_XOR                           ((uint16_t)0x0002) //模拟随机源输出的信号与线性反馈信号异或后的信号
#define IS_TRNG_Source(Source)                    (((Source) == TRNG_Source_FEEDBACK) || \
                                                  ((Source) == TRNG_Source_RANDOM)   || \
                                                  ((Source) == TRNG_Source_XOR))

/** @defgroup TRNG_Shift
  * @{
  */

#define TRNG_Shift_8                              ((uint16_t)0x0004)
#define TRNG_Shift_16                             ((uint16_t)0x0008)
#define TRNG_Shift_32                             ((uint16_t)0x000C)
#define TRNG_Shift_64                             ((uint16_t)0x0010)
#define TRNG_Shift_128                            ((uint16_t)0x0014)
#define TRNG_Shift_256                            ((uint16_t)0x0018)
#define IS_TRNG_Shift(Shift)                      (((Shift) == TRNG_Shift_8)   || \
                                                  ((Shift) == TRNG_Shift_16)   || \
                                                  ((Shift) == TRNG_Shift_32)   || \
                                                  ((Shift) == TRNG_Shift_64)   || \
                                                  ((Shift) == TRNG_Shift_128)  || \
                                                  ((Shift) == TRNG_Shift_256))

/** @defgroup TRNG_Random_Source_En
  * @{
  */

#define TRNG_RandomSourceDisable                  ((uint16_t)0x0000)
#define TRNG_RandomSourceEnable                   ((uint16_t)0x0001)
#define IS_TRNG_Random_Source_En(RandomSourceEn)  (((RandomSourceEn) == TRNG_RandomSourceDisable) || \
                                                  ((RandomSourceEn) == TRNG_RandomSourceEnable))

/******************************************************************************
* Global variable definitions (declared in header file with 'extern')
******************************************************************************/


/******************************************************************************
* Global function prototypes
******************************************************************************/

/* 初始化 */
void TRNG_Init(TRNG_InitTypeDef* TRNG_InitStruct);
void TRNG_DeInit(void);

/* 生成随机数 */
int TRNG_Generate(TRNG_InitTypeDef* TRNG_InitStruct);

/* 启动TRNG运算 */
void TRNG_Cmd(void);

/* 获取随机数 */
uint32_t TRNG_GetRandomNumber0(void);
uint32_t TRNG_GetRandomNumber1(void);

/* 打开/关闭模拟随机源电路 */
void TRNG_RandomSourceCmd(FunctionalState NewState);

/* 获取TRNG状态 */
FlagStatus TRNG_GetFlagStatus(void);

#ifdef __cplusplus
}
#endif

#endif /*__CW32L083_TRNG_H */
/**
  * @}
  */








