/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "i2c-lcd.h"
#include "rc522.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define R1_PORT GPIOA
#define R1_PIN GPIO_PIN_15

#define R2_PORT GPIOB
#define R2_PIN GPIO_PIN_3

#define R3_PORT GPIOB
#define R3_PIN GPIO_PIN_4

#define R4_PORT GPIOB
#define R4_PIN GPIO_PIN_5

#define C1_PORT GPIOB
#define C1_PIN GPIO_PIN_6

#define C2_PORT GPIOB
#define C2_PIN GPIO_PIN_7

#define C3_PORT GPIOB
#define C3_PIN GPIO_PIN_8

#define C4_PORT GPIOB
#define C4_PIN GPIO_PIN_9


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;

osThreadId rfidHandle;
osThreadId keyboardHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C2_Init(void);
void StartRFID(void const * argument);
void StartTask04(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char read_keypad (void)
{
	/* Make ROW 1 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_RESET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High

	if ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0)   // if the Col 1 is low
	{
		while ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0);   // wait till the button is pressed
		return '1';
	}

	if ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN))==0)   // if the Col 2 is low
	{
		while ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN))==0);   // wait till the button is pressed
		return '2';
	}

	if ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0)   // if the Col 3 is low
	{
		while ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0);   // wait till the button is pressed
		return '3';
	}

	if ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0)   // if the Col 4 is low
	{
		while ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0);   // wait till the button is pressed
		return 'A';
	}

	/* Make ROW 2 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_RESET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High

	if ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0)   // if the Col 1 is low
	{
		while ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0);   // wait till the button is pressed
		return '4';
	}

	if ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN))==0)   // if the Col 2 is low
	{
		while ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN))==0);   // wait till the button is pressed
		return '5';
	}

	if ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0)   // if the Col 3 is low
	{
		while ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0);   // wait till the button is pressed
		return '6';
	}

	if ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0)   // if the Col 4 is low
	{
		while ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0);   // wait till the button is pressed
		return 'B';
	}


	/* Make ROW 3 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_RESET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);  // Pull the R4 High

	if ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0)   // if the Col 1 is low
	{
		while ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0);   // wait till the button is pressed
		return '7';
	}

	if ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN))==0)   // if the Col 2 is low
	{
		while ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));   // wait till the button is pressed
		return '8';
	}

	if ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0)   // if the Col 3 is low
	{
		while ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0);   // wait till the button is pressed
		return '9';
	}

	if ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0)   // if the Col 4 is low
	{
		while ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0);   // wait till the button is pressed
		return 'C';
	}


	/* Make ROW 4 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);  //Pull the R1 low
	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);  // Pull the R2 High
	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);  // Pull the R3 High
	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_RESET);  // Pull the R4 High

	if ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0)   // if the Col 1 is low
	{
		while ((HAL_GPIO_ReadPin (C1_PORT, C1_PIN))==0);   // wait till the button is pressed
		return '*';
	}

	if ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN))==0)   // if the Col 2 is low
	{
		while ((HAL_GPIO_ReadPin (C2_PORT, C2_PIN))==0);   // wait till the button is pressed
		return '0';
	}

	if ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0)   // if the Col 3 is low
	{
		while ((HAL_GPIO_ReadPin (C3_PORT, C3_PIN))==0);   // wait till the button is pressed
		return '#';
	}

	if ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0)   // if the Col 4 is low
	{
		while ((HAL_GPIO_ReadPin (C4_PORT, C4_PIN))==0);   // wait till the button is pressed
		return 'D';
	}
}

char* getInput()
{
	int count = 0;
	char *input = (char*)malloc(5*sizeof(char));

	if (input == NULL) {
	        return NULL;
	    }

	while (count < 4) {
			char ch = read_keypad();
			input[count] = ch;
			count++;
		}
	input[count] = '\0';

	return input;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  uint8_t status;
  uint8_t str[MAX_LEN]; // Max_LEN = 16
  uint8_t sNum[5];

  /* USER CODE END 2 */
  MFRC522_Init();
  lcd_init();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of rfid */
  osThreadDef(rfid, StartRFID, osPriorityIdle, 0, 256);
  rfidHandle = osThreadCreate(osThread(rfid), NULL);

  /* definition and creation of keyboard */
  osThreadDef(keyboard, StartTask04, osPriorityIdle, 0, 128);
  keyboardHandle = osThreadCreate(osThread(keyboard), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB12 PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void update1(int data, int field)	// Hàm này còn chưa sửa xong
{
	int size=0;
	char sendBuff[100]={0};
	char sendBuff1[100]={0};
	char sendBuff2[100]={0};
	char sendBuff3[100]={0};
	char sendBuff4[100]={0};

	sprintf(sendBuff, "AT+RST\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff, sizeof(sendBuff), 1000);
	HAL_Delay(8000);
	sprintf(sendBuff1, "AT\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff1, sizeof(sendBuff1), 1000);
	HAL_Delay(1000);

	sprintf(sendBuff2, "AT+CWMODE=1\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff2, sizeof(sendBuff2), 1000);
	HAL_Delay(1000);

	sprintf(sendBuff3, "AT+CWJAP=\"%s\",\"%s\"\r\n", "Redmi Note 8", "123running");
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff3, sizeof(sendBuff3), 1000);
	HAL_Delay(8000);

	sprintf(sendBuff4, "AT+CIPMUX=1\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff4, sizeof(sendBuff4), 1000);
	HAL_Delay(1000);

	sprintf(sendBuff, "AT+CIPSTART=3,\"TCP\",\"184.106.153.149\",80\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff, sizeof(sendBuff), 1000);
	HAL_Delay(1000);
	if (data<10000){
		size=50;
		if (data<1000){
			size=49;
			if (data<100){
				size=48;
				if (data<10){
					size=47;
				}
			}
		}
	}
	sprintf(sendBuff, "GET /update?api_key=6WNEUAHZ46JO8MKA&field%d=%d\r\n", field, data);  //tao du lieu can truyen
	sprintf(sendBuff2, "AT+CIPSEND=3,%d\r\n",size); //tao lenh khai bao do dai du lieu
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff2, sizeof(sendBuff2), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)sendBuff, sizeof(sendBuff), 1000);
	HAL_Delay(1000);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartRFID */
/**
* @brief Function implementing the rfid thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRFID */
void StartRFID(void const * argument)
{
  /* USER CODE BEGIN StartRFID */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartRFID */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
* @brief Function implementing the keyboard thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask04 */
void StartTask04(void const * argument)
{
  /* USER CODE BEGIN StartTask04 */
char inputBuffer[32];
uint8_t inputLength = 0;
  /* Infinite loop */
  for(;;)
  {
	char* input = getInput();
	if (input != NULL) {

		//update1(input , 1);		// Đang chờ sửa hàm update1

		free(input);
	}
    osDelay(5000);
  }
  /* USER CODE END StartTask04 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
