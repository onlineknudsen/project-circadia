#ifndef I2S_H
#define I2S_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "esp_system.h"

#define SAMPLE_RATE 44100
#define I2S_BCLK 26
#define I2S_LRC 25
#define I2S_DOUT 17

#define I2S_PORT (i2s_port_t)0
#endif