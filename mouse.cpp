#ifdef __BORLANDC__
#include<conio.h>
#include<dos.h>
#endif
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include "mouse.h"
int mousex, mousey,mousepress,prev_mx,prev_my,prev_press;
void* buffer;

#ifdef __BORLANDC__
union REGS regs;
int flag = 0;
/*****************************
FUNCTION: mouse
DESCRIPTION: 画不同形态的鼠标
INPUT: x,y
RETURN: 无
******************************/
void mouse(int x, int y)
{
		setlinestyle(0, 0, 1);
		setcolor(WHITE);
		line(x, y, x, y + 13);
		line(x + 1, y + 1, x + 1, y + 12);
		line(x + 2, y + 2, x + 2, y + 11);
		line(x + 3, y + 3, x + 3, y + 10);
		line(x + 4, y + 4, x + 4, y + 12);
		line(x + 5, y + 5, x + 5, y + 9);
		line(x + 5, y + 11, x + 5, y + 14);
		line(x + 6, y + 6, x + 6, y + 9);
		line(x + 6, y + 13, x + 6, y + 15);
		line(x + 7, y + 7, x + 7, y + 9);
		line(x + 8, y + 8, x + 8, y + 9);
		line(x + 9, y + 9, x + 9, y + 9);
		setcolor(DARKGRAY);
		line(x - 1, y - 1, x - 1, y + 14);
		line(x - 1, y + 14, x + 3, y + 11);
		line(x + 3, y + 11, x + 3, y + 12);
		line(x + 3, y + 12, x + 4, y + 13);
		line(x + 4, y + 13, x + 4, y + 14);
		line(x + 4, y + 14, x + 7, y + 17);
		line(x + 7, y + 17, x + 7, y + 13);
		line(x + 7, y + 13, x + 6, y + 12);
		line(x + 6, y + 12, x + 6, y + 11);
		line(x + 6, y + 11, x + 5, y + 10);
		line(x + 5, y + 10, x + 11, y + 10);
		line(x + 11, y + 10, x - 1, y - 2);
}


/***************************************
FUNCTION: mread
DESCRIPTION: 获取新的寄存器信息
INPUT: nx,ny,nbuttons
RETURN: 无
****************************************/
void mread(int* nx, int* ny, int* nbuttons)
{
	regs.x.ax = 3;
	int86(51, &regs, &regs);
	*nx = regs.x.cx;
	*ny = regs.x.dx;
	*nbuttons = regs.x.bx;
}

/*******************************************
FUNCTION: newmouse
DESCRIPTION: 鼠标状态发生变化则更新鼠标
INPUT: nx,ny,nbuttons
RETURN: 无
********************************************/
void save_bk_mou(int nx, int ny)//存鼠标背景
{
	int size;

	size = imagesize(nx - 1, ny - 2, nx + 11, ny + 17);
	buffer = malloc(size);
	if (buffer != NULL)
		getimage(nx - 1, ny - 2, nx + 11, ny + 17, buffer);
	else
		printf("Error");
}

void clrmous(int nx, int ny)//清除鼠标
{
	if (flag == 1)
	{
		setwritemode(XOR_PUT);
		mouse(nx, ny);
		putimage(nx - 1, ny - 2, buffer, COPY_PUT);
		free(buffer);
		flag = 0;
		setwritemode(COPY_PUT);
	}
}
void drawmous(int nx, int ny)
{
	if (flag == 0)
	{
		setwritemode(COPY_PUT);
		mouse(nx, ny);
		flag = 1;
	}
}
#endif
void mousemove()
{
	prev_mx = mousex;
	prev_my = mousey;
	prev_press = mousepress;
#ifdef __BORLANDC__			
	int xn, yn, buttonsn;
	mread(&xn, &yn, &buttonsn);
	mousex = xn;
	mousey = yn;
	mousepress = buttonsn;
	clrmous(prev_mx, prev_my);
#else
	
	mousepos(&mousex, &mousey);
	mousepress = GetAsyncKeyState(VK_LBUTTON);
#endif
}
void drawmouse()
{
#ifdef __BORLANDC__
	save_bk_mou(mousex, mousey);
	drawmous(mousex, mousey);
#endif
}

void mouseinit()//初始化
{
#ifdef __BORLANDC__
	int retcode;
	int xmin, xmax, ymin, ymax, x_max = 625, y_max = 480;
	int size;

	xmin = 2;
	xmax = x_max - 1;
	ymin = 8;
	ymax = y_max - 2;
	regs.x.ax = 0;
	int86(51, &regs, &regs);
	retcode = regs.x.ax;
	if (retcode == 0)
	{
		printf("Mouse or Mouse Driver Obsent,Please Install!");
		delay(5000);
	}
	else
	{
		regs.x.ax = 7;
		regs.x.cx = xmin;
		regs.x.dx = xmax;
		int86(51, &regs, &regs);
		regs.x.ax = 8;
		regs.x.cx = ymin;
		regs.x.dx = ymax;
		int86(51, &regs, &regs);
	}
	prev_mx = 320, prev_my= 240;
	//save_bk_mou(MouseX,MouseY);
	regs.x.ax = 4;
	regs.x.cx =	prev_mx;
	regs.x.dx = prev_my;
	int86(51, &regs, &regs);
#else
	setrendermode(RENDER_MANUAL);
#endif
}
int buttonpress(int x1, int y1, int x2, int y2,int mx,int my)
{
	if (mx <= x2 && my <= y2 && mx >= x1 && my >= y1)
		return 1;
	else
		return 0;
}