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
	//ɾ���Ĵ���Ϊͷ����
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
	char fn[40] = "�Ƿ񱣴�";
	if (*win == NULL)
	{
		return;
	}
	else if ((*win)->next == NULL)//�رմ���Ϊβ������
	{
		strcat(fn, (*win)->filename);
		if ((flag = drawyesorno(fn, "�Ըô��ڵı༭"))==1)
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
	else//�м䴰��
	{
		strcat(fn, (*win)->filename);
		if ((flag=drawyesorno(fn, "�Ըô��ڵı༭"))==1)
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
	if ((flag=drawyesorno("�Ƿ񱣴��", "���д��ڵı༭"))==1)
	{
		save_all(win);
		win = NULL;
		return 1;
	}
	else if (flag  == 2)
	{
		return 0;//�����б������
	}
}