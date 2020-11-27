#include <stdio.h>
#include "hz.h"
#include "LST.h"
#include "mouse.h"
#include "findwin.h"
#include "usr.h"
#include "draw.h"

#ifdef _MSC_VER
#pragma warning(disable:4996)
#else
#include <dos.h>
#endif
#include <graphics.h>
#include <string.h>

extern usr u;
extern int mousex, mousey, mousepress;

/***************************************show_files****************************************
function:show the open-file history on the files submenu
parameters:
	s:contents of files which should be updated in this function
return:
	the number of files submenu's items 
*******************************************************************************************/
int showfiles(char s[12][21])
{
	int i = 5, j = 5;
	char str[21];
	char buffer[100];
	FILE* fp;

	if (fp = fopen(u.username, "r"))
	{
		while (fgets(buffer, 100, fp) && buffer)
		{
			if (strchr(buffer, '\n') && (strcmp(buffer, "\n") != 0))
			{
				getstring(buffer, strchr(buffer, '\n') - 1, str);
				strcpy(s[i], str);
				i++;
			}
			else if (strcmp(buffer, "\n") != 0)
			{
				strcpy(s[i], buffer);
				i++;
			}
		}
		j = i;
		fclose(fp);
	}

	for (; i < 12; i++)
	{
		strcpy(s[i], "\0");
	}
	return j;
}

/***************************************drawlogin****************************************
function:draw the login interface
parameters:
	none
return:
	none
**********************************************************************************************/
void drawlogin()
{
	setbkcolor(BLACK);
	cleardevice();
	drawbar(0, 0, 640, 180, BLACK);
	drawBCtitle();
	drawbutton(WHITE, BLACK, 100, 240, 160, 280, "用户名:", 10, 10);
	drawbutton(WHITE, BLACK, 100, 300, 160, 340, "密码:", 20, 10);
	drawbar(200, 240, 500, 280, WHITE);
	drawbar(200, 300, 500, 340, WHITE);
	drawbutton(BLACK, LIGHTGRAY, 200, 380, 300, 420, "登录", 30, 10);
	drawbutton(BLACK, LIGHTGRAY, 400, 380, 500, 420, "注册", 30, 10);
}

/***************************************drawBCtitle****************************************
function:draw the artistic BC title
parameters:
	none
return:
	none
**********************************************************************************************/
void drawBCtitle()
{
	setcolor(BLUE);
	settextstyle(1, 0, 7);
	outtextxy(120, 50, "B");
	outtextxy(160, 50, "O");
	outtextxy(200, 50, "R");
	outtextxy(240, 50, "L");
	outtextxy(280, 50, "A");
	outtextxy(320, 50, "N");
	outtextxy(360, 50, "D");
	outtextxy(420, 50, "C");
	outtextxy(460, 50, "+");
	outtextxy(500, 50, "+");
	drawtext(190, 140, "汉化编辑器", 48, 50, 40, BLUE);
	settextstyle(2, 0, 5);
	setcolor(BLACK);
}

/***************************************drawbutton******************************************
function:draw button with text
parameters:
	
return:
	none
**********************************************************************************************/
void drawbutton(int colortext, int colorbar, int x1, int y1, int x2, int y2, char* s, int x, int y)
{
	drawbar(x1, y1, x2, y2, colorbar);
	drawtext(x1 + x, y1 + y, s, 16, 20, 20, colortext);
}

/***************************************drawBCtitle****************************************
function:draw the artistic BC title
parameters:
	fn:filename
return:
	none
**********************************************************************************************/
void drawfilename(char* fn)
{
	char s[40] = " Borland C++ [";
	strcat(s, u.username);
	strcat(s, "]");
	static char lastfn[20] = { '\0' };
	static int flag = 1;
	if (flag)
	{
		drawtext(5, 5, fn, 16, 15, 15, BLACK);
		drawtext(355, 5, s, 16, 15, 15, BLACK);
		flag = 0;
		strcpy(lastfn, fn);
	}
	else if (flag == 0)
	{
		if (strcmp(fn, lastfn) == 0)
		{
			return;
		}
		else
		{
			drawbar(0, 0, 640, 20, WHITE);
			drawtext(5, 5, fn, 16, 15, 15, BLACK);
			drawtext(355, 5, s, 16, 15, 15, BLACK);
			strcpy(lastfn, fn);
			return;
		}
	}
}

