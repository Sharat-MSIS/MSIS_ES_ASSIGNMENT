
/*
 * Name - Sujith.S
 * Roll number - 261100620020
 * Program - To glow alternate LEDs when button pressed.
 *
 * alternate = 0:
 * LED 0,2,4,6,8 ON
 *
 * alternate = 1:
 * LED 1,3,5,7,9 ON
 */

#include "stm32f446xx.h"              // Include STM32F446RE device-specific definitions
#include "stm32f446xx_gpio_driver.h" // Include custom GPIO driver definitions and functions


/* Button is active LOW, so GPIO_PIN_RESET means button is pressed */
#define BTN_PRESSED   GPIO_PIN_RESET

/* Total number of LEDs used in the program */
#define LED_COUNT 10u

/* Mask for the lower 10 bits corresponding to 10 LEDs */
#define LED_MASK 0x000003FFu


/* -------------------------------------------------------------------------- */
/* LED PIN MAPPING                                                           */
/* -------------------------------------------------------------------------- */

/* Structure used to store the GPIO port and pin number of each LED */
typedef struct
{
    GPIO_RegDef_t *port;   // Stores the GPIO port of the LED
    uint8_t pin;           // Stores the GPIO pin number of the LED
} LED_Pin_t;


/* Array containing the GPIO port and pin number for each LED */
const LED_Pin_t led_pins[10] =
{
    {GPIOA, GPIO_PIN_NO_0},   // LED 0 is connected to PA0
    {GPIOA, GPIO_PIN_NO_1},   // LED 1 is connected to PA1
    {GPIOB, GPIO_PIN_NO_0},   // LED 2 is connected to PB0
    {GPIOB, GPIO_PIN_NO_1},   // LED 3 is connected to PB1
    {GPIOA, GPIO_PIN_NO_4},   // LED 4 is connected to PA4
    {GPIOA, GPIO_PIN_NO_5},   // LED 5 is connected to PA5
    {GPIOA, GPIO_PIN_NO_6},   // LED 6 is connected to PA6
    {GPIOA, GPIO_PIN_NO_7},   // LED 7 is connected to PA7
    {GPIOA, GPIO_PIN_NO_8},   // LED 8 is connected to PA8
    {GPIOA, GPIO_PIN_NO_9}    // LED 9 is connected to PA9
};


/* -------------------------------------------------------------------------- */
/* DELAY FUNCTION                                                            */
/* -------------------------------------------------------------------------- */

/* Simple software delay function */
void delay_ms(uint32_t ms)
{
    /* Repeat the loop approximately 1600 times for every millisecond */
    for(volatile uint32_t i = 0; i < (ms * 1600); i++);
}


/* -------------------------------------------------------------------------- */
/* LED PATTERN FUNCTION                                                       */
/* -------------------------------------------------------------------------- */

/* Function used to turn ON/OFF all 10 LEDs according to the given value */
void leds_write(uint32_t value)
{
    /* Keep only the lower 10 bits because only 10 LEDs are used */
    value &= LED_MASK;

    /* Check each LED one by one */
    for(int i = 0; i < LED_COUNT; i++)
    {
        /* Check whether the corresponding bit is set */
        if(value & (1u << i))
        {
            /* Bit is 1, so turn the corresponding LED ON */
            led_pins[i].port->BSRR = (1u << led_pins[i].pin);
        }
        else
        {
            /* Bit is 0, so turn the corresponding LED OFF */
            /*
             * Writing to the upper half of BSRR resets the GPIO pin.
             * Adding 16 selects the reset-bit position.
             */
            led_pins[i].port->BSRR =
                    (1u << (led_pins[i].pin + 16));
        }
    }
}


/* -------------------------------------------------------------------------- */
/* MAIN FUNCTION                                                             */
/* -------------------------------------------------------------------------- */

