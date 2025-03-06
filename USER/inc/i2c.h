#ifndef __I2C_H
#define __I2C_H

#include "main.h"

#define I2C1_SCL_GPIO_PORT CW_GPIOA
#define I2C1_SCL_GPIO_PIN GPIO_PIN_9 // 如果改动口线则GPIO初始化代码需要做同步修改
#define I2C1_SDA_GPIO_PORT CW_GPIOA
#define I2C1_SDA_GPIO_PIN GPIO_PIN_10 // 如果改动口线则GPIO初始化代码需要做同步修改

void i2c1_init(void);

#endif
