/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "robomas_kounai.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
RoboMaster Robomaster[4];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CAN1_Init(void);
static void MX_CAN2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const int MAX_SPEED = 10000;
const float dt = 0.001;
uint32_t id;
uint32_t dlc;
uint8_t data[4] = {127,127,127,127};
uint8_t uart_data[13] = {0};
uint8_t rx_data[8];
float send_data[4] = {0.0,0.0,0.0,0.0};
int16_t int16Data[3];
int16_t angle;
int16_t velocity = 0;
int16_t torque;
int motor;
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
  MX_USART2_UART_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  /* USER CODE BEGIN 2 */
  Robomaster[0].Kp = 30 *0.20;
   //Robomaster[0].Ki = 0;
  Robomaster[0].Ki = Robomaster[0].Kp/(0.83*(1/6.25));
  Robomaster[0].Kd =0.05;
  Robomaster[1].Kp = 30 *0.20;
  Robomaster[1].Ki = Robomaster[0].Kp/(0.83*(1/6.25));
  Robomaster[1].Kd =0.05;
  Robomaster[2].Kp = 30 *0.20;
  Robomaster[2].Ki = Robomaster[0].Kp/(0.83*(1/6.25));
  Robomaster[2].Kd =0.05;
  Robomaster[3].Kp = 30 *0.20;
  Robomaster[3].Ki = Robomaster[0].Kp/(0.83*(1/6.25));
  Robomaster[3].Kd =0.05;
  //D制御のゲインは仮置?��?

  CAN_FilterTypeDef filter1;
  filter1.FilterIdHigh         = 0x001 << 5;                  // フィルターID1
  filter1.FilterIdLow          = 0x002 << 5;                  // フィルターID2
  filter1.FilterMaskIdHigh     = 0x003 << 5;                  // フィルターID3
  filter1.FilterMaskIdLow      = 0x004 << 5;    // フィルターID4
  filter1.FilterScale          = CAN_FILTERSCALE_16BIT; // 16モー?��?
  filter1.FilterFIFOAssignment = CAN_FILTER_FIFO0;      // FIFO0へ格?��?
  filter1.FilterBank           = 0;
  filter1.FilterMode           = CAN_FILTERMODE_IDLIST; // IDリストモー?��?
  filter1.SlaveStartFilterBank = 14;
  filter1.FilterActivation     = ENABLE;

  HAL_CAN_ConfigFilter(&hcan1, &filter1);
  CAN_FilterTypeDef filter2;
  filter2.FilterIdHigh         = 0x001 << 5;                  // フィルターID1
  filter2.FilterIdLow          = 0x002 << 5;                  // フィルターID2
  filter2.FilterMaskIdHigh     = 0x003 << 5;                  // フィルターID3
  filter2.FilterMaskIdLow      = 0x004 << 5;    // フィルターID4
  filter2.FilterScale          = CAN_FILTERSCALE_16BIT; // 16モー?��?
  filter2.FilterFIFOAssignment = CAN_FILTER_FIFO0;      // FIFO0へ格?��?
  filter2.FilterBank           = 0;
  filter2.FilterMode           = CAN_FILTERMODE_IDLIST; // IDリストモー?��?
  filter2.SlaveStartFilterBank = 14;
  filter2.FilterActivation     = ENABLE;

  HAL_CAN_ConfigFilter(&hcan2, &filter2);
  // CANスター?��?
  HAL_CAN_Start(&hcan1);
  // 割り込み有効
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  // CANスター?��?
  HAL_CAN_Start(&hcan2);
  // 割り込み有効
  HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
  /* USER CODE END 2 */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 5;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 5;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_3TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan1) {
	CAN_RxHeaderTypeDef RxHeader;//受信メ?��?セージの?��?報が�??��納されるインスタンス
	uint8_t RxData[8];//受信した?��?ータを�?時保存する�??��?��??
	if (HAL_CAN_GetRxMessage(hcan1, CAN_RX_FIFO0, &RxHeader, RxData)== HAL_OK) {
		if(hcan1->Instance == CAN1){
			for(int j=0;j<=8;j++){
				mokuhyou[j]=RxData[j];
			}
		}
	}
}*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[16] = {0};
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {
          if(hcan->Instance == hcan1.Instance){
        	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
              id = (RxHeader.IDE == CAN_ID_STD)? RxHeader.StdId : RxHeader.ExtId;     // ID
              dlc = RxHeader.DLC;                                                     // DLC

              uart_data[0] = '!';
              HAL_UART_Transmit(&huart2, &uart_data[0], 1, 1);
              for(int i = 0; i < 12; i++){
              	uart_data[i + 1] = RxData[i + 4];
              	HAL_UART_Transmit(&huart2, &uart_data[i], 1, 1);
              }

              for(int i = 0;i < 4;i++){
            	  data[i] = RxData[i];
            	  Robomaster[i].target = ((data[i]-127) / 127.0 * MAX_SPEED) * -1;
			  }
                  motor = RxData[4];
              if (motor != 0){
            	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
            	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, motor);
              }else{
            	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
            	  __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
              }

          }else if(hcan->Instance == hcan2.Instance){
        	  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);ã�?��??��ã‚Œã?��?©ã?��?®LEDã�?��??��ã‚ã�?��??��ã‚�??��?????  ?  ??  ?  ???  ?  ??  ?  ????  ?  ??  ?  ???  ?  ??  ?  ?
              id = (RxHeader.IDE == CAN_ID_STD)? RxHeader.StdId : RxHeader.ExtId;     // ID
              dlc = RxHeader.DLC;
              for(int i = 0; i < 4; i++) {
                  int16Data[i] = (RxData[i*2] << 8) | RxData[i*2+1];
              }

              // IDã�?��??��ã?��?¨ã?��?«????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½? ?
              switch(id) {
                  case 0x201:
                      Robomaster[0].angle    = (int)int16Data[0];
                      Robomaster[0].velocity = (int)int16Data[1];
                      Robomaster[0].torque   = (int)int16Data[2];
                      break;

                  case 0x202:
                      Robomaster[1].angle    = (int)int16Data[0];
                      Robomaster[1].velocity = (int)int16Data[1];
                      Robomaster[1].torque   = (int)int16Data[2];
                      break;

                  case 0x203:
                      Robomaster[2].angle    = (int)int16Data[0];
                      Robomaster[2].velocity = (int)int16Data[1];
                      Robomaster[2].torque   = (int)int16Data[2];
                      break;

                  case 0x204:
                      Robomaster[3].angle    = (int)int16Data[0];
                      Robomaster[3].velocity = (int)int16Data[1];
                      Robomaster[3].torque   = (int)int16Data[2];
                      break;

                  default:
                      // ????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½? ?????  ?  ??  ?  ???  ?  ??  ?  IDã?��?¯ç„¡????  ?  ??  ?  ???  ?  ??  ?  ?
                      break;
              }
          }
      }


}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim == &htim7){
	    for(int i = 0;i<4;i++){
	        float error = Robomaster[i].target - Robomaster[i].velocity;
	        // ????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½? ?????  ?  ??  ?  ???  ?  ??  ?  ?ã?��?®æ›´æ–°
	        Robomaster[i].integral += error * dt;
	        // å?��?¶å¾¡ä¿¡å·ã?��?®????¿½?¿½??¿½?¿½???¿½?¿½??¿½?¿½? ?
	        Robomaster[i].derivative += error / dt;
	        //PI?��?けじ?��?なくてDも�??��れてPID制御をしてみた�??
	        send_data[i] = Robomaster[i].Kp * error + Robomaster[i].Ki * Robomaster[i].integral + Robomaster[i].Kd * Robomaster[i].derivative;
	        if (send_data[i] > 10000){
	        	send_data[i] = 10000;
	        }
	        else if (send_data[i] < -10000){
	        	send_data[i] = -10000;
	        }
	    }
		uint32_t TxMailbox;
		CAN_TxHeaderTypeDef TxHeader;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.StdId = 0x200;  //robomas motor rotation ID
		TxHeader.DLC = 8;
		TxHeader.RTR = CAN_RTR_DATA;
    	robomas robomas(&hcan2,&TxHeader,&TxMailbox);
	    robomas.rotate(send_data);
    }
}


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
