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
  htim3.Init.Prescaler = 0;                                  //PSC(时钟预分频数),不分频
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;               //计数器计数模式，设置为向上计数  
  htim3.Init.Period = (60-1);                                       //ARR(自动重装载寄存器)，累计TIM_Period+1个频率后产生一个更新或者中断
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                //时钟分频因子 ，没用到不用管
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;    //重复计数器的值，没用到不用管
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
  sConfigOC.OCMode = TIM_OCMODE_PWM1;             //输出PWM1模式         
  sConfigOC.Pulse = 0;                            //CCR脉宽值,初始为0
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;     //输出极性,有效电平为高电平
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
    hdma_tim3_ch4_up.Init.Direction = DMA_MEMORY_TO_PERIPH;   //数据传输方向，从内存读取发送到外设  LED_BYTE_Buffer->TIM3_CCR4  
    hdma_tim3_ch4_up.Init.PeriphInc = DMA_PINC_DISABLE;       //TIM3_CCR4(外设)地址寄存器不变
    hdma_tim3_ch4_up.Init.MemInc = DMA_MINC_ENABLE;           //LED_BYTE_Buffer(内存)地址寄存器递增
      
    hdma_tim3_ch4_up.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    //数据宽度为16位
    hdma_tim3_ch4_up.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       //数据宽度为16位
      
    hdma_tim3_ch4_up.Init.Mode = DMA_NORMAL;               // DMA_Mode_Circular(单次)
//      hdma_tim3_ch4_up.Init.Mode = DMA_CIRCULAR;             // DMA_Mode_Circular(循环)
      
//    hdma_tim3_ch4_up.Init.Priority = DMA_PRIORITY_LOW;
      hdma_tim3_ch4_up.Init.Priority = DMA_PRIORITY_MEDIUM;  // 优先级：中
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
                     
       KeyBoardCtrl();   //按键模式切换处理函数
         
       KeyBoardVolumeUp();   //按键音量加处理函数  
         
       KeyBoardVolumeDown();   //按键音量减处理函数 
                
            
                 
       count1 = 0; 
     } 

        /* Upgrade programs*/
        if(UsbiUpdateEepromFlag==2){g_EepromWriteTimeCnt++;}  //0:普通模式  1：升级模式等待数据  2：写入数据 
	//	if(g_EepromWriteTimeCnt%1000==1){printf("Updata... \r\n");} 
		if(g_EepromWriteTimeCnt>1000){g_EepromWriteTimeCnt=0;UsbiUpdateEepromFlag=0;printf("Updata Done. \r\n");}
		
		g_Usart1OverTimeCnt++; //串口1定时器空闲标志位
		g_Usart2OverTimeCnt++; //串口2定时器空闲标志位
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
