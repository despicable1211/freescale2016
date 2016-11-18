
#include "image_control.h"

#define	IMG_BLACK		0
#define	IMG_WHITE		255
#define FIND_CENTER		0
#define FIND_LEFT		1
#define FIND_RIGHT		2
#define CENTER_POINT	IMAGE_W/2

int8 centerLine[IMAGE_H+1] = {0};			// 最后一个元素用来记录转向点对应的行数
int8 leftLine[IMAGE_H] = {0};
int8 rightLine[IMAGE_H] = {0};
extern uint8 *center;
extern uint8 Motor_stop_flag;
uint8 Left_point[59 ];
uint8 Right_point[59 ];
uint8 midline[59];
uint8  Flag[10]={0};  
int8   error;                                           //偏差
volatile uint8 leftFindFlag;				// 用来标记左黑线是否找到
volatile uint8 rightFindFlag;				// 用来标记右黑线是否找到

volatile int8 leftCount;
volatile int8 rightCount;
volatile int8 findLine;

int8 createPoint(int type, int line);

uint8 *findCenterLine(uint8 (* image)[IMAGE_W])
{
	int8 temp;
	// 前十行从中间往两边查找
	for(findLine = IMAGE_H-1; findLine > IMAGE_H-11; findLine--)
	{
		leftFindFlag = 0;
		rightFindFlag = 0;
		for(temp = 0; temp < CENTER_POINT; temp++)
		{
			// 寻找左黑线
			if(leftFindFlag == 0
			&& image[findLine][CENTER_POINT-temp-1] == IMG_BLACK
			&& image[findLine][CENTER_POINT-temp] == IMG_WHITE
			&& image[findLine][CENTER_POINT-temp+1] == IMG_WHITE
			&& image[findLine][CENTER_POINT-temp+2] == IMG_WHITE)
			{
				leftLine[findLine] = CENTER_POINT-temp-1;
				leftFindFlag = 1;
			}
			// 寻找右黑线
			if(rightFindFlag == 0
			&& image[findLine][CENTER_POINT+temp] == IMG_BLACK
			&& image[findLine][CENTER_POINT+temp-1] == IMG_WHITE
			&& image[findLine][CENTER_POINT+temp-2] == IMG_WHITE
			&& image[findLine][CENTER_POINT+temp-3] == IMG_WHITE)
			{
				rightLine[findLine] = CENTER_POINT+temp;
				rightFindFlag = 1;
			}
			if(leftFindFlag == 1 && rightFindFlag == 1)
				break;
		}
		// 对未找到的黑线进行补全
		if(leftFindFlag == 0)	leftLine[findLine] = 0;
		if(rightFindFlag == 0)	rightLine[findLine] = IMAGE_W-1;
		// 对中线进行赋值
		centerLine[findLine] = (leftLine[findLine]+rightLine[findLine])/2;
		leftFindFlag = 0;
		rightFindFlag = 0;
	}
	
	// 十行后根据前面行位置查找黑线
	for(findLine = IMAGE_H-11; findLine >= 0; findLine--)
	{
		leftFindFlag = 0;
		rightFindFlag = 0;
		// 预测下一行黑线位置
		leftCount = createPoint(FIND_LEFT, findLine);
		rightCount = createPoint(FIND_RIGHT, findLine);
		//leftCount = (2 * leftLine[findLine+1] - leftLine[findLine+2]);
		//rightCount =  (2 * rightLine[findLine+1] - rightLine[findLine+2]);
		
		/* 在预测点的左右 FIND_COUNT 个点查找黑线位置 */
		// 寻找左黑线
		for(temp = 0; temp < FIND_COUNT*2+1; temp++)
		{
			if(leftFindFlag != 0)
				break;
			else if((leftCount-temp+FIND_COUNT)+3 > IMAGE_W-1)
				continue;
			else if((leftCount-temp+FIND_COUNT) < 0)
				break;
			else if(image[findLine][leftCount-temp+FIND_COUNT] == IMG_BLACK
			&& image[findLine][leftCount-temp+FIND_COUNT+1] == IMG_WHITE
			&& image[findLine][leftCount-temp+FIND_COUNT+2] == IMG_WHITE
			&& image[findLine][leftCount-temp+FIND_COUNT+3] == IMG_WHITE)
			{
				leftLine[findLine] = leftCount-temp+FIND_COUNT;
				leftFindFlag = 1;
			}
		}
		// 寻找右黑线
		for(temp = 0; temp < FIND_COUNT*2+1; temp++)
		{
			if(rightFindFlag != 0)
				break;
			else if((rightCount+temp-FIND_COUNT)-3 < 0)
				continue;
			else if(rightCount+temp-FIND_COUNT > IMAGE_W-1)
				break;
			else if(image[findLine][rightCount+temp-FIND_COUNT] == IMG_BLACK
			&& image[findLine][rightCount+temp-FIND_COUNT-1] == IMG_WHITE
			&& image[findLine][rightCount+temp-FIND_COUNT-2] == IMG_WHITE
			&& image[findLine][rightCount+temp-FIND_COUNT-3] == IMG_WHITE)
			{
				rightLine[findLine] = rightCount+temp-FIND_COUNT;
				rightFindFlag = 1;
			}
		}
		
		// 补全未找到的左右黑线
		if(leftFindFlag == 0)
			leftLine[findLine] = leftCount;
		if(rightFindFlag == 0)
			rightLine[findLine] = rightCount;
		
		/* 查找中线 */
#if (FIND_TYPE == TYPE_1)				
		
		// 补全未找到的左右黑线
		// if(leftFindFlag == 0)
		// 	leftLine[findLine] = leftCount;
		// if(rightFindFlag == 0)
		// 	rightLine[findLine] = rightCount;
		
		/* 对中线进行赋值 */
		centerLine[findLine] = (leftLine[findLine]+rightLine[findLine])/2;
		if(centerLine[findLine] <= 0)
		{
			centerLine[findLine] = 0;
			break;
		}
		else if(centerLine[findLine] >= IMAGE_W-1)
		{
			centerLine[findLine] = IMAGE_W-1;
			break;
		}		
#else		
		// 补全未找到的左右黑线
		// if(leftFindFlag == 0)
		// 	leftLine[findLine] = (leftCount < 0) ? 0 : ((leftCount > IMAGE_W-1) ? IMAGE_W-1 : leftCount);
		// if(rightFindFlag == 0)
		// 	rightLine[findLine] = (rightCount < 0) ? 0 : ((rightCount > IMAGE_W-1) ? IMAGE_W-1 : rightCount);
		
		/* 对中线进行赋值 */
		// 左右黑线都存在则取左右黑线中值作为黑线值
		if(leftLine[findLine] > 0 && rightLine[findLine] < IMAGE_W-1)
			centerLine[findLine] = (leftLine[findLine]+rightLine[findLine])/2;
		// 左黑线超出范围
		else if(leftLine[findLine] <= 0 && rightLine[findLine] < IMAGE_W-1)
		{
			// 根据右黑线的偏移量来确定中线
			temp = centerLine[findLine+1] + (rightLine[findLine] - rightLine[findLine+1]);
			// 根据最小二乘法补全中线
			// temp = createPoint(FIND_CENTER, findLine);
			if(temp <= 0)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = 0;
				break;
			}
			else
				centerLine[findLine] = temp;
		}
		// 右黑线超出范围
		else if(leftLine[findLine] > 0 && rightLine[findLine] >= IMAGE_W-1)
		{
			// 根据左黑线的偏移量来确定中线
			temp = centerLine[findLine+1] + (leftLine[findLine] - leftLine[findLine+1]);
			// 根据最小二乘法补全中线
			// temp = createPoint(FIND_CENTER, findLine);
			if(temp >= IMAGE_W-1)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = IMAGE_W-1;
				break;
			}
			else
				centerLine[findLine] = temp;
		}
		// 左右黑线都超出范围
		else
		{
			// 根据最小二乘法补全中线
			temp = createPoint(FIND_CENTER, findLine);
			// 根据中线偏移量补全中线
			// temp = centerLine[findLine+1] + (rightLine[findLine] - rightLine[findLine+1]);
			if(temp <= 0)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = 0;
				break;
			}
			else if(temp >= IMAGE_W-1)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = IMAGE_W-1;
				break;
			}
			else
				centerLine[findLine] = temp;
		}		
