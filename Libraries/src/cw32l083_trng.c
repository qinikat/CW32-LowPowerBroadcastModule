/**
 * @file cw32l083_trng.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-01-12
 *
 * @copyright Copyright (c) 2022
 *
 */
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

/* Includes ------------------------------------------------------------------*/
#include "cw32l083_trng.h"

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief 根据TRNG_InitStruct中指定的参数初始化TRNG外设
 *
 * @param TRNG_InitStruct :指向TRNG_InitTypeDef类型结构体的指针
 */
void TRNG_Init(TRNG_InitTypeDef* TRNG_InitStruct)
{
    assert_param(IS_TRNG_Source(TRNG_InitStruct->TRNG_Source));
    assert_param(IS_TRNG_Shift(TRNG_InitStruct->TRNG_Shift));
    assert_param(IS_TRNG_Random_Source_En(TRNG_InitStruct->TRNG_RandomSourceEn));

    /* TRNG_CR1 */
    REGBITS_MODIFY(CW_TRNG->CR1, TRNG_CR1_EN_Msk, TRNG_InitStruct->TRNG_RandomSourceEn);

    /* TRNG_CR2 */
    REGBITS_MODIFY(CW_TRNG->CR2, TRNG_CR2_SHIFT_Msk | TRNG_CR2_SOURCE_Msk \
                   ,\
                   TRNG_InitStruct->TRNG_Shift |\
                   TRNG_InitStruct->TRNG_Source);
}

/**
 * @brief TRNG去初始化
 *
 */
void TRNG_DeInit(void)
{
    CW_SYSCTRL->AHBRST_f.TRNG = 0;
    CW_SYSCTRL->AHBRST_f.TRNG = 1;
}

/**
 * @brief 生成随机数
 *
 * @param TRNG_InitStruct
 */
int TRNG_Generate(TRNG_InitTypeDef* TRNG_InitStruct)
{
    uint16_t timeout = 0xffff;
    CW_TRNG->CR1_f.EN = 1;
    CW_TRNG->CR1_f.START = 1;
    while ((CW_TRNG->CR1_f.START) && timeout--);
    if (timeout == 0) return 1;

    CW_TRNG->CR1_f.EN = 0;
    TRNG_InitStruct->TRNG_RandomNumber[0] = CW_TRNG->DATA0;
    TRNG_InitStruct->TRNG_RandomNumber[1] = CW_TRNG->DATA1;
    return 0;
}

/**
 * @brief 启动TRNG运算
 *
 */
void TRNG_Cmd(void)
{
    CW_TRNG->CR1_f.START = 1;
}

/**
 * @brief 获取随机数:bit31~bit0
 *
 * @return uint32_t
 */
uint32_t TRNG_GetRandomNumber0(void)
{
    return CW_TRNG->DATA0;
}

/**
 * @brief 获取随机数:bit63~bit32
 *
 * @return uint32_t
 */
uint32_t TRNG_GetRandomNumber1(void)
{
    return CW_TRNG->DATA1;
}

/**
 * @brief 打开/关闭模拟随机源电路
 *
 * @param NewState :ENABLE or DISABLE
 */
void TRNG_RandomSourceCmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    CW_TRNG->CR1_f.EN = NewState;
}

/**
 * @brief 获取TRNG状态
 *
 * @return FlagStatus :TRNG当前状态 (SET or RESET)
 */
FlagStatus TRNG_GetFlagStatus(void)
{
    return ((CW_TRNG->CR1_f.START) ? SET : RESET);
}






