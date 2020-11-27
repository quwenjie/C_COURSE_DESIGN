#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include "hz.h"
void drawtext(int x, int y, char* s, int fontsize, int partChinese, int partEnglish, int color)//x,y,s(ANSI),字体大小，中文输出间隔，英文输出间隔，颜色
{
	FILE* hzk_p = NULL;                                       //定义汉字库文件指针
	unsigned char quma, weima;                 //定义汉字的区码和位码
	unsigned long offset;                          //定义汉字在字库中的偏移量
	unsigned char mask[] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };  //功能数组，用于显示汉字点阵中的亮点
	int i, j, pos, y0 = y;
	int orgcolor = getcolor();

	char mat[288];
	char eng[2] = "E";
	char fn[30];
	sprintf(fn, ".//HZK//HZ%d", fontsize);			//根据不同字体来动态读取
	hzk_p=fopen(fn, "rb");            //使用相对路径
	if (hzk_p == NULL)
		exit(-1);
	long fsize=fontsize * fontsize / 8L;
	setcolor(color);
	while (*s != NULL)
	{
		while (x < 640 && (*s != NULL))
		{
			y = y0;
			if (s[0] < 0)
			{
				quma = s[0] - 0xa0;                      //求出区码
				weima = s[1] - 0xa0;                     //求出位码
				offset = (94L * (quma - 1) + (weima - 1)) * fsize;   //求出要显示的汉字在字库文件中的偏移
				fseek(hzk_p, offset, SEEK_SET);         //重定位文件指针
				fread(mat, fsize, 1, hzk_p);            //读出该汉字的具体点阵数据,1为要读入的项数
				for (i = 0; i < fontsize; i++)
				{
					pos = fontsize / 8 * i;
					for (j = 0; j < fontsize; j++)    //一行一行地扫描，将位上为了1的点显示出来
					{
						if ((mask[j & 7] & mat[pos + (j >> 3)]))   //j%8只能在0—8之间循环，j/8在0，1之间循环
							putpixel(x + j, y, color);

					}
					y++;
				}
				s += 2;
				x += partChinese;				//偏移汉字的距离
			}
			else
			{
				eng[0] = s[0];
				outtextxy(x, y, eng);
				s += 1;
				x += partEnglish;				//偏移英字的距离
			}

		}

	}
	fclose(hzk_p);
	setcolor(orgcolor);
}
void drawchar(int x, int y, char c,int col)
{
	char st[3];
	if (c != '\"')
	{
		st[0] = c;
		st[1] = '\0';
	}
	else
	{
		st[0] = '\"';
		st[1] = '\0';
	}
	setcolor(col);
	outtextxy(x, y, st);
}