/**
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  * @file       bsp_buzzer_driver.c/h
  * @brief      蜂鸣器鸣响的驱动程序。视移植情况，可能需要重写两个函数的代码。详细
  *             操作方法请参照RoboMaster官方的GitHub开源文档：“RoboMaster开发板C型
  *             嵌入式软件教程文档.pdf”第71页。
  *
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Sep-19-2020     LionHeart       1. done
  *
  @verbatim
  ==============================================================================
  依赖：
	·数据类型定义：struct_typedef.h
  ==============================================================================
  @endverbatim
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  */

#ifndef __BSP_BUZZER_DRIVER_H
#define __BSP_BUZZER_DRIVER_H

#include "struct_typedef.h"


/**
 * @brief          控制蜂鸣器定时器的分频和重载值
 * @param[in]      psc，设置定时器的分频系数
 * @param[in]      pwm，设置定时器的重载值
 * @retval         none
 */
extern void buzzer_drv_on(uint16_t psc, uint16_t pwm);

/**
  * @brief          关闭蜂鸣器
  * @param[in]      none
  * @retval         none
  */
extern void buzzer_drv_off(void);

#endif
