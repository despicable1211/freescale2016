#include    "common.h"
#include    "FLYFISH_OLED.h"

#include    "FLYFISH_OLEDFONT.h"


u8 OLED_GRAM[128][8];


//***************************************************************************//
//                                                                           //
//                              OLED初始化函数                               //
//                                                                           //
//***************************************************************************//


/*********************************************************************
Function:      OLED_Init();
Description:   OLED初始化函数
Input:		   无
Output:	       无
*********************************************************************/
void OLED_Init()
{
    gpio_init(PTD10,GPO,1);
    gpio_init(PTD11,GPO,1);
    gpio_init(PTD12,GPO,1);
    gpio_init(PTD13,GPO,1);
    gpio_init(PTD14,GPO,1);

    OLED_RST_Set();
    DELAY_MS(100);
    OLED_RST_Clr();
    DELAY_MS(100);
    OLED_RST_Set();


	OLED_Write(0xAE,OLED_CMD); //关闭显示
	OLED_Write(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_Write(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_Write(0xA8,OLED_CMD); //设置驱动路数
	OLED_Write(0X3F,OLED_CMD); //默认0X3F(1/64)
	OLED_Write(0xD3,OLED_CMD); //设置显示偏移
	OLED_Write(0X00,OLED_CMD); //默认为0

	OLED_Write(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.

	OLED_Write(0x8D,OLED_CMD); //电荷泵设置
	OLED_Write(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_Write(0x20,OLED_CMD); //设置内存地址模式
	OLED_Write(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_Write(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_Write(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_Write(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_Write(0x12,OLED_CMD); //[5:4]配置

	OLED_Write(0x81,OLED_CMD); //对比度设置
	OLED_Write(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_Write(0xD9,OLED_CMD); //设置预充电周期
	OLED_Write(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Write(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_Write(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_Write(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_Write(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示
	OLED_Write(0xAF,OLED_CMD); //开启显示

	OLED_Clear();
}


//***************************************************************************//
//                                                                           //
//                               OLED底层函数                                //
//                                                                           //
//***************************************************************************//


/*********************************************************************
Function:      OLED_Write(u8 Data,u8 Cmd);
Description:   OLED写字节函数
Input:		   Data，Cmd
Output:	       无
*********************************************************************/
void OLED_Write(u8 Data,u8 Cmd)
{
	u8 i;
	if(Cmd)
	  OLED_DC_Set();
	else
	  OLED_DC_Clr();
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();
		if(Data&0x80)
		   OLED_SDIN_Set();
		else
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		Data<<=1;
	}
	OLED_CS_Set();
	OLED_DC_Set();
}

/*********************************************************************
Function:      OLED_Refresh_Gram(void);
Description:   更新GRAM到显存
Input:		   无
Output:		   无
*********************************************************************/
void OLED_Refresh_Gram(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		LED_WrCmd(0xb0+i);    //设置页地址（0~7）
		LED_WrCmd(0x00);      //设置显示位置—列低地址
		LED_WrCmd(0x10);      //设置显示位置—列高地址
		for(n=0;n<128;n++)
			LED_WrDat(OLED_GRAM[n][i]);
	}
}


/*********************************************************************
Function:      OLED_Clear(void);
Description:   清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
Input:		   无
Output:		   无
*********************************************************************/
void OLED_Clear()
{
	u8 i,n;
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;

	OLED_Refresh_Gram();//更新显示
}


/*********************************************************************
Function:      OLED_Clear(void);
Description:   填充函数，点亮整个屏幕
Input:		   无
Output:		   无
*********************************************************************/
void OLED_Fill()
{
	u8 i,n;
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0Xff;

	OLED_Refresh_Gram();//更新显示
}


/*********************************************************************
Function:      OLED_POW(u8 m,u8 n);
Description:   计算m^n
Input:		   m：底数  n：幂
Output:		   m^n
*********************************************************************/
u32 OLED_POW(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}


/*********************************************************************
Function:      OLED_DrawPoint(u8 x,u8 y,u8 t);
Description:   画点函数，点亮屏幕上某个点
Input:		   x,y,t
					x:0~127
					y:0~63
					t:1 填充 0,清空
Output:			无
*********************************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;//确定页数，因为oled每次都是从最后一行开始写入的，所以用 7-y/8
	bx=y%8;
	temp=1<<(7-bx);//确定相应点在所属页的行位置
	if(t)
		OLED_GRAM[x][pos]|=temp;
	else
		OLED_GRAM[x][pos]&=~temp;
}


//===========================================================================//
//                                                                           //
//                             英文和数字的显示                              //
//                                                                           //
//===========================================================================//


/*********************************************************************
Function:      OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
Description:   在指定位置显示一个字符,包括部分字符，在" "到"~"之间
Input:		   x,y,chr,size,mode
					x：0~127 起点坐标
					y：0~63
					char：一个字库中的相应字符
               size：选择字体大小 12/16/24
					mode:0,反白显示;1,正常显示
Output:			无
*********************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);	//得到字体一个字符对应点阵集所占的字节数....奇葩公式...
	chr=chr-' ';//得到偏移后的值
    for(t=0;t<csize;t++)//每个字节要写入一次数据，故此为总写入次数
    {
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)	//由于for循环的存在，每当纵向写入8位后就自动结束
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)	//由于size，每次纵向写入到12位后自动结束（如果是1206的话）
			{
				y=y0;	//将纵坐标清为初始值
				x++;	//将横坐标加一，故写入方式为一列一列先
				break;
			}
		}
    }
}


/*********************************************************************
Function:      OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode);
Description:   在指定位置显示数字
Input:		   x,y,num,len,size,mode
					x：0~127 起点坐标
					y：0~63
					num：数值(0~4294967295)
					len：数字长度
               size：选择字体大小 12/16/24
					mode:0,反白显示;1,正常显示
Output:			相应数字
*********************************************************************/
void OLED_ShowNum(u8 x,u8 y,int32 num,u8 len,u8 size,u8 mode)
{
	u8 t;
	if(num < 0)
	{
		OLED_ShowChar(x,y,'-',size,mode);
		x+=(size/2);
        num=abs(num);
	}
	for(t=len;t>0;t--)
	{
		OLED_ShowChar(x+(size/2)*(len-t),y,((num/OLED_POW(10,t-1))%10)+'0',size,mode);
	}
}


/*********************************************************************
Function:      OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
Description:   在指定位置显示字符串
Input:		   x,y,*p,size
					x：0~127 起点坐标
					y：0~63
					*p：字符串起始地址
               size：选择字体大小 12/16/24
                mode:0,反白显示;1,正常显示
Output:			无
*********************************************************************/
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {
        if(x>(128-(size/2))){x=0;y+=size;}	//当一行的字符串最后一个字符位不够放下一个字符时就换行到下一行显示
        if(y>(64-size)){y=x=0;OLED_Clear();}	//当一列的字符位最后一个字符位不够放下一个字符时，就将坐标清零，然后整体清屏，然后再以(0,0)为坐标进行显示
        OLED_ShowChar(x,y,*p,size,mode);
        x+=size/2;	//每写完一个字符，坐标就加一个字符宽度
        p++;
    }
}


//===========================================================================//
//                                                                           //
//                               汉字的显示                                  //
//                                                                           //
//===========================================================================//


/*********************************************************************
Function:      OLED_ShowHanzi(u8 x,u8 y,u8 num,u8 size,u8 mode);      PS.此处的Hanzi就是英文~
Description:   在指定位置显示字汉字
Input:		   x,y,num,size,mode
					x：0~127 起点坐标
					y：0~63
					num：在oledfont.h中自己定义的汉字数组标号
               size：选择字体大小 16
					mode:0,反白显示;1,正常显示
Output:			无
*********************************************************************/
void OLED_ShowHanzi(u8 x,u8 y,u8 num,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size);	//得到字体一个字符对应点阵集所占的字节数....妈的！这是大神公式啊！可以可以！
	for(t=0;t<csize;t++)//每个字节要写入一次数据，故此为总写入次数
	{
		if(size==16)temp=hanzi_1616[num][t]; 	 	//调用1616字体
		else return;								//没有的字库
		  for(t1=0;t1<8;t1++)	//由于for循环的存在，每当纵向写入8位后就自动结束
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)	//由于size，每次纵向写入到12位后自动结束（如果是1206的话）
			{
				y=y0;	//将纵坐标清为初始值
				x++;	//将横坐标加一，故写入方式为一列一列先
				break;
			}
		}
	}
}


/*********************************************************************
Function:     OLED_ShowSentence(u8 x,u8 y,u8 sta,u8 end,u8 size);
Description:   在指定位置显示汉字句子
Input:		   x,y,sta,end,size
					x：0~127 起点坐标
					y：0~63
					sta：汉字句子起始位置
					end：汉字句子终止位置
               size：选择字体大小 16
Output:			无
*********************************************************************/
void OLED_ShowSentence(u8 x,u8 y,u8 sta,u8 end,u8 size)
{
	for(;sta<end+1;sta++)
	{
		if(x>(128-(size))){x=0;y+=size;}	//当一行的句子最后一个汉字位不够放下一个汉字时就换行到下一行显示
		if(y>(64-size)){y=x=0;OLED_Clear();}	//当一列的汉字位最后一个汉字位不够放下一个汉字时，就将坐标清零，然后整体清屏，然后再以(0,0)为坐标进行显示
		OLED_ShowHanzi(x,y,sta,size,1);
		x+=size;	//每写完一个字符，坐标就加一个汉字宽度
	}
}


//===========================================================================//
//                                                                           //
//                               位图的显示                                  //
//                                                                           //
//===========================================================================//


/*********************************************************************
Function:      OLED_DrawBMP(u8 x,u8 y,u8 length,u8 height,u8 mode);
Description:   在指定坐标开始显示相应长和高的位图
Input:		   x,y,length,height,mode
					x：0~127 起点坐标
					y：0~63
					length：显示图像的长
					height：显示图像的高
               mode:0,反白显示;1,正常显示
Output:			无
*********************************************************************/
void OLED_DrawBMP(u8 x,u8 y,u8 length,u8 height,u8 mode)
{
	u8 temp,t1;
	u8 y0=y;
	u16 t;	//此处也是坑！当我改了bmpsize却忘了改t，导致显示异常，经调试想起来t是要累加到bmpsize的！
	u16 bmpsize=(height/8+((height%8)?1:0))*(length);	//坑爹的！此变量bmpsize类型应设为u16，先前为u8最大只能表示256，但我128*64的位图是1024的，故应当改为u16
	for(t=0;t<bmpsize;t++)	//每个字节要写入一次数据，故此为总写入次数
	{
		temp=BMP[t];
		  for(t1=0;t1<8;t1++)	//由于for循环的存在，每当纵向写入8位后就自动结束
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==height)
			{
				y=y0;	//将纵坐标清为初始值
				x++;	//将横坐标加一，故写入方式为一列一列先
				break;
			}
		}
	}
}


//***************************************************************************//
//                                                                           //
//                             OLED控制开关函数                              //
//                                                                           //
//***************************************************************************//


/*********************************************************************
Function:      OLED_ON(void);
Description:   将OLED从休眠中唤醒
Input:		   无
Output:		   无
*********************************************************************/
void OLED_ON(void)
{
	OLED_Write(0X8D,OLED_CMD);  //设置电荷泵
	OLED_Write(0X14,OLED_CMD);  //开启电荷泵
	OLED_Write(0XAF,OLED_CMD);  //OLED唤醒
}


/*********************************************************************
Function:      void OLED_OFF(void);
Description:   让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
Input:		   无
Output:		   无
*********************************************************************/
void OLED_OFF(void)
{
	OLED_Write(0X8D,OLED_CMD);  //设置电荷泵
	OLED_Write(0X10,OLED_CMD);  //关闭电荷泵
	OLED_Write(0XAE,OLED_CMD);  //OLED休眠
}
