/**
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  * @file       buzzer_TIM_init.c/h
  * @brief      RoboMaster-C型开发板蜂鸣器的IO、TIM初始化。源码移植自官方2020步兵
  *             开源程序。视移植情况，可能需要重写两个函数的代码。详细操作方法请参
  *             照RoboMaster官方的GitHub开源文档：“RoboMaster开发板C型嵌入式软件教
  *             程文档.pdf”第71页。
  *
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Sep-19-2020     LionHeart       1. done
  *
  @verbatim
  ==============================================================================
  依赖：
	·HAL库：stm32f4xx_hal.h
  ==============================================================================
  @endverbatim
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  */

#include "buzzer_TIM_init.h"

//为避免与CubeMX自动生成的代码重名，这里变更一下命名
TIM_HandleTypeDef re_htim4;

/**
  * @brief          移植自HAL库：HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
  * @param[in]      none
  * @retval         none
  */
HAL_StatusTypeDef TIM_Base_Init_buzzer(TIM_HandleTypeDef *htim);


/**
  * @brief          移植自HAL库：MX_TIM4_Init(void);
  * @param[in]      none
  * @retval         none
  */
void MXY_TIM4_Init(void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	re_htim4.Instance = TIM4;
	re_htim4.Init.Prescaler = 167;
	re_htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	re_htim4.Init.Period = 65535;
	re_htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	re_htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (TIM_Base_Init_buzzer(&re_htim4) != HAL_OK)
	{
		;
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&re_htim4, &sClockSourceConfig) != HAL_OK)
	{
		;
	}
	if (HAL_TIM_PWM_Init(&re_htim4) != HAL_OK)
	{
		;
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&re_htim4, &sMasterConfig) != HAL_OK)
	{
		;
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&re_htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
		;
	}
	TIM_MspPostInit_buzzer(&re_htim4);
}


/**
  * @brief          移植自CubeMX自动生成代码：HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle);
  * @param[in]      none
  * @retval         none
  */
void TIM_Base_MspInit_buzzer(TIM_HandleTypeDef *tim_baseHandle)
{
	if (tim_baseHandle->Instance == TIM4)
	{
		/* TIM4 clock enable */
		__HAL_RCC_TIM4_CLK_ENABLE();
	}
}

/**
  * @brief          移植自CubeMX自动生成代码：HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle);
  * @param[in]      none
  * @retval         none
  */
void TIM_MspPostInit_buzzer(TIM_HandleTypeDef *timHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	if (timHandle->Instance == TIM4)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
		/**TIM4 GPIO Configuration
		PD14     ------> TIM4_CH3
		*/
		GPIO_InitStruct.Pin = BUZZER_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
		HAL_GPIO_Init(BUZZER_GPIO_Port, &GPIO_InitStruct);

		HAL_TIM_Base_Start(&re_htim4);
		HAL_TIM_PWM_Start(&re_htim4, TIM_CHANNEL_3);
	}
}

/**
  * @brief          移植自CubeMX自动生成代码：HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle);
  * @param[in]      none
  * @retval         none
  */
void TIM_Base_MspDeInit_buzzer(TIM_HandleTypeDef *tim_baseHandle)
{
	if (tim_baseHandle->Instance == TIM4)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM4_CLK_DISABLE();
	}
}

/**
  * @brief          移植自HAL库：HAL_TIM_Base_Init(TIM_HandleTypeDef *htim);
  * @param[in]      none
  * @retval         none
  */
HAL_StatusTypeDef TIM_Base_Init_buzzer(TIM_HandleTypeDef *htim)
{
	/* Check the TIM handle allocation */
	if (htim == NULL)
	{
		return HAL_ERROR;
	}

	/* Check the parameters */
	assert_param(IS_TIM_INSTANCE(htim->Instance));
	assert_param(IS_TIM_COUNTER_MODE(htim->Init.CounterMode));
	assert_param(IS_TIM_CLOCKDIVISION_DIV(htim->Init.ClockDivision));
	assert_param(IS_TIM_AUTORELOAD_PRELOAD(htim->Init.AutoReloadPreload));

	if (htim->State == HAL_TIM_STATE_RESET)
	{
		/* Allocate lock resource and initialize it */
		htim->Lock = HAL_UNLOCKED;
		/* Init the low level hardware : GPIO, CLOCK, NVIC */
		TIM_Base_MspInit_buzzer(htim);
		/* USE_HAL_TIM_REGISTER_CALLBACKS */
	}

	/* Set the TIM state */
	htim->State = HAL_TIM_STATE_BUSY;

	/* Set the Time Base configuration */
	TIM_Base_SetConfig(htim->Instance, &htim->Init);

	/* Initialize the TIM state*/
	htim->State = HAL_TIM_STATE_READY;

	return HAL_OK;
}
