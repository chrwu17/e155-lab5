// Christian Wu
// chrwu@g.hmc.edu
// 10/01/25

// Header file for main.c

#ifndef POLLING_H
#define POLLING_H

#include "STM32L432KC.h"
#include <stm32l432xx.h>

#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_TIM.h"

#define ASIGNAL_PIN PA9
#define BSIGNAL_PIN PA6
#define DELAY_TIM TIM2
#define COUNTER_TIM TIM6

#endif