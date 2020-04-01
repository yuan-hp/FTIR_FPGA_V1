/*
 * H_IRQ.c
 *  放置中断处理函数
 *  Created on: 2019年10月22日
 *      Author: hpy
 */

#include "H_IRQ.h"
#include "xparameters.h"
#include "xil_io.h"
#include "sleep.h"
#include "system.h"


/*
 * 清除指定中断号的中断标志
 *
 */
//static Hclear_irq(u32 irq)
//{
//
//}
/**
 * 激光脉冲中断处理函数
 */