/***************************************mydelay****************************************
function:delay
parameters:
	x:microseconds
return:
	none
**********************************************************************************************/
void mydelay(int x)
{
#ifdef __BORLANDC__
	sleep(x / 1000);
#else
	Sleep(x);
#endif
}

/***************************************initbg****************************************
function:init the background
parameters:
	none
return:
	none
**********************************************************************************************/
void initbg()
{
	setbkcolor(WHITE);
	cleardevice();
}

/***************************************drawbg****************************************
function:draw the background
parameters:
	none
return:
	none
**********************************************************************************************/
void drawbg()
{
	drawbar(0, 40, 640, 460, LIGHTBLUE);
}

/***************************************drawrect****************************************
function:draw a rectangle
parameters:
	
return:
	none
**********************************************************************************************/
void drawrect(int x1, int y1, int x2, int y2, int style, int linesize, int col)		//draw rect border
{
	setcolor(col);
	setlinestyle(style, 0, linesize);
	line(x1, y1, x2, y1);
	line(x1, y1, x1, y2);
	line(x2, y2, x2, y1);
	line(x2, y2, x1, y2);
}

/***************************************drawmenu****************************************
function:draw menu
parameters:
	none
return:
	none
**********************************************************************************************/
void drawmenu()
{
	drawbar(0, 20, 640, 480, LIGHTGRAY);
	drawbar(0, 0, 640, 20, WHITE);
	//区域（5，20，15，25）
	setcolor(RED);
	line(15, 22, 30, 22);
	line(15, 28, 30, 28);
	line(15, 34, 30, 34);


	setcolor(BLUE);
	//drawtext(40, 2, "汉化编辑器", 16, 20, 10, BLUE);
	drawtext(80, 20, "文件", 16, 20, 10, BLACK);
	drawtext(130, 20, "编辑", 16, 20, 10, BLACK);
	drawtext(180, 20, "搜索", 16, 20, 10, BLACK);
	//	drawtext(230, 20, "你好,", 16, 20, 10, BLACK);
	//	drawtext(280, 20, u.username, 16, 20, 10, BLACK);
	drawtext(500, 20, "窗口", 16, 20, 10, BLACK);
	drawtext(550, 20, "帮助", 16, 20, 10, BLACK);
	drawtext(600, 20, "退出", 16, 20, 10, BLACK);
}

/***************************************drawbar****************************************
function:draw a filled bar
parameters:
	
return:
	none
**********************************************************************************************/
void drawbar(int x1, int y1, int x2, int y2, int color)
{
	setfillstyle(1, color);
	bar(x1, y1, x2, y2);
}

/***************************************drawhint***************************************
function:draw hint
parameters:
	s1:text 1
	s2:text 2
return:
	none
**********************************************************************************************/
void drawhint(char* s1, char* s2)
{
	drawbar(210, 190, 510, 390, BLUE);
	drawbar(200, 200, 500, 400, LIGHTBLUE);
	drawtext(210, 220, s1, 16, 20, 10, BLACK);
	drawtext(210, 300, s2, 16, 20, 10, BLACK);
	mydelay(2000);
	cleardevice();
}

/***************************************drawletter****************************************
function:draw a letter in (x1,y1)
parameters:
	
return:
	none
**********************************************************************************************/
void drawletter(int x1, int y1, char c)
{
	char str[2];
	str[0] = c;
	str[1] = '\0';
	settextstyle(2, 0, 7);
	outtextxy(x1, y1, str);
	settextstyle(2, 0, 5);
}

