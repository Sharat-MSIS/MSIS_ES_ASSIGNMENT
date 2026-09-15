/*
 * Name - Sujith.S
 * Roll number - 261100620020
 * Program - To glow alternate LEDs using button interrupt.
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
/* LED PIN MAPPING                                                            */
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
/* GLOBAL VARIABLE                                                            */
/* -------------------------------------------------------------------------- */

/*
 * This variable stores the currently selected LED pattern.
 *
 * alternate = 0 -> LED 0,2,4,6,8 ON
 * alternate = 1 -> LED 1,3,5,7,9 ON
 *
 * It is declared volatile because it is accessed by the interrupt service
 * routine.
 */
volatile uint8_t alternate = 0;


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
            /*
             * Bit is 0, so turn the corresponding LED OFF.
             *
             * Upper 16 bits of BSRR are used to reset GPIO pins.
             */
            led_pins[i].port->BSRR =
                    (1u << (led_pins[i].pin + 16));
        }
    }
}


/* -------------------------------------------------------------------------- */
/* EXTI13 INTERRUPT SERVICE ROUTINE                                           */
/* -------------------------------------------------------------------------- */

/*
 * This function is automatically executed when EXTI13 interrupt occurs.
 *
 * PC13 is connected to EXTI13.
 *
 * EXTI13 belongs to the EXTI15_10 interrupt group.
 */
void EXTI15_10_IRQHandler(void)
{
    /*
     * Check whether EXTI13 generated the interrupt.
     *
     * Bit 13 of the EXTI pending register corresponds to EXTI13.
     */
    if(EXTI->PR & (1u << 13))
    {
        /* Clear the EXTI13 pending flag */
        EXTI->PR = (1u << 13);

        /*
         * Toggle the alternate LED pattern.
         *
         * 0 becomes 1.
         * 1 becomes 0.
         */
        alternate = !alternate;

        /* Check which LED pattern should be displayed */
        if(alternate == 0)
        {
            /*
             * 0x155 = 0101010101
             *
             * LED 0,2,4,6,8 ON
             */
            leds_write(0x155u);
        }
        else
        {
            /*
             * 0x2AA = 1010101010
             *
             * LED 1,3,5,7,9 ON
             */
            leds_write(0x2AAu);
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

    /*
     * Configure PC13 for interrupt on falling edge.
     *
     * The button is active LOW, therefore pressing the button causes
     * the signal to go from HIGH to LOW.
     */
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;

    /* Configure GPIO speed as FAST */
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /* Configure output type as push-pull */
    GpioBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

    /* Enable internal pull-up resistor */
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    /* No alternate function is required */
    GpioBtn.GPIO_PinConfig.GPIO_PinAltFunMode = 0;


    /* ---------------------------------------------------------------------- */
    /* ENABLE GPIO CLOCKS                                                     */
    /* ---------------------------------------------------------------------- */

    /* Enable the clock for GPIOA because LEDs use GPIOA */
    GPIO_PeriClockControl(GPIOA, ENABLE);

    /* Enable the clock for GPIOB because LEDs use GPIOB */
    GPIO_PeriClockControl(GPIOB, ENABLE);

    /* Enable the clock for GPIOC because the button uses GPIOC */
    GPIO_PeriClockControl(GPIOC, ENABLE);


    /* ---------------------------------------------------------------------- */
    /* INITIALIZE GPIOs                                                       */
    /* ---------------------------------------------------------------------- */

    /* Initialize all 10 LED GPIO pins */
    for(int i = 0; i < 10; i++)
    {
        GPIO_Init(&GpioLed[i]);
    }

    /* Initialize PC13 */
    GPIO_Init(&GpioBtn);


    /* ---------------------------------------------------------------------- */
    /* INITIAL LED PATTERN                                                    */
    /* ---------------------------------------------------------------------- */

    /*
     * 0x155 = 0101010101
     *
     * LED 0,2,4,6,8 are ON.
     */
    leds_write(0x155u);


    /* ---------------------------------------------------------------------- */
    /* SYSCFG CONFIGURATION FOR EXTI13                                        */
    /* ---------------------------------------------------------------------- */

    /*
     * Enable the SYSCFG peripheral clock.
     *
     * SYSCFG is required to connect GPIOC pin 13 to EXTI13.
     */
    SYSCFG_PCLK_EN();


    /*
     * EXTI13 belongs to EXTICR[3].
     *
     * EXTICR[3] controls GPIO pins 12,13,14 and 15.
     *
     * For pin 13, the field starts at bit 4.
     *
     * GPIOC corresponds to value 2.
     *
     * Clear the existing configuration first.
     */
    SYSCFG->EXTICR[3] &= ~(0xFu << 4);

    /*
     * Select GPIOC for EXTI13.
     *
     * 2 = GPIOC.
     */
    SYSCFG->EXTICR[3] |= (0x2u << 4);


    /* ---------------------------------------------------------------------- */
    /* EXTI CONFIGURATION                                                     */
    /* ---------------------------------------------------------------------- */

    /*
     * Unmask EXTI13.
     *
     * This allows EXTI13 to generate an interrupt.
     */
    EXTI->IMR |= (1u << 13);

    /*
     * Enable falling-edge trigger for EXTI13.
     *
     * Button press changes PC13 from HIGH to LOW.
     */
    EXTI->FTSR |= (1u << 13);

    /*
     * Disable rising-edge trigger.
     *
     * We only want an interrupt when the button is pressed.
     */
    EXTI->RTSR &= ~(1u << 13);

    /*
     * Clear any pending EXTI13 interrupt before enabling NVIC.
     */
    EXTI->PR = (1u << 13);


    /* ---------------------------------------------------------------------- */
    /* NVIC CONFIGURATION                                                     */
    /* ---------------------------------------------------------------------- */

    /*
     * EXTI15_10 IRQ number is 40.
     *
     * IRQ 40 belongs to NVIC_ISER1 because:
     *
     * 40 / 32 = 1
     */
    *NVIC_ISER1 |= (1u << (IRQ_NO_EXTI15_10 - 32));


    /* ---------------------------------------------------------------------- */
    /* MAIN LOOP                                                              */
    /* ---------------------------------------------------------------------- */

    /*
     * No button polling is required here.
     *
     * The CPU waits for an interrupt.
     */
    while(1)
    {
        /*
         * Main program can remain empty because the LED pattern
         * is changed inside EXTI15_10_IRQHandler().
         */
    }


    /* Return 0 from main function */
    return 0;
}
