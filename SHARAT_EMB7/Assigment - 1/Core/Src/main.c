/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* Function used to redirect printf() to UART */
int _write(int file, char *ptr, int len);


/**
  * @brief  Main program
  * @retval int
  */
int main(void)
{
    /* Initialize HAL library, Flash and SysTick */
    HAL_Init();

    /* Configure system clock */
    SystemClock_Config();

    /* Initialize GPIO peripherals */
    MX_GPIO_Init();

    /* Initialize USART2 */
    MX_USART2_UART_Init();

    /* Disable stdout buffering so printf() is transmitted immediately */
    setvbuf(stdout, NULL, _IONBF, 0);

    /* Initialize board LED */
    BSP_LED_Init(LED2);

    /* Initialize user push button */
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);



    /*--------------------------------------------------------------------------*/
    /* Search Algorithm                                                        */
    /*--------------------------------------------------------------------------*/

    int array[7] = {1, 2, 3, 4, 5, 6, 7};

    int arr1[4] = {5, 6, 1, 3};

    int key = 6;

    /* Calculate number of elements in array */
    int size = sizeof(array) / sizeof(array[0]);

    /* Perform ternary search */
    int search = ternary(array, size, key);

    /* Calculate number of elements in arr1 */
    int sortsize = sizeof(arr1) / sizeof(arr1[0]);

    /* Sort array using insertion sort */
    insertionsort(arr1, sortsize);


    /*--------------------------------------------------------------------------*/
    /* Prime Number Generation                                                 */
    /*--------------------------------------------------------------------------*/

    int primearray[20] = {0};

    int retcount = 0;

    /* Generate prime numbers */
    PrimeNumber(20, primearray, &retcount);


    /*--------------------------------------------------------------------------*/
    /* Display Results                                                         */
    /*--------------------------------------------------------------------------*/

    printf("Key %d is at %d position\n", key, search);

    printf("Sorted Array\n");

    for (int i = 0; i < sortsize; i++)
    {
        printf("%d\n", arr1[i]);
    }

    printf("Prime Number Array\n");

    for (int i = 0; i < retcount; i++)
    {
        printf("%d\n", primearray[i]);
    }


    /* Infinite loop */
    while (1)
    {

    }
}


/**
  * @brief Redirect printf() output to USART2
  */
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(
        &huart2,
        (uint8_t *)ptr,
        len,
        HAL_MAX_DELAY
    );

    return len;
}


/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable power controller clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Configure voltage scaling */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /* Configure HSI oscillator */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_DEFAULT;

    /* Enable PLL */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

    /* Use HSI as PLL source */
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;

    /* PLL configuration */
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Configure CPU and peripheral clocks */
    RCC_ClkInitStruct.ClockType =
          RCC_CLOCKTYPE_HCLK
        | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1
        | RCC_CLOCKTYPE_PCLK2;

    /* Select PLL as system clock */
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

    /* AHB clock divider */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

    /* APB1 clock divider */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

    /* APB2 clock divider */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(
            &RCC_ClkInitStruct,
            FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief USART2 Initialization Function
  */
static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;

    /* UART baud rate */
    huart2.Init.BaudRate = 115200;

    /* 8-bit data */
    huart2.Init.WordLength = UART_WORDLENGTH_8B;

    /* One stop bit */
    huart2.Init.StopBits = UART_STOPBITS_1;

    /* No parity */
    huart2.Init.Parity = UART_PARITY_NONE;

    /* Enable TX and RX */
    huart2.Init.Mode = UART_MODE_TX_RX;

    /* No hardware flow control */
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    /* 16x oversampling */
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}


/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
    /* Enable GPIO clocks */

    __HAL_RCC_GPIOC_CLK_ENABLE();

    __HAL_RCC_GPIOH_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
}


/**
  * @brief Error Handler
  */
void Error_Handler(void)
{
    /* Disable interrupts */
    __disable_irq();

    /* Stay here if an error occurs */
    while (1)
    {
    }
}


#ifdef USE_FULL_ASSERT

/**
  * @brief Assert failed handler
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add error reporting here */
}

#endif /* USE_FULL_ASSERT */
