/*
 * Htas5111.h
 *
 *  Created on: 2019��11��18��
 *      Author: hpy
 */

#ifndef SRC_HTAS5111_H_
#define SRC_HTAS5111_H_

//��Ӳ���йصĺ����
#define HTAS5111_USING_SERIES_MODE 					(0x80000000)  //����tas5111����lm12800�����ݴ���
#define HTAS5111_SERIES_SET_POLARITY				(0x40000000)  //����TAS5111������������ԣ��Ա�֤��Ȧ�й������������

#define HTAS5111_SET_RESET							(0x00000080)  //TAS5111RESETӳ��
#define HTAS5111_SET_SD								(0x00000040)  //TAS5111sdӳ��



/*********************************************************************
 * TAS5111����
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