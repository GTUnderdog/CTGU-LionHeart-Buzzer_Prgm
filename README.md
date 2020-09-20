# 一、程序简述：
为RoboMaster机器人写的简易蜂鸣器程序，可通过蜂鸣音向战队队员汇报机器人状态。

RoboMaster机器人通常是远程遥控操作，且要求嵌入式控制系统具有较高的效率、稳定性、强健性。因此机器人通常不会安装过多交互器件，最常用的即为LED灯和蜂鸣器。而在机器人的日常研发、调试中，机器人总会出现各种问题。机器人马力大速度快，在很多测试中不易插线调试；复杂的系统又增加了debug的难度。因此，我们需要一套简单、高效的交互方案，能在多种测试场合辅助参赛队员快速定位问题、理顺排查思路，同时又要占用系统资源少、不影响系统的速度与稳定性。于是我们选择了RoboMaster-C板板载蜂鸣器作为交互硬件，使用不同频率、不同音色的蜂鸣音来指示机器人的运行状态，帮助参赛队员在较远距离、不使用其他设备的情况下快速了解机器人的信息。


# 二、程序特点：
+ 由RTOS分出一个线程独立维护，不影响其他任务的运行；
+ 程序代码轻量，原理简单，不占用系统资源；
+ 具有十二种预置效果音，可灵活适配多种调试场景；
+ 大量使用指针传递参数，效率高、封闭性好。
  
  
# 三、源文件说明：
1. `sound_effects_task.c/h`
：存放着需要用系统维护的任务函数，其中包含各种音效的实现逻辑；还有调用任务功能的结构体成员、指针传递函数等。需要二次开发、增加新音效等，需要修改此文件。
2. `bsp_buzzer_driver.c/h`
：存放着RoboMaster-C板板载蜂鸣器的驱动函数。在一些移植情况下，可能需要修改此文件中的部分代码。
3. `buzzer_TIM_init.c/h`
：存放着RoboMaster-C板有关板载陀螺仪的接口、TIM初始化函数。在一些移植情况下，可能需要修改此文件中的部分代码。
  
  
# 四、使用方法说明：
1. 任务维护：
使用FreeRTOS维护任务函数：buzzer_effects_task(void const *argument)，保证该任务得到较高的优先级。

需要包含头文件：
`#include "sound_effects_task.h" `
来索引上述函数。


2. 功能调用（举例）：
假设在另一源文件“xxx.c"中需要触发蜂鸣器音效，则首先需要包含头文件：
`#include "sound_effects_task.h"`

然后需要创建一指针，使其指向sound_effects_task中的buzzer_control，此处直接调用函数
`get_buzzer_effect_point() `
来获取地址，例如：

		`buzzer_t *buzzer = get_buzzer_effect_point();`
若需要在程序进行到某阶段时，触发指定的音效，则直接操作指针的sound_effect即可。以触发“系统启动音效”为例：

```
	......
	buzzer->sound_effect = SYSTEM_START_BEEP;
	......
```

	或：
	
```
		......
		if (*(buzzer->is_busy) == FALSE)
			buzzer->sound_effect = SYSTEM_START_BEEP;
		......
```

若其他任务中调用蜂鸣器，与此任务产生冲突，导致蜂鸣器音效不正常，可通过操作：
`buzzer->work = FALSE;`
来停用蜂鸣器音效操作。此时，蜂鸣器仍会完成当前正在鸣响的音效，然后才会停止。
  
有关各种音效的说明，详见sound_effects_task.h中的sound_effects_t枚举类型。
    
# 五、使用实例：
本程序的文件皆移植自RM2020官方步兵开源程序。本程序依赖HAL库“stm32f4xx_hal”工作，请确保移植的目标工程使用HAL库，否则需要重新设计buzzer_TIM_init.c/h和bsp_buzzer_driver.c/h中的部分函数。也可由HAL直接产生初始化代码，详细操作方法请参照RoboMaster官方的GitHub开源文档：“RoboMaster开发板C型嵌入式软件教程文档.pdf”第71页。

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
        以消除官方代码中的模块离线提示音与任务冲突造成的音效异常（其实不操作也没有大问题，只是声音难听一点而已）
			·按照需求，在其他源文件中执行功能调用（上述）步骤。移植进其他工程中，则需要根据具体情况自行做出调整。

