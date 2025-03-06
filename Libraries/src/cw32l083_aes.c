/**
 * @file cw32l083_aes.c
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
#include "cw32l083_aes.h"


/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief AES加密
 *
 * @param AES_InitStruct
 */
int AES_Encrypt(AES_InitTypeDef* AES_InitStruct)
{
    uint16_t timeout = 0xFFFF;
    assert_param(IS_AES_Key_Size(AES_InitStruct->AES_KeySize));

    /* 密钥配置 */
    switch(AES_InitStruct->AES_KeySize)
    {
        case AES_KeySize_128:
            CW_AES->CR_f.KEYSIZE = 0;
            CW_AES->KEY0 = AES_InitStruct->AES_Key[0];
            CW_AES->KEY1 = AES_InitStruct->AES_Key[1];
            CW_AES->KEY2 = AES_InitStruct->AES_Key[2];
            CW_AES->KEY3 = AES_InitStruct->AES_Key[3];
            break;
        case AES_KeySize_192:
            CW_AES->CR_f.KEYSIZE = 1;
            CW_AES->KEY0 = AES_InitStruct->AES_Key[0];
            CW_AES->KEY1 = AES_InitStruct->AES_Key[1];
            CW_AES->KEY2 = AES_InitStruct->AES_Key[2];
            CW_AES->KEY3 = AES_InitStruct->AES_Key[3];
            CW_AES->KEY4 = AES_InitStruct->AES_Key[4];
            CW_AES->KEY5 = AES_InitStruct->AES_Key[5];
            break;
        case AES_KeySize_256:
            CW_AES->CR_f.KEYSIZE = 2;
            CW_AES->KEY0 = AES_InitStruct->AES_Key[0];
            CW_AES->KEY1 = AES_InitStruct->AES_Key[1];
            CW_AES->KEY2 = AES_InitStruct->AES_Key[2];
            CW_AES->KEY3 = AES_InitStruct->AES_Key[3];
            CW_AES->KEY4 = AES_InitStruct->AES_Key[4];
            CW_AES->KEY5 = AES_InitStruct->AES_Key[5];
            CW_AES->KEY6 = AES_InitStruct->AES_Key[6];
            CW_AES->KEY7 = AES_InitStruct->AES_Key[7];
            break;
        default:
            break;
    }

    /* 明文配置 */
    CW_AES->DATA0 = AES_InitStruct->AES_Plaintext[0];
    CW_AES->DATA1 = AES_InitStruct->AES_Plaintext[1];
    CW_AES->DATA2 = AES_InitStruct->AES_Plaintext[2];
    CW_AES->DATA3 = AES_InitStruct->AES_Plaintext[3];

    CW_AES->CR_f.MODE = 0;
    CW_AES->CR_f.START = 1;
    while (CW_AES->CR_f.START != RESET && timeout--);
    if (timeout == 0) return 1;

    AES_InitStruct->AES_Ciphertext[0] = CW_AES->DATA0;
    AES_InitStruct->AES_Ciphertext[1] = CW_AES->DATA1;
    AES_InitStruct->AES_Ciphertext[2] = CW_AES->DATA2;
    AES_InitStruct->AES_Ciphertext[3] = CW_AES->DATA3;
    return 0;
}

/**
 * @brief AES解密
 *
 * @param AES_InitStruct
 */
int AES_Decrypt(AES_InitTypeDef* AES_InitStruct)
{
    uint16_t timeout = 0xffff;
    assert_param(IS_AES_Key_Size(AES_InitStruct->AES_KeySize));

    /* 密钥配置 */
    switch(AES_InitStruct->AES_KeySize)
    {
        case AES_KeySize_128:
            CW_AES->CR_f.KEYSIZE = 0;
            CW_AES->KEY0 = AES_InitStruct->AES_Key[0];
            CW_AES->KEY1 = AES_InitStruct->AES_Key[1];
            CW_AES->KEY2 = AES_InitStruct->AES_Key[2];
            CW_AES->KEY3 = AES_InitStruct->AES_Key[3];
            break;
        case AES_KeySize_192:
            CW_AES->CR_f.KEYSIZE = 1;
            CW_AES->KEY0 = AES_InitStruct->AES_Key[0];
            CW_AES->KEY1 = AES_InitStruct->AES_Key[1];
            CW_AES->KEY2 = AES_InitStruct->AES_Key[2];
            CW_AES->KEY3 = AES_InitStruct->AES_Key[3];
            CW_AES->KEY4 = AES_InitStruct->AES_Key[4];
            CW_AES->KEY5 = AES_InitStruct->AES_Key[5];
            break;
        case AES_KeySize_256:
            CW_AES->CR_f.KEYSIZE = 2;
            CW_AES->KEY0 = AES_InitStruct->AES_Key[0];
            CW_AES->KEY1 = AES_InitStruct->AES_Key[1];
            CW_AES->KEY2 = AES_InitStruct->AES_Key[2];
            CW_AES->KEY3 = AES_InitStruct->AES_Key[3];
            CW_AES->KEY4 = AES_InitStruct->AES_Key[4];
            CW_AES->KEY5 = AES_InitStruct->AES_Key[5];
            CW_AES->KEY6 = AES_InitStruct->AES_Key[6];
            CW_AES->KEY7 = AES_InitStruct->AES_Key[7];
            break;
        default:
            break;
    }

    /* 密文配置 */
    CW_AES->DATA0 = AES_InitStruct->AES_Ciphertext[0];
    CW_AES->DATA1 = AES_InitStruct->AES_Ciphertext[1];
    CW_AES->DATA2 = AES_InitStruct->AES_Ciphertext[2];
    CW_AES->DATA3 = AES_InitStruct->AES_Ciphertext[3];

    CW_AES->CR_f.MODE = 1;
    CW_AES->CR_f.START = 1;
    while (CW_AES->CR_f.START != RESET && timeout--);

    if (timeout == 0) return 1;

    AES_InitStruct->AES_Plaintext[0] = CW_AES->DATA0;
    AES_InitStruct->AES_Plaintext[1] = CW_AES->DATA1;
    AES_InitStruct->AES_Plaintext[2] = CW_AES->DATA2;
    AES_InitStruct->AES_Plaintext[3] = CW_AES->DATA3;

    return 0;
}






