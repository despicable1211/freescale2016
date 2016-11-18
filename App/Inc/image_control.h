#ifndef	__IMAGE_CONTROL_H__
#define	__IMAGE_CONTROL_H__

#include "include.h"
//#include "define.h"

#define IMAGE_W                80
#define IMAGE_H                60
#define FIND_COUNT		3				// ������������ķ�Χ
#define TYPE_1			1
#define TYPE_2			2
#define FIND_TYPE		TYPE_2			// �������ߵĲ���ģʽ

extern int8 error;
// �����άͼ�����飬������������
uint8 *findCenterLine(uint8 (* image)[IMAGE_W]);
int8 get_error();
void cartstopflag(int x);
// ����ͼ��ָ�룬������������
#define getCenterLine(imagePointer)		findCenterLine((uint8 (*)[IMAGE_W])imagePointer)

#endif