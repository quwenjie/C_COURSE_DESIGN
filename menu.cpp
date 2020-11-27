#include "LST.h"
#include "mouse.h"
#include "keyboard.h"
#include "hz.h"
#include "file_op.h"
#include "findwin.h"
#include "draw.h"
#include "edit.h"
#include "menu.h"
#include "refresh.h"
#include "file_op.h"
#include "help.h"
#include "search.h"
#include "closewin.h"
#include "alg.h"
#ifdef _MSC_VER
#pragma warning(disable:4996)
#else
#include <dos.h>
#include <process.h>
#endif
#include <graphics.h>
#include <stdio.h>
extern int mousex, mousey, mousepress;
int r = 0, cb = 0,ce = 0;

/***************************************menu****************************************
function:
	menu is showed on the top of the window.click parts of menu will triggered related 
	functions.this function is the control framework of menu
parameters:
	wp:pointer to winhead pointer 
	focuswin:
	(r1,c1,r2,c2): pointers -- paragraph position for editing
	(rr,cc):pointers -- cursor position for editing
	rep:editing paragraph container
procedure:
	#trigger file & edit & search & close & help submenus and functions
	#exit
return:
	state:0 for no operations,2 for seach, 1 for else
****************************************************************************************/
int menu(WINDOW** wp, WINDOW** focuswin, int* r1, int* c1, int* r2, int* c2, int* rr, int* cc, RE_PARA* rep)
{
	FILE* fp;
	int i; 
	char submenu1[12][21] = { "新建窗口", "打开", "保存", "另存为", "全部保存" };    //text saved in arrays
	char submenu2[6][21] = { "撤销", "重做", "剪切", "复制", "粘贴" ,"全选"};
	char submenu3[3][21] = { "查找", "替换", "转到" };
	char submenu4[2][21] = { "关闭当前", "关闭所有" };
	char submenu5[2][21] = { "介绍", "关于作者" };
	
	if (!mousepress && !buttonpress(mousex, mousey, 0, 20, 640, 480))
		return 0;

	if (*r1 >= 0 && *r2 >= 0 && *c1 >= 0 && *c2 >= 0)        //avoid illegal r1,c1,r2,c2
	{
		if (*r1 > * r2)
		{
			swap(r1, r2);
			swap(c1, c2);
		}
		else if (*r1 == *r2 && *c1 > * c2)
			swap(c1, c2);
	}
	if (!dzwindow(*wp))
	{
		if (buttonpress(80, 20, 130, 40, mousex, mousey) )
		{
			i = showfiles(submenu1);
			drawsubmenu(80, submenu1, i);
			file(wp, focuswin, submenu1);	
			while (1)
			{
				mousemove();
				if(!mousepress)
					break;
				drawmouse();
#ifdef _MSC_VER
				delay_ms(50);
#else
				delay(50);
#endif
			}
			return 1;
		}
		if (buttonpress(130, 20, 180, 40, mousex, mousey))
		{
			drawsubmenu(130, submenu2, 6);
			edit(*focuswin,r1,c1,r2,c2,*rr+1,*cc+1,rep);
			while (1)
			{
				mousemove();
				if(!mousepress)
					break;
				drawmouse();
#ifdef _MSC_VER
				delay_ms(50);
#else
				delay(50);
#endif
		}
			return 1;
		}
		if (buttonpress(180, 20, 230, 40, mousex, mousey))
		{
			drawsubmenu(180, submenu3,3);
			search(*focuswin,rr,cc,r1,c1,r2,c2);
			while (1)
			{
				mousemove();
				if(!mousepress)
					break;
				drawmouse();
#ifdef _MSC_VER
				delay_ms(50);
#else
				delay(50);
#endif
		}
			return 2;
		}
		if (buttonpress(500, 20, 550, 40, mousex, mousey) )
		{
			drawsubmenu(500, submenu4,2);
			close(focuswin, wp);
			while (1)
			{
				mousemove();
				if(!mousepress)
					break;
				drawmouse();
#ifdef _MSC_VER
				delay_ms(50);
#else
				delay(50);
#endif
			}
			return 1;
		}
		if (buttonpress(550, 20, 600, 40, mousex, mousey))
		{
			drawsubmenu(550, submenu5, 2);
			help();
			while (1)
			{
				mousemove();
				if(!mousepress)
					break;
				drawmouse();
#ifdef _MSC_VER
				delay_ms(50);
#else
				delay(50);
#endif
			}
			return 1;
		}
		if (buttonpress(600, 20, 640, 40, mousex, mousey))
		{
			if (drawyesorno("您确定", "要退出吗"))
			{
				if (drawyesorno("是否保存", "所有更改") == 1)
				{
					save_all(*wp);
				}
				else if (drawyesorno("是否保存", "所有更改") == 2)
				{
					return 1;
				}
				refreshusers(*wp);
				fp = fopen("copytxt", "w");
				fputc('\n', fp);
				fclose(fp);
				exit(0);
			}
			else
			{
				while (1)
				{
					mousemove();
					if(!mousepress)
						break;
					drawmouse();
#ifdef _MSC_VER
					delay_ms(50);
#else
					delay(50);
#endif
				}
				return 1;
			}
		}
	}
	return 0;
}


