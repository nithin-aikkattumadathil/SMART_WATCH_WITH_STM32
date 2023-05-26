/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ssd1306.h"
#include "test.h"
#include "battery.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CENTER_Pin GPIO_PIN_13
#define CENTER_GPIO_Port GPIOC
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
uint8_t time[10];
uint8_t date[10];
uint8_t day[10];
uint8_t today_1[10]="Monday";
uint8_t today_2[10]="Tuesday";
uint8_t today_3[10]="Wednesday";
uint8_t today_4[10]="Thursday";
uint8_t today_5[10]="Friday";
uint8_t today_6[10]="Saturday";
uint8_t today_7[10]="Sunday";
uint8_t alarm=0;
uint8_t adc_res=0;
uint8_t adc_val=0;
GPIO_PinState buttonState = GPIO_PIN_RESET;
GPIO_PinState lastButtonState = GPIO_PIN_RESET;
uint32_t buttonPressTime = 0;
int flag = 0;
uint8_t x=1;
uint16_t sensor1_data, sensor2_data, sensor3_data,sensor4_data;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC_Init(void);
uint8_t day_function(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void set_time(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	sTime.Hours = 0x10;
	  sTime.Minutes = 0x35;
	  sTime.Seconds = 0x40;
	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
	    sDate.Month = RTC_MONTH_MAY;
	    sDate.Date = 0x16;
	    sDate.Year = 0x0;

	    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	    {
	      Error_Handler();
	    }
	    HAL_RTCEx_BKUPWrite(&hrtc , RTC_BKP_DR1, 0x32F2);

}


void set_alarm(void)
{
	  RTC_AlarmTypeDef sAlarm = {0};
	sAlarm.AlarmTime.Hours = 0x10;
	  sAlarm.AlarmTime.Minutes = 0x36;
	  sAlarm.AlarmTime.Seconds = 0x00;
	  sAlarm.AlarmTime.SubSeconds = 0x0;
	  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	  sAlarm.AlarmDateWeekDay = 0x18;
	  sAlarm.Alarm = RTC_ALARM_A;
	  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void get_time(void)
{
  RTC_DateTypeDef gDate;
  RTC_TimeTypeDef gTime;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

  /* Display time Format: hh:mm:ss */
  sprintf((char*)time,"%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

  /* Display date Format: mm-dd-yy */
  sprintf((char*)date,"%02d-%02d-%2d",gDate.Date, gDate.Month, 2000 + gDate.Year);  // I like the date first
  sprintf((char*)day,"%02d",gDate.WeekDay);
  day_function();

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	alarm=1;
}
void to_do_on_alarm(void)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1); //LED ON
    SSD1306_Clear ();
    SSD1306_GotoXY (0, 40);
	SSD1306_Puts ("ALARM ON", &Font_11x18, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(5000);
	SSD1306_Clear ();
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
}


uint8_t day_function(void)
{
	if(day[0]==48&&day[1]==49)
	  {
		  memcpy(day,today_1,sizeof(day));

	  }
	  if(day[0]==48&&day[1]==50)
	    {
	  	  memcpy(day,today_2,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==51)
	    {
	  	  memcpy(day,today_3,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==52)
	    {
	  	  memcpy(day,today_4,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==53)
	    {
	  	  memcpy(day,today_5,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==54)
	    {
	  	  memcpy(day,today_6,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==55)
	    {
	  	  memcpy(day,today_7,sizeof(day));

	    }
	  return day;
}

void home_screen(void)
{
	/* USER CODE END WHILE */

					  get_time();
					  SSD1306_GotoXY (15, 25);
					  SSD1306_Puts (time, &Font_11x18, 1);

					  SSD1306_GotoXY (0, 0);
					  SSD1306_Puts (date, &Font_7x10, 1);

					  SSD1306_GotoXY (10, 10);
					  SSD1306_Puts (day, &Font_7x10, 1);
					  SSD1306_UpdateScreen(); //display
					  SSD1306_DrawBitmap(100, 0, bat, 20, 12, 1);
					  HAL_Delay (200);



					 HAL_ADC_Start(&hadc);
					if(  HAL_ADC_PollForConversion(&hadc, 100)==HAL_OK)
					  {
						  adc_res=HAL_ADC_GetValue(&hadc);
						  adc_val=(adc_res*330)/1023;
						  if(adc_res<=60&&adc_res>=20)
						  {
							  SSD1306_GotoXY (102,2);
							  SSD1306_Puts ("25%", &Font_7x10, 0);
							  SSD1306_UpdateScreen();
						  }
						  if(adc_res<=100&&adc_res>=60)
						  {
							  SSD1306_GotoXY (102,2);
							  SSD1306_Puts ("50%", &Font_7x10, 0);
							  SSD1306_UpdateScreen();
						  }
						  if(adc_res<=200&&adc_res>=100)
						  {
							  SSD1306_GotoXY (102,2);
							  SSD1306_Puts ("75%", &Font_7x10, 0);
							  SSD1306_UpdateScreen();
						  }
						 if(adc_res>=200)
						  {
							  SSD1306_GotoXY (102,2);
							  SSD1306_Puts ("100%", &Font_7x10, 0);
							  SSD1306_UpdateScreen();
						  }
					  }

					  if(alarm)
						{
						  to_do_on_alarm();
						  alarm=0;

						}

					 /* USER CODE BEGIN 3 */

}


void up_button(void)
{
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
						{
						SSD1306_Clear ();
						HAL_Delay (10);
						int y=1;
						while(y)
						{
									HAL_ADC_Start(&hadc);
									HAL_ADC_PollForConversion(&hadc, 100);
									sensor1_data = HAL_ADC_GetValue(&hadc);
									sensor1_data=(((sensor1_data*330)/1023)-65580);
									 char buffer[16];
								//	 Display pressure value
								//	SSD1306_Clear();
									//HAL_Delay(100);
									SSD1306_GotoXY(0, 0);
									SSD1306_Puts("                            ", &Font_7x10, 1);
									SSD1306_GotoXY(0, 2);
									SSD1306_Puts(" TEMPERATURE            ", &Font_7x10, 1);
									SSD1306_GotoXY(10, 30);
									sprintf(buffer, "Temp now: %d'C", sensor1_data);
									SSD1306_Puts(buffer, &Font_7x10, 1);
									SSD1306_UpdateScreen();
									HAL_Delay(2000);
									HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1); //LED ON
									HAL_Delay(80);
									HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
															if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
															{
															SSD1306_Clear ();
															HAL_Delay (10);
															int y=1;
															while(y)
															{
																		HAL_ADC_Start(&hadc);
																		HAL_ADC_PollForConversion(&hadc, 100);
																		sensor1_data = HAL_ADC_GetValue(&hadc);
																		sensor1_data=(((sensor1_data*330)/1023)-65580);
																		 char buffer[16];
																	//	 Display pressure value
																	//	SSD1306_Clear();
																		//HAL_Delay(100);
																		SSD1306_GotoXY(0, 0);
																		SSD1306_Puts("                            ", &Font_7x10, 1);
																		SSD1306_GotoXY(0, 2);
																		SSD1306_Puts(" HUMIDITY            ", &Font_7x10, 1);
																		SSD1306_GotoXY(10, 30);
																		sprintf(buffer, "Humidity now: %d'", sensor1_data);
																		SSD1306_Puts(buffer, &Font_7x10, 1);
																		SSD1306_UpdateScreen();
																		HAL_Delay(2000);
																		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1); //LED ON
																		HAL_Delay(80);
																		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
																																	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
																																	{
																																	SSD1306_Clear ();
																																	HAL_Delay (10);
																																	int y=1;
																																	while(y)
																																	{
																																				HAL_ADC_Start(&hadc);
																																				HAL_ADC_PollForConversion(&hadc, 100);
																																				sensor1_data = HAL_ADC_GetValue(&hadc);
																																				sensor1_data=(((sensor1_data*330)/1023)-65580);
																																				 char buffer[16];
																																			//	 Display pressure value
																																			//	SSD1306_Clear();
																																				//HAL_Delay(100);
																																				SSD1306_GotoXY(0, 0);
																																				SSD1306_Puts("                            ", &Font_7x10, 1);
																																				SSD1306_GotoXY(0, 2);
																																				SSD1306_Puts(" PULSE RATE            ", &Font_7x10, 1);
																																				SSD1306_GotoXY(10, 30);
																																				sprintf(buffer, "Pulse Rate now: %d'", sensor1_data);
																																				SSD1306_Puts(buffer, &Font_7x10, 1);
																																				SSD1306_UpdateScreen();
																																				HAL_Delay(2000);
																																				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1); //LED ON
																																				HAL_Delay(80);
																																				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);
																																				                                                           if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3) == GPIO_PIN_RESET)
																																																			{
																																																			SSD1306_Clear ();
																																																			HAL_Delay (10);
																																																			int y=1;
																																																			while(y)
																																																			{
																																																						HAL_ADC_Start(&hadc);
																																																						HAL_ADC_PollForConversion(&hadc, 100);
																																																						sensor1_data = HAL_ADC_GetValue(&hadc);
																																																						sensor1_data=(((sensor1_data*330)/1023)-65580);
																																																						 char buffer[16];
																																																					//	 Display pressure value
																																																					//	SSD1306_Clear();
																																																						//HAL_Delay(100);
																																																						SSD1306_GotoXY(0, 0);
																																																						SSD1306_Puts("                            ", &Font_7x10, 1);
																																																						SSD1306_GotoXY(0, 2);
																																																						SSD1306_Puts(" PRESSURE           ", &Font_7x10, 1);
																																																						SSD1306_GotoXY(10, 30);
																																																						sprintf(buffer, "Pressure now: %d'", sensor1_data);
																																																						SSD1306_Puts(buffer, &Font_7x10, 1);
																																																						SSD1306_UpdateScreen();
																																																						HAL_Delay(2000);
																																																						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,1); //LED ON
																																																						HAL_Delay(80);
																																																						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,0);

																																																						 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
																																																											{
																																																											 SSD1306_Clear();
																																																											 HAL_Delay(100);
																																																											 flag=0;
																																																											 x=0;
																																																											 y=0;
																																																											}
																																																			}
																																																			}
																																				 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
																																									{
																																									 SSD1306_Clear();
																																									 HAL_Delay(100);
																																									 flag=0;
																																									 x=0;
																																									 y=0;
																																									}
																																	}
																																	}
																		 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
																							{
																							 SSD1306_Clear();
																							 HAL_Delay(100);
																							 flag=0;
																							 x=0;
																							 y=0;
																							}
															}
															}
									 if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
														{
														 SSD1306_Clear();
														 HAL_Delay(100);
														 flag=0;
														 x=0;
														 y=0;
														}
						}
						}
}

