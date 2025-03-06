/**
 * @file cw32l083_autotrim.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2022-01-17
 *
 * @copyright Copyright (c) 2021
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
#include "cw32l083_autotrim.h"


/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief AUTOTRIM  去初始化
 *
 */
void AUTOTRIM_DeInit(void)
{
    CW_AUTOTRIM->CR = 0UL;
    CW_AUTOTRIM->ARR = 0x0000FFFFUL;
    CW_AUTOTRIM->IER = 0UL;
    CW_AUTOTRIM->ICR = 0UL;
}

/**
 * @brief AUTOTRIM定时器计数初始化
 *
 * @param AUTOTRIM_TimeCntInitStruct
 */
void AUTOTRIM_TimeCntInit(AUTOTRIM_TimeCntInitTypeDef* AUTOTRIM_TimeCntInitStruct)
{
    assert_param(IS_AUTOTRIM_CLKSOURCE(AUTOTRIM_TimeCntInitStruct->AUTOTRIM_ClkSource));
    assert_param(IS_AUTOTRIM_PRS_DIV(AUTOTRIM_TimeCntInitStruct->AUTOTRIM_Prescaler));
    assert_param(IS_AUTOTRIM_ARR(AUTOTRIM_TimeCntInitStruct->AUTOTRIM_Period));
    
    REGBITS_MODIFY(CW_AUTOTRIM->CR, AUTOTRIM_CR_SRC_Msk |\
                   AUTOTRIM_CR_PRS_Msk |\
                   AUTOTRIM_CR_MD_Msk
                   ,\
                   AUTOTRIM_TimeCntInitStruct->AUTOTRIM_ClkSource |\
                   AUTOTRIM_TimeCntInitStruct->AUTOTRIM_Prescaler |\
                   AUTOTRIM_MODE_TIMECNT);

    CW_AUTOTRIM->ARR = AUTOTRIM_TimeCntInitStruct->AUTOTRIM_Period;
}

/**
 * @brief 结构体数据AUTOTRIM_TimeCntInitStruct初始化
 *
 * @param AUTOTRIM_TimeCntInitStruct
 */
void AUTOTRIM_TimeCntStructInit(AUTOTRIM_TimeCntInitTypeDef * AUTOTRIM_TimeCntInitStruct)
{
    AUTOTRIM_TimeCntInitStruct->AUTOTRIM_ClkSource = AUTOTRIM_CLKSOURCE_HSIOSC;
    AUTOTRIM_TimeCntInitStruct->AUTOTRIM_Prescaler = AUTOTRIM_PRS_DIV2;    
    AUTOTRIM_TimeCntInitStruct->AUTOTRIM_Period = (uint32_t)0x0000FFFF;
}

/**
 * @brief AUTOTRIM校准模式初始化
 * 
 * @param AUTOTRIM_TrimInitStruct 
 */
void AUTOTRIM_TrimInit(AUTOTRIM_TrimInitTypeDef* AUTOTRIM_TrimInitStruct)
{
    assert_param(IS_AUTOTRIM_MODE(AUTOTRIM_TrimInitStruct->TrimMode));
    assert_param(IS_AUTOTRIM_TYPE(AUTOTRIM_TrimInitStruct->TrimType));
    assert_param(IS_AUTOTRIM_CLKSOURCE(AUTOTRIM_TrimInitStruct->TrimClkSource));
    assert_param(IS_AUTOTRIM_PRS_DIV(AUTOTRIM_TrimInitStruct->TrimClkPrescale));
    assert_param(IS_AUTOTRIM_ARR(AUTOTRIM_TrimInitStruct->TrimPeriod));

    REGBITS_MODIFY(CW_AUTOTRIM->CR,
        AUTOTRIM_CR_OST_Msk | AUTOTRIM_CR_SRC_Msk | AUTOTRIM_CR_PRS_Msk | AUTOTRIM_CR_AUTO_Msk | AUTOTRIM_CR_MD_Msk,
        AUTOTRIM_TrimInitStruct->TrimType | AUTOTRIM_TrimInitStruct->TrimClkSource | AUTOTRIM_TrimInitStruct->TrimClkPrescale |
        AUTOTRIM_TrimInitStruct->TrimAuto | AUTOTRIM_TrimInitStruct->TrimMode);
    CW_AUTOTRIM->ARR = AUTOTRIM_TrimInitStruct->TrimPeriod;
}

/**
 * @brief 使能AUTOTRIM定时器
 *
 * @param NewState
 */
