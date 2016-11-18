#ifndef	__IMAGE_CONTROL_H__
#define	__IMAGE_CONTROL_H__

#include "include.h"
//#include "define.h"

#define IMAGE_W                80
#define IMAGE_H                60
#define FIND_COUNT		3				// 定义跟踪搜索的范围
#define TYPE_1			1
#define TYPE_2			2
#define FIND_TYPE		TYPE_2			// 定义中线的查找模式

extern int8 error;
// 传入二维图形数组，返回中线数组
uint8 *findCenterLine(uint8 (* image)[IMAGE_W]);
int8 get_error();
void cartstopflag(int x);
// 传入图像指针，返回中线数组
#define getCenterLine(imagePointer)		findCenterLine((uint8 (*)[IMAGE_W])imagePointer)

#endif