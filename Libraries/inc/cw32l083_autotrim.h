/**
 * @file cw32l083_autotrim.h
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2021-05-18
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CW32L083_AUTOTRIM_H
#define __CW32L083_AUTOTRIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "base_types.h"
#include "cw32l083.h"

typedef struct
{
    uint32_t AUTOTRIM_ClkSource;         /* 时钟源选择，设置值0 - 4 */
    uint32_t AUTOTRIM_Prescaler;         /* 预分配系数，设置值1 - 15，对应取值范围2的n次幂，n=1,2…15 */
    uint32_t AUTOTRIM_Period;            /* 计数重载周期，低16位有效，取值范围0x0000 到 0xFFFF.  */
   
} AUTOTRIM_TimeCntInitTypeDef;

typedef struct
{
    uint32_t TrimMode;        // 校准模式：AUTOTRIM_TRIMMODE_HSIOSC/AUTOTRIM_TRIMMODE_LSI
    uint32_t TrimType;        // 校准类型：AUTOTRIM_TRIMTYPE_SINGLE/ AUTOTRIM_TRIMTYPE_CONSTANT
    uint32_t TrimAuto;        // 自动校准：AUTOTRIM_AUTO_ENABLE/AUTOTRIM_AUTO_DISABLE
    uint32_t TrimClkSource;   // 时钟源选择: AUTOTRIM_CLKSOURCE_HSIOSC/AUTOTRIM_CLKSOURCE_LSI/AUTOTRIM_CLKSOURCE_HSE /AUTOTRIM_CLKSOURCE_LSE/AUTOTRIM_CLKSOURCE_ETR 
    uint32_t TrimClkPrescale; // 预分配系数:AUTOTRIM_PRS_DIV2~AUTOTRIM_PRS_DIV32768
    uint32_t TrimPeriod;      // 校准周期：实际一个校准周期为此值的1.5倍,该值的选取ARR = TCLK×2的PRS次幂/RCLK-1，详见用户手册        
}AUTOTRIM_TrimInitTypeDef;

#define AUTOTRIM_CLKSOURCE_HSIOSC               ( uint32_t )(0x00UL << 8)
#define AUTOTRIM_CLKSOURCE_LSI                  ( uint32_t )(0x01UL << 8)
#define AUTOTRIM_CLKSOURCE_HSE                  ( uint32_t )(0x02UL << 8)
#define AUTOTRIM_CLKSOURCE_LSE                  ( uint32_t )(0x03UL << 8)
#define AUTOTRIM_CLKSOURCE_ETR                  ( uint32_t )(0x04UL << 8)
#define IS_AUTOTRIM_CLKSOURCE(SOURCE)           (((SOURCE) == AUTOTRIM_CLKSOURCE_HSIOSC) || \
                                                 ((SOURCE) == AUTOTRIM_CLKSOURCE_LSI)     || \
                                                 ((SOURCE) == AUTOTRIM_CLKSOURCE_HSE)     || \
                                                 ((SOURCE) == AUTOTRIM_CLKSOURCE_LSE)     || \
                                                 ((SOURCE) == AUTOTRIM_CLKSOURCE_ETR))

#define AUTOTRIM_PRS_DIV2                       ( uint32_t )(0x01UL << 4)
#define AUTOTRIM_PRS_DIV4                       ( uint32_t )(0x02UL << 4)
#define AUTOTRIM_PRS_DIV8                       ( uint32_t )(0x03UL << 4)
#define AUTOTRIM_PRS_DIV16                      ( uint32_t )(0x04UL << 4)
#define AUTOTRIM_PRS_DIV32                      ( uint32_t )(0x05UL << 4)
#define AUTOTRIM_PRS_DIV64                      ( uint32_t )(0x06UL << 4)
#define AUTOTRIM_PRS_DIV128                     ( uint32_t )(0x07UL << 4)
#define AUTOTRIM_PRS_DIV256                     ( uint32_t )(0x08UL << 4)
#define AUTOTRIM_PRS_DIV512                     ( uint32_t )(0x09UL << 4)
#define AUTOTRIM_PRS_DIV1024                    ( uint32_t )(0x0AUL << 4)
#define AUTOTRIM_PRS_DIV2048                    ( uint32_t )(0x0BUL << 4)
#define AUTOTRIM_PRS_DIV4096                    ( uint32_t )(0x0CUL << 4)
#define AUTOTRIM_PRS_DIV8192                    ( uint32_t )(0x0DUL << 4)
#define AUTOTRIM_PRS_DIV16384                   ( uint32_t )(0x0EUL << 4)
#define AUTOTRIM_PRS_DIV32768                   ( uint32_t )(0x0FUL << 4)
#define IS_AUTOTRIM_PRS_DIV(DIV)                (((DIV) == AUTOTRIM_PRS_DIV2)    || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV4)     || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV8)     || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV16)    || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV32)    || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV64)    || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV128)   || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV256)   || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV512)   || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV1024)  || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV2048)  || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV4096)  || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV8192)  || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV16384) || \
                                                 ((DIV) == AUTOTRIM_PRS_DIV32768))

