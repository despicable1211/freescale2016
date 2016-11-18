#include    "common.h"
#include    "FLYFISH_OLED.h"

#include    "FLYFISH_OLEDFONT.h"


u8 OLED_GRAM[128][8];


//***************************************************************************//
//                                                                           //
//                              OLED��ʼ������                               //
//                                                                           //
//***************************************************************************//


/*********************************************************************
Function:      OLED_Init();
Description:   OLED��ʼ������
Input:		   ��
Output:	       ��
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


	OLED_Write(0xAE,OLED_CMD); //�ر���ʾ
	OLED_Write(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_Write(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_Write(0xA8,OLED_CMD); //��������·��
	OLED_Write(0X3F,OLED_CMD); //Ĭ��0X3F(1/64)
	OLED_Write(0xD3,OLED_CMD); //������ʾƫ��
	OLED_Write(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_Write(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.

	OLED_Write(0x8D,OLED_CMD); //��ɱ�����
	OLED_Write(0x14,OLED_CMD); //bit2������/�ر�
	OLED_Write(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_Write(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_Write(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_Write(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_Write(0xDA,OLED_CMD); //����COMӲ����������
	OLED_Write(0x12,OLED_CMD); //[5:4]����

	OLED_Write(0x81,OLED_CMD); //�Աȶ�����
	OLED_Write(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_Write(0xD9,OLED_CMD); //����Ԥ�������
	OLED_Write(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_Write(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_Write(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_Write(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_Write(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ
	OLED_Write(0xAF,OLED_CMD); //������ʾ

	OLED_Clear();
}


//***************************************************************************//
//                                                                           //
//                               OLED�ײ㺯��                                //
//                                                                           //
//***************************************************************************//


/*********************************************************************
Function:      OLED_Write(u8 Data,u8 Cmd);
Description:   OLEDд�ֽں���
Input:		   Data��Cmd
Output:	       ��
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
Description:   ����GRAM���Դ�
Input:		   ��
Output:		   ��
*********************************************************************/
void OLED_Refresh_Gram(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		LED_WrCmd(0xb0+i);    //����ҳ��ַ��0~7��
		LED_WrCmd(0x00);      //������ʾλ�á��е͵�ַ
		LED_WrCmd(0x10);      //������ʾλ�á��иߵ�ַ
		for(n=0;n<128;n++)
			LED_WrDat(OLED_GRAM[n][i]);
	}
}


/*********************************************************************
Function:      OLED_Clear(void);
Description:   ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
Input:		   ��
Output:		   ��
*********************************************************************/
void OLED_Clear()
{
	u8 i,n;
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0X00;

	OLED_Refresh_Gram();//������ʾ
}


/*********************************************************************
Function:      OLED_Clear(void);
Description:   ��亯��������������Ļ
Input:		   ��
Output:		   ��
*********************************************************************/
void OLED_Fill()
{
	u8 i,n;
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0Xff;

	OLED_Refresh_Gram();//������ʾ
}


/*********************************************************************
Function:      OLED_POW(u8 m,u8 n);
Description:   ����m^n
Input:		   m������  n����
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
Description:   ���㺯����������Ļ��ĳ����
Input:		   x,y,t
					x:0~127
					y:0~63
					t:1 ��� 0,���
Output:			��
*********************************************************************/
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;//ȷ��ҳ������Ϊoledÿ�ζ��Ǵ����һ�п�ʼд��ģ������� 7-y/8
	bx=y%8;
	temp=1<<(7-bx);//ȷ����Ӧ��������ҳ����λ��
	if(t)
		OLED_GRAM[x][pos]|=temp;
	else
		OLED_GRAM[x][pos]&=~temp;
}


//===========================================================================//
//                                                                           //
//                             Ӣ�ĺ����ֵ���ʾ                              //
//                                                                           //
//===========================================================================//