void settings(void)
{
	SSD1306_Clear ();
	HAL_Delay (100);
	while(x)
	{
	SSD1306_GotoXY (0,0);
	SSD1306_Puts ("                            ", &Font_7x10, 0);
	SSD1306_GotoXY (0,2);
	SSD1306_Puts ("      SETTINGS            ", &Font_7x10, 0);
	SSD1306_GotoXY (0,12);
	SSD1306_Puts (" U-Pulse Rate        ", &Font_7x10, 0);
	SSD1306_GotoXY (0,22);
	SSD1306_Puts (" D-Temperature        ", &Font_7x10, 0);
	SSD1306_GotoXY (0,32);
	SSD1306_Puts (" L-Humidity           ", &Font_7x10, 0);
	SSD1306_GotoXY (0,42);
	SSD1306_Puts (" R-Pressure           ", &Font_7x10, 0);
	SSD1306_UpdateScreen();
	HAL_Delay (500);
	up_button();
	}
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
  MX_RTC_Init();
  MX_I2C1_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */
  SSD1306_Init();

    if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1)!= 0x32F2)
    {
    set_time();
    }
    set_alarm();

  /* USER CODE END 2 */

  /* Infinite loop */



  /* USER CODE BEGIN WHILE */
	while (1)
	{

														    home_screen();
														    buttonState = HAL_GPIO_ReadPin(CENTER_GPIO_Port, CENTER_Pin);

														    if (buttonState != lastButtonState)
														    {
														        if (HAL_GetTick() - buttonPressTime >= 2000) // Button held down for 2 seconds
														        {
														            flag = 1;
														        }
														        lastButtonState = buttonState;
														    }

														    if (flag == 1 && buttonState == GPIO_PIN_RESET && HAL_GetTick() - buttonPressTime >= 2000)
														    {
														        settings();
														    }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_10B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000708;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x10;
  sTime.Minutes = 0x35;
  sTime.Seconds = 0x30;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
  sDate.Month = RTC_MONTH_MAY;
  sDate.Date = 0x18;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x10;
  sAlarm.AlarmTime.Minutes = 0x36;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x16;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : CENTER_PIN_Pin PC3 */
  GPIO_InitStruct.Pin = CENTER_PIN_Pin|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LEFT_Pin PA3 DOWN_Pin */
  GPIO_InitStruct.Pin = LEFT_Pin|GPIO_PIN_3|DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : UP_Pin */
  GPIO_InitStruct.Pin = UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(UP_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
