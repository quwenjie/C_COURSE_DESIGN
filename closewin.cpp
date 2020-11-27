#include "docwin.h"
#include "draw.h"
#include "file_op.h"
#include "closewin.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************popwin*******************************************
function:delete focuswin
parameters:
	win:winhead
	focus:top win
return:
	new win chain
**********************************************************************************************/
WINDOW* popwin(WINDOW* win,WINDOW* focus)
{
	WINDOW* wp,*wlast;
	//删除的窗口为头窗口
	if (win == focus)
	{
		return win->next;
	}
	for (wp = win; wp; wp = wp->next)
	{
		if (wp->next== focus)
		{
			wlast = wp;
			wp = wp->next;
			wlast->next = wp->next;
			free(focus);
			return win;
		}
	}
}

/***************************************close_a_window****************************************
function:close top window
parameters:
	win:winhead
	focus:top win
return:
	none
**********************************************************************************************/
void close_a_window(WINDOW** focuswin,WINDOW** win)
{
	int flag;
	char fn[40] = "是否保存";
	if (*win == NULL)
	{
		return;
	}
	else if ((*win)->next == NULL)//关闭窗口为尾部窗口
	{
		strcat(fn, (*win)->filename);
		if ((flag = drawyesorno(fn, "对该窗口的编辑"))==1)
		{
			save_file(*focuswin);
		}
		else if (flag==2)
		{
			return;
		}
		free(*win);
		*win = NULL;
	}
	else//中间窗口
	{
		strcat(fn, (*win)->filename);
		if ((flag=drawyesorno(fn, "对该窗口的编辑"))==1)
		{
			save_file(*focuswin);
		}
		else if(flag== 2)
		{
			return;
		}
		*win = popwin(*win, *focuswin);
	}
	*focuswin = *win;
}

/***************************************close_all_windows*************************************
function:close all windows
parameters:
	win:winhead
return:
	state:1 for saving all,0 for no operations
**********************************************************************************************/
int close_all_windows(WINDOW* win)
{
	int flag;
	if (win == NULL)
		return 1;
	if ((flag=drawyesorno("是否保存对", "所有窗口的编辑"))==1)
	{
		save_all(win);
		win = NULL;
		return 1;
	}
	else if (flag  == 2)
	{
		return 0;//不进行保存操作
	}
}