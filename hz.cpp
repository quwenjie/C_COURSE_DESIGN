#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include "hz.h"
void drawtext(int x, int y, char* s, int fontsize, int partChinese, int partEnglish, int color)//x,y,s(ANSI),�����С��������������Ӣ������������ɫ
{
	FILE* hzk_p = NULL;                                       //���庺�ֿ��ļ�ָ��
	unsigned char quma, weima;                 //���庺�ֵ������λ��
	unsigned long offset;                          //���庺�����ֿ��е�ƫ����
	unsigned char mask[] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };  //�������飬������ʾ���ֵ����е�����
	int i, j, pos, y0 = y;
	int orgcolor = getcolor();

	char mat[288];
	char eng[2] = "E";
	char fn[30];
	sprintf(fn, ".//HZK//HZ%d", fontsize);			//���ݲ�ͬ��������̬��ȡ
	hzk_p=fopen(fn, "rb");            //ʹ�����·��
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
				quma = s[0] - 0xa0;                      //�������
				weima = s[1] - 0xa0;                     //���λ��
				offset = (94L * (quma - 1) + (weima - 1)) * fsize;   //���Ҫ��ʾ�ĺ������ֿ��ļ��е�ƫ��
				fseek(hzk_p, offset, SEEK_SET);         //�ض�λ�ļ�ָ��
				fread(mat, fsize, 1, hzk_p);            //�����ú��ֵľ����������,1ΪҪ���������
				for (i = 0; i < fontsize; i++)
				{
					pos = fontsize / 8 * i;
					for (j = 0; j < fontsize; j++)    //һ��һ�е�ɨ�裬��λ��Ϊ��1�ĵ���ʾ����
					{
						if ((mask[j & 7] & mat[pos + (j >> 3)]))   //j%8ֻ����0��8֮��ѭ����j/8��0��1֮��ѭ��
							putpixel(x + j, y, color);

					}
					y++;
				}
				s += 2;
				x += partChinese;				//ƫ�ƺ��ֵľ���
			}
			else
			{
				eng[0] = s[0];
				outtextxy(x, y, eng);
				s += 1;
				x += partEnglish;				//ƫ��Ӣ�ֵľ���
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