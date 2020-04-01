/*
 * Httc.h
 *
 *  Created on: 2019Äê6ÔÂ30ÈÕ
 *      Author: heer
 */

#ifndef SRC_HTTC_H_
#define SRC_HTTC_H_

#include "xstatus.h"
#include "xil_exception.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xil_printf.h"
#include "sleep.h"

int Httc_init_ttc0_timer0(u32 *MatchValue);
int Httc_init_ttc0_timer1(u32 *MatchValue);
XTtcPs *Httc_GetTimerInst(int TTCn,int channel);
void Httc_SetInterval(int TTCn , int channel , u16 value);

#endif /* SRC_HTTC_H_ */