#define AUTOTRIM_AUTO_ENABLE                    (bv3)
#define AUTOTRIM_AUTO_DISABLE                   (0)
#define IS_AUTOTRIM_AUTO(AUTO)                  ((AUTO)==AUTOTRIM_AUTO_ENABLE || \
                                                 (AUTO) == AUTOTRIM_AUTO_DISABLE)

#define AUTOTRIM_MODE_HSIOSC                    ( uint32_t )(0x00UL << 1)
#define AUTOTRIM_MODE_LSI                       ( uint32_t )(0x01UL << 1)
#define AUTOTRIM_MODE_TIMECNT                   ( uint32_t )(0x03UL << 1)
#define IS_AUTOTRIM_MODE(MODE)                  ((MODE) == AUTOTRIM_MODE_TIMECNT ||\
                                                 (MODE) == AUTOTRIM_MODE_HSIOSC  ||\
                                                 (MODE) == AUTOTRIM_MODE_LSI)

#define AUTOTRIM_TRIMTYPE_SINGLE                (bv11)
#define AUTOTRIM_TRIMTYPE_CONSTANT              (0)
#define IS_AUTOTRIM_TYPE(TYPE)                  ((TYPE) == AUTOTRIM_TRIMTYPE_SINGLE ||\
                                                 (TYPE) == AUTOTRIM_TRIMTYPE_CONSTANT)




#define AUTOTRIM_CR_EN                          (bv0)

#define IS_AUTOTRIM_ARR(ARR)                    ((ARR) <= 65535)

#define AUTOTRIM_IT_MISS                        ((uint32_t)0x20)
#define AUTOTRIM_IT_OV                          ((uint32_t)0x10)
#define AUTOTRIM_IT_UD                          ((uint32_t)0x08)
#define AUTOTRIM_IT_OK                          ((uint32_t)0x02)
#define AUTOTRIM_IT_END                         ((uint32_t)0x01)
#define IS_AUTOTRIM_IT(IT)                      ((((IT) & (uint32_t)0xFFFFFFC4) == 0x0000) && ((IT) != 0x0000))


void AUTOTRIM_DeInit(void);
void AUTOTRIM_TimeCntInit(AUTOTRIM_TimeCntInitTypeDef* AUTOTRIM_TimeCntInitStruct);
void AUTOTRIM_TimeCntStructInit(AUTOTRIM_TimeCntInitTypeDef* AUTOTRIM_TimeCntInitStruct);
void AUTOTRIM_TrimInit(AUTOTRIM_TrimInitTypeDef* AUTOTRIM_TrimInitStruct);
void AUTOTRIM_Cmd(FunctionalState NewState);
void AUTOTRIM_ITConfig(uint32_t AUTOTRIM_IT, FunctionalState NewState);
ITStatus AUTOTRIM_GetITStatus(uint32_t AUTOTRIM_IT);
void AUTOTRIM_ClearITPendingBit(uint32_t AUTOTRIM_IT);
void AUTOTRIM_ClkSourceConfig(uint32_t AUTOTRIM_ClkSource);
void AUTOTRIM_PrescalerConfig(uint32_t AUTOTRIM_Prescaler);
uint16_t AUTOTRIM_GetCounter(void);
uint16_t AUTOTRIM_GetAutoreload(void);
uint32_t AUTOTRIM_GetClkSource(void);
uint32_t AUTOTRIM_GetPrescaler(void);
uint32_t AUTOTRIM_GetTrimValue(void);
void AUTOTRIM_LSITrim(uint32_t TrimValue);
void AUTOTRIM_HSIOSCTrim(uint32_t TrimValue);


#ifdef __cplusplus
}
#endif

#endif

