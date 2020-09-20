/**
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  * @file       sound_effects.c/h
  * @brief      实现蜂鸣器鸣响效果音的功能。程序内置了12种蜂鸣器效果音，可由用户简
  *             单调用。在机器人开/关功能、状态切换、识别目标等时鸣响对应的效果音，
  *             可在日常研发中辅助开发人员识别机器人状态、判断程序的运行进度，以及
  *             在赛场备赛中辅助队员快速调整机器人。
  *             本程序调用了RoboMaster-C型开发板的板载蜂鸣器，程序简洁、轻便，占用
  *             芯片资源极少，适合用于各种开发场合。
  *
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Sep-19-2020     LionHeart       1. done
  *
  @verbatim
  ==============================================================================
  使用说明：

	1.任务维护：
		使用FreeRTOS维护任务函数：buzzer_effects_task(void const *argument)，保证
		该任务得到较高的优先级。
		需要包含头文件：#include "sound_effects_task.h" 来索引上述函数。

	2.功能调用（举例）：
		假设在另一源文件“xxx.c"中需要触发蜂鸣器音效，则首先需要包含头文件：
			#include "sound_effects_task.h"
		然后需要创建一指针，使其指向sound_effects_task中的buzzer_control，此处直接
		调用函数 get_buzzer_effect_point() 来获取地址，例如：
			buzzer_t *buzzer = get_buzzer_effect_point();
		若需要在程序进行到某阶段时，触发指定的音效，则直接操作指针的sound_effect即
		可。以触发“系统启动音效”为例：
			......
			buzzer->sound_effect = SYSTEM_START_BEEP;
			......
		或：
			......
			if (*(buzzer->is_busy) == FALSE)
				buzzer->sound_effect = SYSTEM_START_BEEP;
			......
		若其他任务中调用蜂鸣器，与此任务产生冲突，导致蜂鸣器音效不正常，可通过操作：
			buzzer->work = FALSE;
		来停用蜂鸣器音效操作。此时，蜂鸣器仍会完成当前正在鸣响的音效，然后才会停止。
		有关各种音效的说明，详见sound_effects_task.h中的sound_effects_t枚举类型。

	3.环境配置：
		本程序还包含buzzer_TIM_init.c/h和bsp_buzzer_driver.c/h两组文件。这些文件皆
		移植自RM2020官方步兵开源程序。本程序依赖HAL库“stm32f4xx_hal.c/h”工作，请确
		保移植的目标工程使用HAL库，否则需要重新设计buzzer_init.c/h和bsp_buzzer.c/h
		中的部分函数。也可由HAL直接产生初始化代码，详细操作方法请参照RoboMaster官方
		的GitHub开源文档：“RoboMaster开发板C型嵌入式软件教程文档.pdf”第71页。
		以RM2020官方开源程序为例，若要将此程序移植进RM2020官方步兵程序，则需要：
			·将源文件和头文件复制到工程目录下，并在工程中添加源文件
			·在freertos.c中包含头文件：#include "sound_effects_task.h"
			·创建蜂鸣器音效任务：
				osThreadDef(buzr, buzzer_effects_task, osPriorityNormal, 0, 128);
				testHandle = osThreadCreate(osThread(buzr), NULL);
			·在test_task.c中引入头文件，并在任务中合适的位置上添加：
				buzzer_t *buzzer = get_buzzer_effect_point();
				......
				buzzer->work = FALSE;
				......
				buzzer->work = TRUE;
			 以消除官方代码中的模块离线提示音与任务冲突造成的音效异常（其实不操作也
			 没有大问题，只是声音难听一点而已）
			·按照需求，在其他源文件中执行功能调用（上述）步骤。
		移植进其他工程中，则需要根据具体情况自行做出调整。
  ==============================================================================
  @endverbatim
  *************************(C) COPYRIGHT 2020 LionHeart*************************
  */

#include "sound_effects_task.h"

buzzer_t buzzer_control;
bool_check_t buzzer_is_busy;

/**
  * @brief          返回buzzer_is_busy指针
  * @param[in]      none
  * @retval         bool_check_t *buzzer_is_busy
  */
const bool_check_t *get_buzzer_is_busy_point(void);


/**
  * @brief          蜂鸣器音效任务，间隔 BUZZER_TASK_CONTROL_TIME 10ms
  * @param[in]      pvParameters: 空
  * @retval         none
  */
void buzzer_effects_task(void const *argument)
{
	//初始化标志位
	buzzer_control.is_busy = get_buzzer_is_busy_point();
	buzzer_is_busy = FALSE;   
	buzzer_control.work = TRUE; 
	buzzer_control.sound_effect = STOP;  

	//等待机器人其他模块初始化完成
	osDelay(500);
	//初始化TIM4，为蜂鸣器服务
	MXY_TIM4_Init();
	//关闭蜂鸣器
	buzzer_drv_off();
	//鸣响一次“开机启动”音效，若不需要在此使用，请将本句注释
	buzzer_control.sound_effect = SYSTEM_START_BEEP;

	for (;;)
	{
		//检查工作标志是否置位
		if (buzzer_control.work == TRUE)
		{
			switch (buzzer_control.sound_effect)
			{
			case STOP:
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			case SYSTEM_START_BEEP:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(3, 10000);	osDelay(333);
				buzzer_drv_on(2, 10000);	osDelay(333);
				buzzer_drv_on(1, 10000);	osDelay(333);
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			case B_:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			case B_B_:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			case B_B_B_:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			case B___:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(1, 10000);	osDelay(500);
				buzzer_is_busy = FALSE;
				break;

			case B_CONTINUE:
				buzzer_is_busy = TRUE;
				buzzer_drv_on(1, 10000);	osDelay(100);
				buzzer_drv_off();           osDelay(50);
				break;

			case D_:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(4, 10000);	osDelay(70);
				buzzer_is_busy = FALSE;
				break;

			case D_D_:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(4, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(4, 10000);	osDelay(70);
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			case D_D_D_:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(4, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(4, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(4, 10000);	osDelay(70);
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			case D___:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(4, 10000);    osDelay(500);
				buzzer_is_busy = FALSE;
				break;

			case D_CONTINUE:
				buzzer_is_busy = TRUE;
				buzzer_drv_on(4, 10000);    osDelay(100);
				buzzer_drv_off();           osDelay(50);
				break;

			case D_B_B_:
				buzzer_control.sound_effect = STOP;
				buzzer_is_busy = TRUE;
				buzzer_drv_on(4, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();			osDelay(70);
				buzzer_drv_on(1, 10000);	osDelay(70);
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;

			default:
				buzzer_control.sound_effect = STOP;
				buzzer_drv_off();
				buzzer_is_busy = FALSE;
				break;
			}
		}
		else if (buzzer_control.sound_effect != STOP)
		{
			buzzer_control.sound_effect = STOP;
			buzzer_drv_off();
			buzzer_is_busy = FALSE;
		}
		osDelay(BUZZER_TASK_CONTROL_TIME);  //任务间隔10ms
	}
}


/**
  * @brief          返回蜂鸣器任务数据指针
  * @param[in]      none
  * @retval         buzzer_t *buzzer_control
  */
buzzer_t *get_buzzer_effect_point(void)
{
	return &buzzer_control;
}

/**
  * @brief          返回buzzer_is_busy指针
  * @param[in]      none
  * @retval         bool_check_t *buzzer_is_busy
  */
const bool_check_t *get_buzzer_is_busy_point(void)
{
	return &buzzer_is_busy;
}
