/*
 * Htas5111.h
 *
 *  Created on: 2019年11月18日
 *      Author: hpy
 */


#ifndef SRC_HTAS5111_H_
#define SRC_HTAS5111_H_

//与硬件有关的宏参数
#define HTAS5111_USING_SERIES_MODE 					(0x80000000)  //开启tas5111级联lm12800，兼容代码
#define HTAS5111_SERIES_SET_POLARITY				(0x40000000)  //设置TAS5111级联的输出极性，以保证线圈中国电流方向兼容

#define HTAS5111_SET_RESET							(0x00000080)  //TAS5111RESET映射
#define HTAS5111_SET_SD								(0x00000040)  //TAS5111sd映射



/*********************************************************************
 * TAS5111函数
 ********************************************************************/
void Htas5111_init();
void Htas5111_SeriesMode();
void Htas5111_SeriesModeClose();
void Htas5111_SetReset();
void Htas5111_Reset();
void Htas5111_SetPolar();
void Htas5111_ResetPolar();
void Htas5111_SetSd();
void Htas5111_ResetSd();

#endif /* SRC_HTAS5111_H_ */