void search(WINDOW* focuswin, int* focusrow, int* focuscol, int* ccrow1, int* ccol1, int* crow2, int* col2)   //search menu
{
	while (1)
	{
		mousemove();
		if (buttonpress(180, 55, 240, 75, mousex, mousey) &&mousepress)
		{
			find_opt(focuswin, focusrow, focuscol, ccrow1, ccol1,  crow2,  col2);      //find option
			return;
		}
		if (buttonpress(180, 75, 240, 95, mousex, mousey) && mousepress)
		{
			replace_opt(focuswin, focusrow, focuscol, ccrow1, ccol1, crow2, col2);      //replace option
			return;
		}
		if (buttonpress(180, 95, 240, 115, mousex, mousey) && mousepress)
		{
			jump_opt(focuswin, focusrow, focuscol);                                  //jump option
			return;
		}
		if (!buttonpress(180, 20, 240, 140, mousex, mousey) && mousepress)
			return;
		drawmouse();
#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
	
}

/***************************************file****************************************
function:
	file functions control framework	
parameters:
	wp:pointer to winhead pointer 
	focuswin:
	s:file submenu content
procedure:
	tigger save & save as & save all & new file & open file functions
return:
	none
****************************************************************************************/
void file(WINDOW** wp, WINDOW** focuswin, char s[12][21])
{
	WINDOW* win;
	while (1)
	{
		mousemove();
		if (buttonpress(80, 155, 130, 155 + 20 + 20 * (file_num(s) - 5), mousex, mousey) && mousepress)
		{
 			open_click_file(mousex, mousey, s, wp,focuswin);
			if ((*wp)->next == NULL)
				*focuswin = *wp;
			return;
		}
		if (buttonpress(80, 55, 130, 75, mousex, mousey) && mousepress)
		{
			if(*focuswin)
				(*focuswin)->mode = 2;
			if (win = new_file(wp))
			{
				*focuswin = win;
			}
			return;
		}
		if (buttonpress(80, 75, 130, 95, mousex, mousey) && mousepress)
		{
			if(*focuswin)
				(*focuswin)->mode = 2;
			if (win = open_file(wp))
			{
				*focuswin = win;
			}
			return;
		}
		if (buttonpress(80, 95, 130, 115, mousex, mousey) && mousepress)
		{
			save_file(*focuswin);
			return;
		}
		if (buttonpress(80, 115, 130, 135, mousex, mousey) && mousepress)
		{
			save_as(*focuswin);
			return;
		}
		if (buttonpress(80, 135, 130, 155, mousex, mousey) && mousepress)
		{
			save_all(*wp);
		}
		if (!buttonpress(80, 20, 130, 155 + 20 + 20 * (file_num(s) - 5), mousex, mousey) && mousepress)
			return;
		drawmouse();

#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
}

/***************************************edit****************************************
function:
	edit functions control framework	
parameters: 
	focuswin:
	(r1,c1,r2,c2): pointers -- paragraph position for editing
	(rr,cc):pointers -- cursor position for editing
	rep:editing paragraph container
procedure:
	#tigger redo & undo as & paste & cut & copy & select all functions
	#record cut & copy paragraph
return:
	none
****************************************************************************************/
void edit(WINDOW* focuswin, int* r1, int* c1, int* r2, int* c2, int rr, int cc, RE_PARA* rep)
{
	char str[] = "copytext";
	while (1)
	{
		mousemove();
		if (buttonpress(130, 55, 180, 75, mousex, mousey) && mousepress)
		{
			redo(rep);
			return;
		}
		if (buttonpress(130, 75, 180, 95, mousex, mousey) && mousepress)
		{
			undo(rep);
			return;
		}
		if (buttonpress(130, 95, 180, 115, mousex, mousey) && mousepress)
		{
			cut(focuswin, *r1 + 1, *c1 + 1, *r2 + 1, *c2 + 1, str);
			record_cut(focuswin, *r1 + 1, *c1 + 1, *r2 + 1, *c2 + 1,rep);
			r = *r2 - *r1;
			cb = *c1;
			ce = *c2;
			*r1 = *c1 = *r2 = *c2 = -1;
			return;
		}
		if (buttonpress(130, 115, 180, 135, mousex, mousey) && mousepress)
		{
			copy(focuswin, *r1 + 1, *c1 + 1, *r2 + 1, *c2 + 1, str);
			r = *r2 - *r1;
			cb = *c1;
			ce = *c2;
			return;
		}
		if (buttonpress(130, 135, 180, 155, mousex, mousey) && mousepress)
		{
			paste(focuswin, rr, cc, str);
			record_paste(focuswin, rr, cc, r, cb,ce ,rep);
			return;
		}
		if (buttonpress(130, 155, 180, 175, mousex, mousey) && mousepress)
		{
			selectall(focuswin,r1,c1,r2,c2);
			break;
		}
		if (!buttonpress(130, 20, 180, 180, mousex, mousey) && mousepress)
			return;
		drawmouse();

#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
}

/***************************************close****************************************
function:
	close functions control framework
parameters:
	wp:pointer to winhead pointer 
	focuswin:
procedure:
	trigger close one window & close all windows functions
return:
	none
****************************************************************************************/
void close(WINDOW** focuswin, WINDOW** win)
{
	int flag;
	while (1)
	{
		mousemove();
		if (buttonpress(500, 55, 550, 75, mousex, mousey) && mousepress)
		{
			close_a_window(focuswin, win);
			return;
		}
		if (buttonpress(500, 75, 550, 95, mousex, mousey) && mousepress)
		{
			flag=close_all_windows(*win);
			if(flag==1)
				*win = *focuswin = NULL;
			return;
		}
		if (!buttonpress(500, 20, 550, 140, mousex, mousey) && mousepress)
			return;
		drawmouse();

#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
}

void help()
{
	while (1)
	{
		mousemove();
		if (buttonpress(550, 55, 600, 75, mousex, mousey) && mousepress)
		{
			introduction();
			return;
		}
		if (buttonpress(550, 75, 600, 95, mousex, mousey) && mousepress)
		{
			about();
			return;
		}
		if (!buttonpress(550, 20, 600, 140, mousex, mousey) && mousepress)
			return;
		drawmouse();

#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
}