/* Program execution starts from the main function */
int main(void)
{
    /* Create an array of GPIO configurations for the 10 LEDs */
    GPIO_Handle_t GpioLed[10];

    /* Create one GPIO configuration structure for the push button */
    GPIO_Handle_t GpioBtn;

    /*
     * Variable used to remember which alternate LED pattern is currently
     * selected.
     *
     * alternate = 0 -> LED 0,2,4,6,8 ON
     * alternate = 1 -> LED 1,3,5,7,9 ON
     */
    uint8_t alternate = 0;


    /* ---------------------------------------------------------------------- */
    /* LED CONFIGURATION                                                      */
    /* ---------------------------------------------------------------------- */

    /* Configure all 10 LEDs */
    for(int i = 0; i < 10; i++)
    {
        /* Select the GPIO port for the current LED */
        GpioLed[i].pGPIOx = led_pins[i].port;

        /* Select the GPIO pin number for the current LED */
        GpioLed[i].GPIO_PinConfig.GPIO_PinNumber = led_pins[i].pin;

        /* Configure the LED pin as a digital output */
        GpioLed[i].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;

        /* Configure the GPIO speed as FAST */
        GpioLed[i].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

        /* Configure output type as push-pull */
        GpioLed[i].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

        /* Disable internal pull-up and pull-down resistors */
        GpioLed[i].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

        /* No alternate function is required for normal GPIO output */
        GpioLed[i].GPIO_PinConfig.GPIO_PinAltFunMode = 0;
    }


    /* ---------------------------------------------------------------------- */
    /* BUTTON CONFIGURATION                                                  */
    /* ---------------------------------------------------------------------- */

    /* Select GPIOC as the button port */
    GpioBtn.pGPIOx = GPIOC;

    /* Configure PC13 as the button input pin */
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;

    /* Configure PC13 as a digital input */
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;

    /* Configure GPIO speed as FAST */
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /* Configure input type as push-pull */
    GpioBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

    /* Enable internal pull-up resistor */
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    /* No alternate function is required for the button */
    GpioBtn.GPIO_PinConfig.GPIO_PinAltFunMode = 0;


    /* ---------------------------------------------------------------------- */
    /* ENABLE GPIO CLOCKS                                                    */
    /* ---------------------------------------------------------------------- */

    /* Enable the clock for GPIOA because LEDs use GPIOA */
    GPIO_PeriClockControl(GPIOA, ENABLE);

    /* Enable the clock for GPIOB because LEDs use GPIOB */
    GPIO_PeriClockControl(GPIOB, ENABLE);

    /* Enable the clock for GPIOC because the button uses GPIOC */
    GPIO_PeriClockControl(GPIOC, ENABLE);


    /* ---------------------------------------------------------------------- */
    /* INITIALIZE GPIOs                                                      */
    /* ---------------------------------------------------------------------- */

    /* Initialize all 10 LED GPIO pins using their configurations */
    for(int i = 0; i < 10; i++)
    {
        GPIO_Init(&GpioLed[i]);
    }

    /* Initialize the button GPIO pin */
    GPIO_Init(&GpioBtn);


    /* ---------------------------------------------------------------------- */
    /* INITIAL LED PATTERN                                                   */
    /* ---------------------------------------------------------------------- */

    /*
     * 0x155 = binary 0101010101
     *
     * Bit 0 -> LED 0 ON
     * Bit 1 -> LED 1 OFF
     * Bit 2 -> LED 2 ON
     * Bit 3 -> LED 3 OFF
     * Bit 4 -> LED 4 ON
     * Bit 5 -> LED 5 OFF
     * Bit 6 -> LED 6 ON
     * Bit 7 -> LED 7 OFF
     * Bit 8 -> LED 8 ON
     * Bit 9 -> LED 9 OFF
     *
     * Therefore LED 0,2,4,6,8 are ON.
     */
    leds_write(0x155u);


    /* ---------------------------------------------------------------------- */
    /* MAIN PROGRAM LOOP                                                     */
    /* ---------------------------------------------------------------------- */

    /* Continuously monitor the push button */
    while(1)
    {
        /*
         * Read the button input.
         *
         * Because the button is configured with a pull-up:
         * GPIO_PIN_SET   -> Button released
         * GPIO_PIN_RESET -> Button pressed
         */
        if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)
                == BTN_PRESSED)
        {
            /* Wait for 20 ms for button debounce */
            delay_ms(20);


            /*
             * Read the button again after the debounce delay.
             * This prevents false detection caused by switch bouncing.
             */
            if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)
                    == BTN_PRESSED)
            {
                /*
                 * Toggle the alternate variable.
                 *
                 * If alternate was 0, it becomes 1.
                 * If alternate was 1, it becomes 0.
                 */
                alternate = !alternate;


                /* Check which LED pattern should be displayed */
                if(alternate == 0)
                {
                    /*
                     * 0x155 = 0101010101
                     *
                     * Turn ON:
                     * LED 0, LED 2, LED 4, LED 6 and LED 8
                     */
                    leds_write(0x155u);
                }
                else
                {
                    /*
                     * 0x2AA = 1010101010
                     *
                     * Turn ON:
                     * LED 1, LED 3, LED 5, LED 7 and LED 9
                     */
                    leds_write(0x2AAu);
                }


                /*
                 * Wait until the button is released.
                 *
                 * This prevents one long button press from being detected
                 * as multiple button presses.
                 */
                while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13)
                        == BTN_PRESSED);


                /* Small delay to debounce the button release */
                delay_ms(20);
            }
        }
    }


    /* Return 0 from main function */
    return 0;
}
