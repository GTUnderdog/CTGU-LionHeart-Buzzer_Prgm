/**
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  * @file       bsp_buzzer_driver.c/h
  * @brief      蜂鸣器鸣响的驱动程序。视移植情况，需要重写两个函数的代码。详细操作
  *             方法请参照RoboMaster官方的GitHub开源文档：“RoboMaster开发板C型嵌入
  *             式软件教程文档.pdf”第71页。
  *
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Sep-19-2020     LionHeart       1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  */


#include "buzzer_TIM_init.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef re_htim4;

/**
  * @brief          控制蜂鸣器定时器的分频和重载值
  * @param[in]      psc，设置定时器的分频系数
  * @param[in]      pwm，设置定时器的重载值
  * @retval         none
  */
void buzzer_drv_on(uint16_t psc, uint16_t pwm)
{
    __HAL_TIM_PRESCALER(&re_htim4, psc);
    __HAL_TIM_SetCompare(&re_htim4, TIM_CHANNEL_3, pwm);

}

/**
  * @brief          关闭蜂鸣器
  * @param[in]      none
  * @retval         none
  */
void buzzer_drv_off(void)
{
    __HAL_TIM_SetCompare(&re_htim4, TIM_CHANNEL_3, 0);
}
