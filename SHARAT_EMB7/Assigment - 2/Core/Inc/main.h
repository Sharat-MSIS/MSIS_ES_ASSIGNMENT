/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */


/* Define to prevent recursive inclusion -------------------------------------*/

/*
 * Header guard.
 *
 * It prevents this file from being included more than once
 * during compilation.
 */
#ifndef __MAIN_H
#define __MAIN_H


#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

/* STM32F4 HAL library */
#include "stm32f4xx_hal.h"

/* NUCLEO board support package */
#include "stm32f4xx_nucleo.h"


/*
 * Application-specific header files.
 *
 * search.h  -> Search algorithms
 * sort.h    -> Sorting algorithms
 * series.h  -> Series / prime number functions
 */
#include "search.h"
#include "sort.h"
#include "series.h"


/* Standard C libraries */

#include <stdio.h>
#include <stdbool.h>


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


/*
 * Debounce delay/time.
 *
 * This value can be used to avoid detecting
 * multiple button presses caused by mechanical
 * bouncing of the push button.
 */
#define DEBOUNCE 1


/*
 * buttonstate is defined in led_pattern.c.
 *
 * extern tells the compiler that the variable
 * is defined in another source file.
 */
extern bool buttonstate;


/* USER CODE END Includes */


/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


/* USER CODE END ET */


/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */


/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */


/* USER CODE END EM */


/* Exported functions prototypes ---------------------------------------------*/
/* USER CODE BEGIN EFP */


/* USER CODE END EFP */


/**
 * @brief Initialize GPIO pins used for LEDs.
 *
 * Configures the LED pins as output pins.
 */
void led_init(void);


/**
 * @brief LED Pattern 1.
 *
 * Displays the first LED pattern.
 */
void pattern1(void);


/**
 * @brief LED Pattern 2.
 *
 * Displays the second LED pattern.
 */
void patern2(void);


/**
 * @brief LED Pattern 3.
 *
 * Displays the third LED pattern.
 */
void patern3(void);


/**
 * @brief Error Handler.
 *
 * Called whenever a critical HAL error occurs.
 */
void Error_Handler(void);


/* Private defines -----------------------------------------------------------*/

/*
 * USART2 Transmit pin.
 *
 * PA2 is used to transmit UART data.
 */
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA


/*
 * USART2 Receive pin.
 *
 * PA3 is used to receive UART data.
 */
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA


/*
 * SWD Debug interface pins.
 *
 * PA13 -> SWDIO / TMS
 * PA14 -> SWCLK / TCK
 */
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA

#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA


/*
 * SWO debug output pin.
 *
 * PB3 is used for Serial Wire Output.
 */
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB


/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */


/*
 * End of C++ extern block.
 */
#ifdef __cplusplus
}
#endif


/*
 * End of header guard.
 */
#endif /* __MAIN_H */