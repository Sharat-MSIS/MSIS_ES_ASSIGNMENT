/*
 * led_pattern.c
 *
 *  Created on: Sep 14, 2026
 *      Author: SHARAT
 */
/*
 * This file contains LED initialization
 * and different LED blinking patterns.
 */

#include "main.h"




/**
 * @brief Initialize GPIOC pins for LED output.
 *
 * PC0 to PC9 are configured as output pins.
 */
void led_init(void)
{
    /*
     * Configure PC0-PC9 as output.
     *
     * MODER:
     * Each GPIO pin uses 2 bits.
     * 01 = General purpose output mode.
     *
     * 0x55555:
     * Binary pattern:
     * 0101 0101 0101 0101 0101
     *
     * This configures PC0-PC9 as output.
     */
    GPIOC->MODER |= 0x55555;

    /*
     * Set high speed for PC0-PC9.
     *
     * OSPEEDR uses 2 bits per pin.
     * 11 = High speed.
     */
    GPIOC->OSPEEDR |= 0xFFFFF;

    /*
     * Initially turn OFF all 10 LEDs.
     *
     * PC0-PC9 correspond to the lower 10 bits.
     */
    GPIOC->ODR &= ~0x3FF;
}


/**
 * @brief Pattern 1
 *
 * Displays different LED combinations.
 *
 * Sequence:
 * 1. All LEDs ON
 * 2. Alternate LEDs ON
 * 3. Alternate LEDs OFF
 *
 * Delay between each step = 500 ms.
 */
void pattern1(void)
{
    /*
     * 0x3FF = 1111111111
     *
     * Turns ON all 10 LEDs:
     * PC0-PC9 = HIGH
     */
    GPIOC->ODR = 0x3FF;

    /* Wait for 500 ms */
    HAL_Delay(500);


    /*
     * 0x155 = 0101010101
     *
     * Turns ON alternate LEDs.
     */
    GPIOC->ODR = 0x155;

    /* Wait for 500 ms */
    HAL_Delay(500);


    /*
     * Invert the alternate LED pattern.
     *
     * & 0x3FF ensures that only PC0-PC9
     * are affected.
     */
    GPIOC->ODR = (~0x155) & 0x3FF;

    /* Wait for 500 ms */
    HAL_Delay(500);
}


/**
 * @brief Pattern 2
 *
 * LEDs are turned ON one by one from
 * PC0 to PC9.
 *
 * After all LEDs are ON, they are turned
 * OFF one by one from PC0 to PC9.
 *
 * Delay = 200 ms.
 */
void patern2(void)
{
    uint16_t led;


    /*
     * Turn ON LEDs one by one.
     *
     * i = 0  -> PC0
     * i = 1  -> PC1
     * i = 2  -> PC2
     * ...
     * i = 9  -> PC9
     */
    for (int i = 0; i < 10; i++)
    {
        /*
         * Create a bit mask for the current LED.
         *
         * Example:
         * i = 0 -> 0000000001
         * i = 1 -> 0000000010
         * i = 2 -> 0000000100
         */
        led = (1 << i);

        /*
         * Turn ON the selected LED.
         *
         * OR operation keeps the previously
         * turned ON LEDs ON.
         */
        GPIOC->ODR |= led;

        /* Wait for 200 ms */
        HAL_Delay(200);
    }


    /*
     * Turn OFF LEDs one by one.
     */
    for (int i = 0; i < 10; i++)
    {
        /*
         * Select the current LED.
         */
        led = (1 << i);

        /*
         * Turn OFF only the selected LED.
         *
         * ~led creates a mask where the
         * selected bit is 0.
         */
        GPIOC->ODR &= ~led;

        /* Wait for 200 ms */
        HAL_Delay(200);
    }
}


/**
 * @brief Pattern 3
 *
 * Creates a running/bouncing LED effect.
 *
 * LED moves:
 *
 * PC0 -> PC1 -> PC2 -> ... -> PC9
 *
 * Then returns:
 *
 * PC9 -> PC8 -> PC7 -> ... -> PC0
 *
 * Delay = 200 ms.
 */
void patern3(void)
{
    uint16_t led;


    /*
     * Move LED from PC0 to PC9.
     */
    for (int i = 0; i < 10; i++)
    {
        /*
         * Create mask for current LED.
         */
        led = (1 << i);

        /*
         * Write only the selected LED.
         *
         * Since '=' is used instead of '|=',
         * the previous LED is turned OFF.
         */
        GPIOC->ODR = led;

        /* Wait for 200 ms */
        HAL_Delay(200);
    }


    /*
     * Move LED back from PC8 to PC0.
     *
     * PC9 is skipped because it is already
     * the last LED in the forward direction.
     */
    for (int i = 8; i >= 0; i--)
    {
        /*
         * Create mask for current LED.
         */
        led = (1 << i);

        /*
         * Turn ON only the selected LED.
         */
        GPIOC->ODR = led;

        /* Wait for 200 ms */
        HAL_Delay(200);
    }


    /*
     * Turn OFF all LEDs after completing
     * the bouncing pattern.
     */
    GPIOC->ODR &= ~0x3FF;
}

