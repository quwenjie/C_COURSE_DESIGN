#define CONFIG_DEBUG
#pragma warning(disable:4996)
#include "LST.h"
#include "mouse.h"
#include "text.h"
#include "usr.h"
#include "draw.h"
#include "file_op.h"
#include "docwin.h"
#include "refresh.h"
#ifdef _MSC_VER
#pragma warning(disable:4996)
#else
#include <dos.h>
#endif
#include <graphics.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern usr u;
extern int mousex, mousey, mousepress;
void refreshbg()
{
	drawbg();
	drawmenu();
}
//state &2 -> refresh all  &1->refresh focuswindow
void refresh_windows(WINDOW* winhead, WINDOW* focuswin, int crow1, int ccol1, int crow2, int ccol2, int focusrow, int focuscol, int* cursorflag, int* state,ACNode* ac)
{
	WINDOW* win;
	
	if (*state & 2)   //have 2 redraw all
		drawbg();
	if (focuswin != NULL)
	{
		if (*state & 2)                    //redraw all windows
		{
			for (win = winhead; win; win = win->next)
			{
				if (win != focuswin)
					drawWindow(win, -1, -1, -1, -1,ac,focusrow,focuscol);
			}
		}
		if (*state)                   //only when top window unchanged,nothing happen 
			drawWindow(focuswin, crow1, ccol1, crow2, ccol2,ac, focusrow, focuscol);  //otherwise,redraw top
		if(focuswin->mode<3)              //if not drag zoom, draw cursor
			drawcursor(focuswin, focusrow, focuscol, cursorflag);
	}
	*state = 0;
}

/***************************************refresh****************************************
function:update file name showing
parameters:
	s:filename
	win:winhead
return:
	none
*******************************************************************************************/
void refresh(char* s, WINDOW* win)
{
	if (win)
		drawfilename(s);
}

/***************************************selectall****************************************
function:refresh user's files
parameters:
	win:winhead
return:
	none
*******************************************************************************************/
void refreshusers(WINDOW* win)
{
	WINDOW* wp;
	FILE* fp;
	fp = fopen(u.username, "w");
	for (wp = win; wp; wp = wp->next)
	{
		if (wp->filename&&wp->next)
		{
			if (!strstr(wp->filename, "noname"))
			{
				fputs(wp->filename, fp);
				fputc('\n', fp);
			}
		}
		else if(wp->next==0)
		{
			if (!strstr(wp->filename, "noname"))
			{
				fputs(wp->filename, fp);
				fputc('\n', fp);
			}
		}
	}
	fclose(fp);
}

void update_first_rowcol(WINDOW* win, int focusrow, int focuscol)		//刷新 firstrow col 准则：保持光标能被看到
{
	int vrow, vcol, maxrow, maxcol;
	vrow = visiblerowcount(win);
	maxrow = win->firstrow + vrow - 1;
	if (focusrow > maxrow)
		win->firstrow = 1 + focusrow - vrow;
	else if (focusrow < win->firstrow)
		win->firstrow = focusrow;
	vcol = visiblecolcount(win);
	maxcol = win->firstcol + vcol - 1;
	if (focuscol > maxcol)
		win->firstcol = 1 + focuscol - vcol;
	else if (focuscol < win->firstcol)
		win->firstcol = focuscol;
}

void update_focus_rowcol(WINDOW* win, int* focusrow, int* focuscol)		//刷新 focus row col 准则：保持光标能被看到
{
	int vrow, vcol, maxrow, maxcol;
	ROWNODE* r;
	vrow = visiblerowcount(win);
	maxrow = win->firstrow + vrow - 1;             //the bottom row visible
	if (*focusrow > maxrow)                       
		*focusrow = maxrow;
	else if (*focusrow < win->firstrow)      //the first row visible
		*focusrow = win->firstrow;
	vcol = visiblecolcount(win);
	maxcol = win->firstcol + vcol - 1; //the max col visible
	if (*focuscol > maxcol)             
		*focuscol = maxcol;
	else if (*focuscol < win->firstcol)
		*focuscol = win->firstcol;
	r = queryrowindex(win, *focusrow);
	if (*focuscol > r->len)
		*focuscol = r->len;
}