#endif
		
		/* 判断十字弯 */
		
		
	}
	if(findLine<0 && centerLine[0]<0)
		centerLine[0] = 0;
	else if(findLine<0 && centerLine[0]>IMAGE_W-1)
		centerLine[0] = IMAGE_W-1;
	
	// 最后一个元素用来记录转向行
	centerLine[IMAGE_H] = (findLine < 0) ? 0 : findLine;	
	
	/*
	// 在 TFT 上绘制出 3 线
	for(temp = IMAGE_H-1; temp >= centerLine[IMAGE_H]; temp--)
	{
		guiDot(3*leftLine[temp], 3*temp, GREEN);
		guiDot(3*rightLine[temp], 3*temp, RED);
		guiDot(3*centerLine[temp], 3*temp, BLUE);
	}
	*/
	
	return (uint8 *)centerLine;
}

/* 利用最小二乘法生成需要补全的点 */
int8 createPoint(int type, int line)
{
	int8 *linePointer;
	uint8 tmp = 0;
	double sumX = 0;
	double sumY = 0;
	double averageX = 0;
	double averageY = 0;
	double sumUp = 0;
	double sumDown = 0;
	double A, B;
	
	if(type == FIND_LEFT)
		linePointer = &leftLine[line];
	else if(type == FIND_RIGHT)
		linePointer = &rightLine[line];
	else
		linePointer = &centerLine[line];
	
	// 取邻近的十个点进行拟合
	while(++tmp <= 10)
	{
		sumX += (line+tmp);
		sumY += linePointer[tmp];
	}
	--tmp;
	averageX = sumX/tmp;
	averageY = sumY/tmp;
	do
	{
		sumUp += (linePointer[tmp]-averageY) * (line+tmp-averageX);
		sumDown += (line+tmp-averageX) * (line+tmp-averageX);
	} while(--tmp > 0);
	
	if(sumDown == 0)
		B = 0;
	else
		B = sumUp/sumDown;
	A = averageY-B*averageX;
	return (int8)(A+B*line+0.5);
}
/*******************************
作用：提取偏差值
*******************************/
int8 get_error()
{
  int8 midle=0,offset=0,midlerow=0;
  if(center[60]==0)                             //中线到图像顶部取偏差
  {
    midlerow=(center[59]+center[0])/2;
    if(((center[0]-center[midlerow])>0&&(center[59]-center[midlerow])>0)||((center[0]-center[midlerow])<0&&(center[59]-center[midlerow])<0))
    {
       offset=(center[0]-39)+(center[30]-39);    //取3点算偏差，顶部，底部，中间
       error=-(center[59]+offset-39);
    }
    else
    {
      offset=center[0]-39;    //取3点算偏差，顶部，底部，中间
      error=-(center[59]+offset-39);
    }
  }
  else                                          //中线不到图像顶部，偏向右时的偏差
  {
    if(center[center[60]]>39)
    {
      error=-(39+center[60]);                   //根据最后结束行算偏差
    }
    else                                        //中线不到图像顶部，偏向左边时的偏差
    {
      error=-(-39-center[60]);                  //根据最后结束行算偏差
    }
  }
    return 0;
}
//停车检测
void cartstopflag(int x)
{
  int r,l,blackflag=80;
  for(r=20;r<x;r++)
  {
    for(l=center[r];l<60;l++)
    {
      if(img[r][l]==0xff) blackflag--;
    }
    for(l=center[r];l>0;l--)
    {
      if(img[r][l]==0xff) blackflag--;
    }
  }
  if(blackflag<20) Motor_stop_flag=1;
}