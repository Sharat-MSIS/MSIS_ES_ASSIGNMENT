/*
 * Name - Sujith.S
 * Roll number - 261100620020
 * Program - To glow alternate LEDs when button pressed using interrupt.
 *
 * alternate = 0:
 * LED 0,2,4,6,8 ON
 *
 * alternate = 1:
 * LED 1,3,5,7,9 ON
 */

#include <string.h>                    // Include memset function
#include "stm32f446xx.h"               // Include STM32F446RE device-specific definitions
#include "stm32f446xx_gpio_driver.h"   // Include custom GPIO driver definitions and functions


/* Total number of LEDs used */
#define LED_COUNT 10u

/* Mask for the lower 10 bits corresponding to 10 LEDs */
#define LED_MASK 0x000003FFu


/* -------------------------------------------------------------------------- */
/* LED PIN MAPPING                                                            */
/* -------------------------------------------------------------------------- */

/* Structure used to store the GPIO port and pin number of each LED */
typedef struct
{
    GPIO_RegDef_t *port;               // Stores the GPIO port of the LED
    uint8_t pin;                       // Stores the GPIO pin number
} LED_Pin_t;


/* Array containing the GPIO port and pin number for each LED */
const LED_Pin_t led_pins[10] =
{
    {GPIOA, GPIO_PIN_NO_0},            // LED 0 is connected to PA0
    {GPIOA, GPIO_PIN_NO_1},            // LED 1 is connected to PA1
    {GPIOB, GPIO_PIN_NO_0},            // LED 2 is connected to PB0
    {GPIOB, GPIO_PIN_NO_1},            // LED 3 is connected to PB1
    {GPIOA, GPIO_PIN_NO_4},            // LED 4 is connected to PA4
    {GPIOA, GPIO_PIN_NO_5},            // LED 5 is connected to PA5
    {GPIOA, GPIO_PIN_NO_6},            // LED 6 is connected to PA6
    {GPIOA, GPIO_PIN_NO_7},            // LED 7 is connected to PA7
    {GPIOA, GPIO_PIN_NO_8},            // LED 8 is connected to PA8
    {GPIOA, GPIO_PIN_NO_9}             // LED 9 is connected to PA9
};


/* -------------------------------------------------------------------------- */
/* GLOBAL VARIABLE                                                            */
/* -------------------------------------------------------------------------- */

/*
 * Stores the current LED pattern.
 *
 * alternate = 0 -> LED 0,2,4,6,8 ON
 * alternate = 1 -> LED 1,3,5,7,9 ON
 *
 * volatile is used because this variable is changed inside the ISR.
 */
volatile uint8_t alternate = 0;


/* -------------------------------------------------------------------------- */
/* DELAY FUNCTION                                                             */
/* -------------------------------------------------------------------------- */

/*
 * Simple software delay function.
 *
 * This delay is used inside the interrupt routine for button debounce.
 */
void delay(void)
{
    /* Burn CPU cycles to provide a small delay */
    for(volatile uint32_t i = 0; i < 500000 / 2; i++);
}


/* -------------------------------------------------------------------------- */
/* LED PATTERN FUNCTION                                                       */
/* -------------------------------------------------------------------------- */

/*
 * Function used to turn ON/OFF all 10 LEDs according to the given value.
 *
 * Bit 0  -> LED 0
 * Bit 1  -> LED 1
 * Bit 2  -> LED 2
 * ...
 * Bit 9  -> LED 9
 */
void leds_write(uint32_t value)
{
    /* Keep only the lower 10 bits */
    value &= LED_MASK;

    /* Check all 10 LEDs */
    for(int i = 0; i < LED_COUNT; i++)
    {
        /* Check whether the corresponding bit is 1 */
        if(value & (1u << i))
        {
            /* Bit is 1, so turn the LED ON */
            led_pins[i].port->BSRR = (1u << led_pins[i].pin);
        }
        else
        {
            /*
             * Bit is 0, so turn the LED OFF.
             *
             * Upper 16 bits of BSRR are used to reset a GPIO pin.
             */
            led_pins[i].port->BSRR = (1u << (led_pins[i].pin + 16));
        }
    }
}


/* -------------------------------------------------------------------------- */
/* MAIN FUNCTION                                                              */
/* -------------------------------------------------------------------------- */

