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
#ifndef __CW32L083_AES_H
#define __CW32L083_AES_H

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
 * @brief  AES Init Structure definition
 */

typedef struct
{
    uint32_t* AES_Plaintext;            /*!< AES明文 128bit */

    uint32_t* AES_Ciphertext;           /*!< AES密文 128bit */

    uint32_t* AES_Key;                  /*!< AES密钥 128bit/192bit/256bit */

    uint8_t AES_KeySize;                /*!< AES密钥长度
                                           参数取值 @ref AES_Key_Size */
} AES_InitTypeDef;

/******************************************************************************
 * pre-processor symbols/macros ('#define')
 ******************************************************************************/

/** @defgroup AES_Key_Size
  * @{
  */

#define AES_KeySize_128                    ((uint8_t)0x00)
#define AES_KeySize_192                    ((uint8_t)0x01)
#define AES_KeySize_256                    ((uint8_t)0x02)
#define IS_AES_Key_Size(KeySize)           (((KeySize) == AES_KeySize_128) || \
                                           ((KeySize) == AES_KeySize_192) || \
                                           ((KeySize) == AES_KeySize_256))

/******************************************************************************
* Global variable definitions (declared in header file with 'extern')
******************************************************************************/


/******************************************************************************
* Global function prototypes
******************************************************************************/

/* AES加密 */
int AES_Encrypt(AES_InitTypeDef* AES_InitStruct);

/* AES解密 */
int AES_Decrypt(AES_InitTypeDef* AES_InitStruct);

#ifdef __cplusplus
}
#endif

#endif /*__CW32L083_AES_H */
/**
  * @}
  */








