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

#ifndef __BUZZER_TIM_INIT_H
#define __BUZZER_TIM_INIT_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"

//RM-C板上蜂鸣器所接的接口
#define BUZZER_Pin        GPIO_PIN_14
#define BUZZER_GPIO_Port  GPIOD

//为避免与CubeMX自动生成的代码重名，这里变更一下命名
extern TIM_HandleTypeDef re_htim4;

/**
  * @brief          移植自HAL库：MX_TIM4_Init(void);
  * @param[in]      none
  * @retval         none
  */
void MXY_TIM4_Init(void);
    
/**
  * @brief          移植自CubeMX自动生成代码：HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle);
  * @param[in]      none
  * @retval         none
  */
void TIM_MspPostInit_buzzer(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif 