int main(void)
{
    /* Create GPIO handle structures for the 10 LEDs */
    GPIO_Handle_t GpioLed[10];

    /* Create GPIO handle structure for the button */
    GPIO_Handle_t GpioBtn;


    /* Clear all LED handle structures */
    memset(&GpioLed, 0, sizeof(GpioLed));

    /* Clear button handle structure */
    memset(&GpioBtn, 0, sizeof(GpioBtn));


    /* ---------------------------------------------------------------------- */
    /* LED CONFIGURATION                                                      */
    /* ---------------------------------------------------------------------- */

    /* Configure all 10 LEDs */
    for(int i = 0; i < LED_COUNT; i++)
    {
        /* Select the GPIO port of the current LED */
        GpioLed[i].pGPIOx = led_pins[i].port;

        /* Select the GPIO pin number of the current LED */
        GpioLed[i].GPIO_PinConfig.GPIO_PinNumber = led_pins[i].pin;

        /* Configure LED pin as digital output */
        GpioLed[i].GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;

        /* Configure GPIO speed as FAST */
        GpioLed[i].GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

        /* Configure output type as push-pull */
        GpioLed[i].GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

        /* Disable internal pull-up and pull-down resistors */
        GpioLed[i].GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

        /* No alternate function is required */
        GpioLed[i].GPIO_PinConfig.GPIO_PinAltFunMode = 0;
    }


    /* ---------------------------------------------------------------------- */
    /* BUTTON CONFIGURATION                                                   */
    /* ---------------------------------------------------------------------- */

    /* Select GPIOC as the button port */
    GpioBtn.pGPIOx = GPIOC;

    /* Configure PC13 as the button pin */
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber =  GPIO_PIN_NO_13;

    /*
     * Configure PC13 for interrupt on falling edge.
     *
     * Button is active LOW.
     * Therefore pressing the button generates a HIGH-to-LOW transition.
     */
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;

    /* Configure GPIO speed as FAST */
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /* Configure input pull-up */
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;


    /* ---------------------------------------------------------------------- */
    /* ENABLE GPIO CLOCKS                                                     */
    /* ---------------------------------------------------------------------- */

    /* Enable GPIOA clock because LEDs use GPIOA */
    GPIO_PeriClockControl(GPIOA, ENABLE);

    /* Enable GPIOB clock because LEDs use GPIOB */
    GPIO_PeriClockControl(GPIOB, ENABLE);

    /* Enable GPIOC clock because button uses GPIOC */
    GPIO_PeriClockControl(GPIOC, ENABLE);


    /* ---------------------------------------------------------------------- */
    /* INITIALIZE LEDs                                                        */
    /* ---------------------------------------------------------------------- */

    /* Initialize all 10 LED GPIO pins */
    for(int i = 0; i < LED_COUNT; i++)
    {
        GPIO_Init(&GpioLed[i]);
    }


    /* ---------------------------------------------------------------------- */
    /* INITIALIZE BUTTON                                                      */
    /* ---------------------------------------------------------------------- */

    /*
     * GPIO_Init() configures:
     *
     * PC13 as input
     * EXTI13
     * Falling-edge trigger
     * SYSCFG EXTICR
     * EXTI interrupt mask
     */
    GPIO_Init(&GpioBtn);


    /* ---------------------------------------------------------------------- */
    /* INITIAL LED PATTERN                                                    */
    /* ---------------------------------------------------------------------- */

    /*
     * 0x155 = 0101010101
     *
     * LED 0 ON
     * LED 1 OFF
     * LED 2 ON
     * LED 3 OFF
     * LED 4 ON
     * LED 5 OFF
     * LED 6 ON
     * LED 7 OFF
     * LED 8 ON
     * LED 9 OFF
     *
     * Therefore:
     *
     * LED 0,2,4,6,8 are ON.
     */
    leds_write(0x155u);


    /* ---------------------------------------------------------------------- */
    /* NVIC INTERRUPT CONFIGURATION                                           */
    /* ---------------------------------------------------------------------- */

    /*
     * Configure EXTI15_10 interrupt.
     *
     * EXTI lines 10 to 15 share the same interrupt.
     *
     * PC13 therefore uses EXTI15_10.
     *
     * IRQ number = 40.
     *
     * NVIC priority = 15.
     *
     * ENABLE = enable the interrupt.
     */
    GPIO_IRQConfig(
            IRQ_NO_EXTI15_10,
            NVIC_IRQ_PRI15,
            ENABLE
    );


    /* ---------------------------------------------------------------------- */
    /* SUPER LOOP                                                             */
    /* ---------------------------------------------------------------------- */

    /*
     * No button polling is required.
     *
     * The button is handled completely by the interrupt.
     */
    while(1)
    {
        /* Main application can run here */
    }


    /* This statement is never reached */
    return 0;
}


/* -------------------------------------------------------------------------- */
/* EXTI15_10 INTERRUPT SERVICE ROUTINE                                       */
/* -------------------------------------------------------------------------- */

/*
 * EXTI15_10_IRQHandler()
 *
 * This ISR is called whenever an interrupt occurs on EXTI lines 10 to 15.
 *
 * PC13 -> EXTI13 -> EXTI15_10 -> NVIC -> this ISR
 */
void EXTI15_10_IRQHandler(void)
{
    /* Wait for button bouncing to settle */
    delay();


    /*
     * Handle the EXTI13 interrupt.
     *
     * This clears the EXTI pending flag for pin 13.
     */
    GPIO_IRQHandling(GPIO_PIN_NO_13);


    /*
     * Toggle the alternate variable.
     *
     * If alternate = 0:
     *     it becomes 1.
     *
     * If alternate = 1:
     *     it becomes 0.
     */
    alternate = !alternate;


    /* Check which alternate pattern should be displayed */
    if(alternate == 0)
    {
        /*
         * 0x155 = 0101010101
         *
         * Turn ON:
         * LED 0, LED 2, LED 4, LED 6, LED 8
         */
        leds_write(0x155u);
    }
    else
    {
        /*
         * 0x2AA = 1010101010
         *
         * Turn ON:
         * LED 1, LED 3, LED 5, LED 7, LED 9
         */
        leds_write(0x2AAu);
    }
}
