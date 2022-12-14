/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "tim.h"


/* USER CODE BEGIN 0 */
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch4_up;

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  //f(HZ)=48M/(TIM_Period+1)(TIM_Prescaler+1)       T(S)=1/f;   T(S)=1.25uS;
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;                                  //PSC(ʱ��Ԥ��Ƶ��),����Ƶ
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;               //����������ģʽ������Ϊ���ϼ���  
  htim3.Init.Period = (60-1);                                       //ARR(�Զ���װ�ؼĴ���)���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                //ʱ�ӷ�Ƶ���� ��û�õ����ù�
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;    //�ظ���������ֵ��û�õ����ù�
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;             //���PWM1ģʽ         
  sConfigOC.Pulse = 0;                            //CCR����ֵ,��ʼΪ0
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;     //�������,��Ч��ƽΪ�ߵ�ƽ
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE; 
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 DMA Init */
    /* TIM3_CH4_UP Init */
    hdma_tim3_ch4_up.Instance = DMA1_Channel3;
//    hdma_tim3_ch4_up.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_tim3_ch4_up.Init.Direction = DMA_MEMORY_TO_PERIPH;   //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����  LED_BYTE_Buffer->TIM3_CCR4  
    hdma_tim3_ch4_up.Init.PeriphInc = DMA_PINC_DISABLE;       //TIM3_CCR4(����)��ַ�Ĵ�������
    hdma_tim3_ch4_up.Init.MemInc = DMA_MINC_ENABLE;           //LED_BYTE_Buffer(�ڴ�)��ַ�Ĵ�������
      
    hdma_tim3_ch4_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    //���ݿ��Ϊ16λ
    hdma_tim3_ch4_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       //���ݿ��Ϊ16λ
      
    hdma_tim3_ch4_up.Init.Mode = DMA_NORMAL;               // DMA_Mode_Circular(����)
//      hdma_tim3_ch4_up.Init.Mode = DMA_CIRCULAR;             // DMA_Mode_Circular(ѭ��)
      
//    hdma_tim3_ch4_up.Init.Priority = DMA_PRIORITY_LOW;
      hdma_tim3_ch4_up.Init.Priority = DMA_PRIORITY_MEDIUM;  // ���ȼ�����
    if (HAL_DMA_Init(&hdma_tim3_ch4_up) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(tim_pwmHandle,hdma[TIM_DMA_ID_CC4],hdma_tim3_ch4_up);
    __HAL_LINKDMA(tim_pwmHandle,hdma[TIM_DMA_ID_UPDATE],hdma_tim3_ch4_up);

  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PB1     ------> TIM3_CH4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* tim_pwmHandle)
{

  if(tim_pwmHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 DMA DeInit */
    HAL_DMA_DeInit(tim_pwmHandle->hdma[TIM_DMA_ID_CC4]);
    HAL_DMA_DeInit(tim_pwmHandle->hdma[TIM_DMA_ID_UPDATE]);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE END 0 */

TIM_HandleTypeDef htim15;

/* TIM15 init function */
void MX_TIM15_Init(void)
{

  /* USER CODE BEGIN TIM15_Init 0 */

  /* USER CODE END TIM15_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM15_Init 1 */

  /* USER CODE END TIM15_Init 1 */
  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 479;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 99;
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim15) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM15_Init 2 */

  /* USER CODE END TIM15_Init 2 */

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    //ycx
{
    static volatile uint16_t count1;
    
    if(htim->Instance == TIM15)  //counting time 1ms   
   {
     count1++;
       
     if(count1 >= 10)    //10ms_execute
     {
//       printf("tim15 \r\n");
         
         
      /* Key_Board_Handle  */   
                     
       KeyBoardCtrl();   //����ģʽ�л�������
         
       KeyBoardVolumeUp();   //���������Ӵ�����  
         
       KeyBoardVolumeDown();   //���������������� 
                
            
                 
       count1 = 0; 
     } 

        /* Upgrade programs*/
        if(UsbiUpdateEepromFlag==2){g_EepromWriteTimeCnt++;}  //0:��ͨģʽ  1������ģʽ�ȴ�����  2��д������ 
	//	if(g_EepromWriteTimeCnt%1000==1){printf("Updata... \r\n");} 
		if(g_EepromWriteTimeCnt>1000){g_EepromWriteTimeCnt=0;UsbiUpdateEepromFlag=0;printf("Updata Done. \r\n");}
		
		g_Usart1OverTimeCnt++; //����1��ʱ�����б�־λ
		g_Usart2OverTimeCnt++; //����2��ʱ�����б�־λ
		g_CfgSaveOverTimeCnt++;
        
     
   }
}

//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
//{

//  if(tim_baseHandle->Instance==TIM15)
//  {
//  /* USER CODE BEGIN TIM15_MspInit 0 */

//  /* USER CODE END TIM15_MspInit 0 */
//    /* TIM15 clock enable */
//    __HAL_RCC_TIM15_CLK_ENABLE();

//    /* TIM15 interrupt Init */
//    HAL_NVIC_SetPriority(TIM15_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(TIM15_IRQn);
//  /* USER CODE BEGIN TIM15_MspInit 1 */

//  /* USER CODE END TIM15_MspInit 1 */
//  }
//}

//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
//{

//  if(tim_baseHandle->Instance==TIM15)
//  {
//  /* USER CODE BEGIN TIM15_MspDeInit 0 */

//  /* USER CODE END TIM15_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_TIM15_CLK_DISABLE();

//    /* TIM15 interrupt Deinit */
//    HAL_NVIC_DisableIRQ(TIM15_IRQn);
//  /* USER CODE BEGIN TIM15_MspDeInit 1 */

//  /* USER CODE END TIM15_MspDeInit 1 */
//  }
//}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