/*********************************************************************
Function:      OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
Description:   ��ָ��λ����ʾһ���ַ�,���������ַ�����" "��"~"֮��
Input:		   x,y,chr,size,mode
					x��0~127 �������
					y��0~63
					char��һ���ֿ��е���Ӧ�ַ�
               size��ѡ�������С 12/16/24
					mode:0,������ʾ;1,������ʾ
Output:			��
*********************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);	//�õ�����һ���ַ���Ӧ������ռ���ֽ���....���⹫ʽ...
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ
    for(t=0;t<csize;t++)//ÿ���ֽ�Ҫд��һ�����ݣ��ʴ�Ϊ��д�����
    {
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)	//����forѭ���Ĵ��ڣ�ÿ������д��8λ����Զ�����
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)	//����size��ÿ������д�뵽12λ���Զ������������1206�Ļ���
			{
				y=y0;	//����������Ϊ��ʼֵ
				x++;	//���������һ����д�뷽ʽΪһ��һ����
				break;
			}
		}
    }
}


/*********************************************************************
Function:      OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 mode);
Description:   ��ָ��λ����ʾ����
Input:		   x,y,num,len,size,mode
					x��0~127 �������
					y��0~63
					num����ֵ(0~4294967295)
					len�����ֳ���
               size��ѡ�������С 12/16/24
					mode:0,������ʾ;1,������ʾ
Output:			��Ӧ����
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
Description:   ��ָ��λ����ʾ�ַ���
Input:		   x,y,*p,size
					x��0~127 �������
					y��0~63
					*p���ַ�����ʼ��ַ
               size��ѡ�������С 12/16/24
                mode:0,������ʾ;1,������ʾ
Output:			��
*********************************************************************/
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size,u8 mode)
{
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if(x>(128-(size/2))){x=0;y+=size;}	//��һ�е��ַ������һ���ַ�λ��������һ���ַ�ʱ�ͻ��е���һ����ʾ
        if(y>(64-size)){y=x=0;OLED_Clear();}	//��һ�е��ַ�λ���һ���ַ�λ��������һ���ַ�ʱ���ͽ��������㣬Ȼ������������Ȼ������(0,0)Ϊ���������ʾ
        OLED_ShowChar(x,y,*p,size,mode);
        x+=size/2;	//ÿд��һ���ַ�������ͼ�һ���ַ����
        p++;
    }
}


//===========================================================================//
//                                                                           //
//                               ���ֵ���ʾ                                  //
//                                                                           //
//===========================================================================//


/*********************************************************************
Function:      OLED_ShowHanzi(u8 x,u8 y,u8 num,u8 size,u8 mode);      PS.�˴���Hanzi����Ӣ��~
Description:   ��ָ��λ����ʾ�ֺ���
Input:		   x,y,num,size,mode
					x��0~127 �������
					y��0~63
					num����oledfont.h���Լ�����ĺ���������
               size��ѡ�������С 16
					mode:0,������ʾ;1,������ʾ
Output:			��
*********************************************************************/
void OLED_ShowHanzi(u8 x,u8 y,u8 num,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size);	//�õ�����һ���ַ���Ӧ������ռ���ֽ���....��ģ����Ǵ���ʽ�������Կ��ԣ�
	for(t=0;t<csize;t++)//ÿ���ֽ�Ҫд��һ�����ݣ��ʴ�Ϊ��д�����
	{
		if(size==16)temp=hanzi_1616[num][t]; 	 	//����1616����
		else return;								//û�е��ֿ�
		  for(t1=0;t1<8;t1++)	//����forѭ���Ĵ��ڣ�ÿ������д��8λ����Զ�����
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)	//����size��ÿ������д�뵽12λ���Զ������������1206�Ļ���
			{
				y=y0;	//����������Ϊ��ʼֵ
				x++;	//���������һ����д�뷽ʽΪһ��һ����
				break;
			}
		}
	}
}


