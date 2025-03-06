#include "i2c.h"
#include "main.h"

// 已改为串口通信，i2c驱动已舍弃

void i2c1_init()
{
    // PB8,PB9复用为i2c1
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef I2C_InitStruct = {0};

    CW_SYSCTRL->AHBEN_f.GPIOA = 1;
    // CW_SYSCTRL->AHBEN_f.GPIOB = 1;
    //  CW_SYSCTRL->AHBEN_f.GPIOC = 1;
    //  CW_SYSCTRL->AHBEN_f.GPIOD = 1;
    //  CW_SYSCTRL->AHBEN_f.GPIOE = 1;
    //  CW_SYSCTRL->AHBEN_f.GPIOF = 1;

    PA09_AFx_I2C1SCL();
    PA10_AFx_I2C1SDA();
    GPIO_InitStructure.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);

    // I2C初始化
    I2C_InitStruct.I2C_OwnSlaveAddr0 = OwnSlaveAddress0; // TEST ADDRESS0
    I2C_InitStruct.I2C_OwnSlaveAddr1 = OwnSlaveAddress1; // TEST ADDRESS1
    I2C_InitStruct.I2C_OwnSlaveAddr2 = OwnSlaveAddress2; // TEST ADDRESS2

    I2C_InitStruct.I2C_OwnGc = DISABLE;
    I2C_InitStruct.I2C_FLT = DISABLE;
    I2C_InitStruct.I2C_AA = DISABLE;

    I2C1_DeInit();
    I2C_Slave_Init(CW_I2C1, &I2C_InitStruct); // 初始化模块
    I2C_Cmd(CW_I2C1, ENABLE);                 // 模块使能
    I2C_AcknowledgeConfig(CW_I2C1, ENABLE);   // ACK打开
}