void AUTOTRIM_Cmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        CW_AUTOTRIM->CR |= AUTOTRIM_CR_EN;
    }
    else
    {
        CW_AUTOTRIM->CR &= (~AUTOTRIM_CR_EN);
    }

}

/**
 * @brief 配置AUTOTRIM的中断
 *
 * @param AUTOTRIM_IT
 * @param NewState
 */
void AUTOTRIM_ITConfig(uint32_t AUTOTRIM_IT, FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_AUTOTRIM_IT(AUTOTRIM_IT));

    if (NewState != DISABLE)
    {
        CW_AUTOTRIM->IER |= AUTOTRIM_IT;
    }
    else
    {
        CW_AUTOTRIM->IER &= (~AUTOTRIM_IT);
    }
}


/**
 * @brief 获取AUTOTRIM中断标志
 *
 * @param AUTOTRIM_IT
 * @return FlagStatus
 */
FlagStatus AUTOTRIM_GetITStatus(uint32_t AUTOTRIM_IT)
{
    assert_param(IS_AUTOTRIM_IT(AUTOTRIM_IT));

    return (CW_AUTOTRIM->ISR & AUTOTRIM_IT) ? SET : RESET;
}

/**
 * @brief 清除AUTOTRIM中断标志
 *
 * @param AUTOTRIM_IT
 */
void AUTOTRIM_ClearITPendingBit(uint32_t AUTOTRIM_IT)
{
    assert_param(IS_AUTOTRIM_IT(AUTOTRIM_IT));

    CW_AUTOTRIM->ICR &= (~AUTOTRIM_IT);

}

/**
 * @brief AUTOTRIM计数时钟源配置
 *
 * @param AUTOTRIM_ClkSrouce
 */
void AUTOTRIM_ClkSourceConfig(uint32_t AUTOTRIM_ClkSource)
{
    assert_param(IS_AUTOTRIM_CLKSOURCE(AUTOTRIM_ClkSource));

    REGBITS_MODIFY(CW_AUTOTRIM->CR, AUTOTRIM_CR_SRC_Msk, AUTOTRIM_ClkSource);
}

/**
 * @brief AUTOTRIM计数时钟分频系数配置
 *
 * @param Prescaler
 */
void AUTOTRIM_PrescalerConfig(uint32_t AUTOTRIM_Prescaler)
{
    assert_param(IS_AUTOTRIM_PRS_DIV(AUTOTRIM_Prescaler));

    REGBITS_MODIFY(CW_AUTOTRIM->CR, AUTOTRIM_CR_PRS_Msk, AUTOTRIM_Prescaler);
}

/**
 * @brief 读取AUTOTRIM计数器CNT寄存器值
 *
 * @return uint16_t
 */
uint16_t AUTOTRIM_GetCounter(void)
{
    return (uint16_t)(CW_AUTOTRIM->CNT);
}

/**
 * @brief 读取AUTOTRIM计数器重载值寄存器ARR值
 *
 * @return uint16_t
 */
uint16_t AUTOTRIM_GetAutoreload(void)
{
    return (uint16_t)(CW_AUTOTRIM->ARR);
}

/**
 * @brief 读取AUTOTRIM时钟源
 *
 * @return uint32_t
 */
uint32_t AUTOTRIM_GetClkSource(void)
{
    return (uint32_t)((CW_AUTOTRIM->CR_f.SRC) );
}

/**
 * @brief 读取AUTOTRIM时钟分频系数
 *
 * @return uint32_t
 */
uint32_t AUTOTRIM_GetPrescaler(void)
{
    return (uint32_t)((CW_AUTOTRIM->CR_f.PRS) );
}

/**
 * @brief 读取校准值
 * 
 * @return uint32_t 
 */
uint32_t AUTOTRIM_GetTrimValue(void)
{
    return (CW_AUTOTRIM->TVAL & 0x0000FFFF);
}

/**
 * @brief 对LSI进行补偿
 * 
 * @param TrimValue 
 */
void AUTOTRIM_LSITrim(uint32_t TrimValue)
{
    CW_SYSCTRL->LSI = (CW_SYSCTRL->LSI & 0xFE00) | TrimValue;
}

/**
 * @brief 对HSIOSC进行补偿
 * 
 * @param TrimValue 
 */
void AUTOTRIM_HSIOSCTrim(uint32_t TrimValue)
{
    CW_SYSCTRL->HSI = (CW_SYSCTRL->HSI & 0xFE00) | TrimValue;
}