/***************************************drawsubwin****************************************
function:draw sub window
parameters:
	s1:text 1
	s2:text 2
return:
	none
**********************************************************************************************/
void drawsubwin(char* s1, char* s2)
{
	drawbar(170, 150, 470, 330, LIGHTGRAY);
	drawtext(200, 180, s1, 16, 20, 10, BLACK);
	drawtext(200, 210, s2, 16, 20, 10, BLACK);
	drawbar(240, 270, 380, 300, WHITE);
	drawbar(450, 150, 470, 170, RED);
	drawtext(453, 153, "x", 16, 5, 5, BLACK);
	drawbar(400, 270, 440, 300, YELLOW);
	drawtext(403, 273, "确定", 16, 20, 10, BLACK);
}

/***************************************drawfillcircle****************************************
function:draw filled cicle
parameters:
	
return:
	none
**********************************************************************************************/
void fillcircle(int x, int y, int r, int col)
{
	setcolor(col);
	setfillstyle(1, col);
	fillellipse(x, y, r, r);
}

/***************************************drawsubmenu****************************************
function:draw sub window
parameters:
	x:x-axis value
	s：contents
	num:number of items
return:
	none
**********************************************************************************************/
void drawsubmenu(int x, char s[][21], int num)
{
	int i = 0, y = 55;
	drawbar(x, 40, x + 120, 70 + num * 20, LIGHTGRAY);
	drawrect(x + 10, 50, x + 110, 60 + num * 20, SOLID_LINE, 2, BLACK);
	for (i = 0; i < num; i++)
	{
		drawtext(x + 15, y, s[i], 16, 20, 10, BLACK);
		y += 20;
	}
}

/***************************************drawattention****************************************
function:draw attention 
parameters:
	
return:
	none
**********************************************************************************************/
void drawattention(char* title, char s1[][50], int line)
{
	int y = 135, i = 0;
	drawbar(80, 100, 590, 150 + line * 20, LIGHTGRAY);
	drawbar(570, 100, 590, 120, RED);
	drawtext(100, 105, title, 16, 15, 15, BLACK);
	for (i = 0; i < line; i++)
	{
		drawtext(100, y, s1[i], 16, 15, 15, BLACK);
		y += 20;
	}
	drawtext(570, 105, "x", 16, 15, 15, BLACK);
	while (1)
	{
		mousemove();
		if (buttonpress(570, 100, 590, 120, mousex, mousey) && mousepress)
			return;

		drawmouse();
#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
}

/***************************************drawyesorno****************************************
function:draw a pop message with yes and no
parameters:
	s1:text 1
	s2:text 2
return:
	state:1 for yes,0 for no,2 for no operations(x)
**********************************************************************************************/
int drawyesorno(char* s1, char* s2)
{
	static int last_mousex = -1, last_mousey = -1, last_mousepress = 0;
	setcolor(BLACK);
	rectangle(220, 200, 420, 320);
	drawbar(220, 200, 420, 320, GREEN);
	drawbar(400, 200, 420, 220, RED);
	drawtext(405, 205, "x", 16, 20, 20, BLACK);
	drawtext(240, 220, s1, 16, 20, 10, BLACK);
	drawtext(240, 245, s2, 16, 20, 10, BLACK);
	drawbar(240, 280, 280, 310, YELLOW);
	drawbar(320, 280, 360, 310, YELLOW);
	drawtext(245, 285, "确定", 16, 15, 15, BLACK);
	drawtext(325, 285, "取消", 16, 15, 15, BLACK);
	while (1)
	{
		mousemove();
		if (buttonpress(400, 200, 420, 220, mousex, mousey) && mousepress)
		{
			last_mousex = mousex;
			last_mousey = mousey;
			last_mousepress = mousepress;
			return 2;
		}
		if (buttonpress(320, 280, 360, 310, mousex, mousey) && mousepress)
		{
			last_mousex = mousex;
			last_mousey = mousey;
			last_mousepress = mousepress;
			return 0;
		}
		if (buttonpress(240, 280, 280, 310, mousex, mousey) && mousepress)
		{
			last_mousex = mousex;
			last_mousey = mousey;
			last_mousepress = mousepress;
			return 1;
		}

		drawmouse();
#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
	while (1)
	{
		mousemove();
		if (last_mousepress && last_mousex == mousex && last_mousey == mousey)
		{

		}
		else
			break;
#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
}