/*********************************************************************
Function:     OLED_ShowSentence(u8 x,u8 y,u8 sta,u8 end,u8 size);
Description:   ��ָ��λ����ʾ���־���
Input:		   x,y,sta,end,size
					x��0~127 �������
					y��0~63
					sta�����־�����ʼλ��
					end�����־�����ֹλ��
               size��ѡ�������С 16
Output:			��
*********************************************************************/
void OLED_ShowSentence(u8 x,u8 y,u8 sta,u8 end,u8 size)
{
	for(;sta<end+1;sta++)
	{
		if(x>(128-(size))){x=0;y+=size;}	//��һ�еľ������һ������λ��������һ������ʱ�ͻ��е���һ����ʾ
		if(y>(64-size)){y=x=0;OLED_Clear();}	//��һ�еĺ���λ���һ������λ��������һ������ʱ���ͽ��������㣬Ȼ������������Ȼ������(0,0)Ϊ���������ʾ
		OLED_ShowHanzi(x,y,sta,size,1);
		x+=size;	//ÿд��һ���ַ�������ͼ�һ�����ֿ��
	}
}


//===========================================================================//
//                                                                           //
//                               λͼ����ʾ                                  //
//                                                                           //
//===========================================================================//


/*********************************************************************
Function:      OLED_DrawBMP(u8 x,u8 y,u8 length,u8 height,u8 mode);
Description:   ��ָ�����꿪ʼ��ʾ��Ӧ���͸ߵ�λͼ
Input:		   x,y,length,height,mode
					x��0~127 �������
					y��0~63
					length����ʾͼ��ĳ�
					height����ʾͼ��ĸ�
               mode:0,������ʾ;1,������ʾ
Output:			��
*********************************************************************/
void OLED_DrawBMP(u8 x,u8 y,u8 length,u8 height,u8 mode)
{
	u8 temp,t1;
	u8 y0=y;
	u16 t;	//�˴�Ҳ�ǿӣ����Ҹ���bmpsizeȴ���˸�t��������ʾ�쳣��������������t��Ҫ�ۼӵ�bmpsize�ģ�
	u16 bmpsize=(height/8+((height%8)?1:0))*(length);	//�ӵ��ģ��˱���bmpsize����Ӧ��Ϊu16����ǰΪu8���ֻ�ܱ�ʾ256������128*64��λͼ��1024�ģ���Ӧ����Ϊu16
	for(t=0;t<bmpsize;t++)	//ÿ���ֽ�Ҫд��һ�����ݣ��ʴ�Ϊ��д�����
	{
		temp=BMP[t];
		  for(t1=0;t1<8;t1++)	//����forѭ���Ĵ��ڣ�ÿ������д��8λ����Զ�����
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==height)
			{
				y=y0;	//����������Ϊ��ʼֵ
				x++;	//���������һ����д�뷽ʽΪһ��һ����
				break;
			}
		}
	}
}


//***************************************************************************//
//                                                                           //
//                             OLED���ƿ��غ���                              //
//                                                                           //
//***************************************************************************//


/*********************************************************************
Function:      OLED_ON(void);
Description:   ��OLED�������л���
Input:		   ��
Output:		   ��
*********************************************************************/
void OLED_ON(void)
{
	OLED_Write(0X8D,OLED_CMD);  //���õ�ɱ�
	OLED_Write(0X14,OLED_CMD);  //������ɱ�
	OLED_Write(0XAF,OLED_CMD);  //OLED����
}


/*********************************************************************
Function:      void OLED_OFF(void);
Description:   ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
Input:		   ��
Output:		   ��
*********************************************************************/
void OLED_OFF(void)
{
	OLED_Write(0X8D,OLED_CMD);  //���õ�ɱ�
	OLED_Write(0X10,OLED_CMD);  //�رյ�ɱ�
	OLED_Write(0XAE,OLED_CMD);  //OLED����
}
