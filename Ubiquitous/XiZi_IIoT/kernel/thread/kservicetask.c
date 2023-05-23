
/*
* Copyright (c) 2020 AIIT XUOS Lab
* XiUOS is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*        http://license.coscl.org.cn/MulanPSL2
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
* See the Mulan PSL v2 for more details.
*/

/**
* @file:    kservicetask.c
* @brief:   create service task for system 
* @version: 1.0
* @author:  AIIT XUOS Lab
* @date:    2020/3/15
*
*/

#include <xizi.h>
// #include <connect_lcd.h>
// #include <transform.h>

extern void ZombieTaskRecycleInit(void);
extern void InitIdleKTask(void);
void KSerciveKTaskIdle(void)
{
    InitIdleKTask();
}

void KServiceKTaskRecycle()
{
    ZombieTaskRecycleInit();
}

// void KTaskProgressBar(void *args)
// {
//     DrvLcdClear(0xFFFF);
//     int loadStatue=84-1;

//     while (1)
//     {          
//         // if(loadStatue > 148+84+2) break;
        
//         if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//         if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//        // PrivTaskDelay(250);
//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);

//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
        
//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//     //    PrivTaskDelay(250);

//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);

//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);
//          if(loadStatue++<=148+84+2)LcdDrawLine(loadStatue,190,loadStatue,206,0x0000);

//         printf("%d ", loadStatue);
//     }
    
// }

void CreateKServiceKTask(void)
{
    /* create zombie recycle task */
    KServiceKTaskRecycle();

    /* create idle task */
    KSerciveKTaskIdle();

    /**
     * @author: Detcher
     * @description: create a real-time progress bar
     * @details: Like i guess, after init the kernel queue and complete "SysInitOsAssign();",
     *           here fullfil the premise of creating and inserting a task to the kernel queue,
     *           thus create a "progress bar" task and terminate until all things done ("StartupOsAssign();").
    */
    // int32 taskid = KTaskCreate("KTaskProgressBar", KTaskProgressBar, NULL, 4096, 25);
    // StartupKTask(taskid);
}

