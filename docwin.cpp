#include <graphics.h>
#include <string.h>
#include "docwin.h"
#include "mouse.h"
#include "hz.h"
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"
#include "refresh.h"
extern int mousex, mousey, mousepress,prev_press;


void bar_horz(WINDOW* win, int dx)	//handle bar horz move to pos->dx
{
	int x1, y1, x2, y2;
	int leftx,rightx,mxc=0,viscol=0;
	
	leftx = win->x + 85;                           //left of bar
	rightx = (win->x + win->w - 1) - FR - 10;           //right of bar
	x1 = dx;
	if (x1 <= leftx)                    //corner case
		x1 = leftx;
	mxc = getmaxcol(win);                 //max col
	viscol = visiblecolcount(win);
	win->firstcol = (x1 - leftx)*mxc / (rightx-leftx+1);   //set to ideal pos
 	
	if (win->firstcol + viscol > mxc)            //right do not exceed
		win->firstcol = mxc - viscol ;
	if (win->firstcol < 0)                 //left do not exceed
		win->firstcol = 0;
}
void bar_vert(WINDOW* win, int dy)  //handle bar vert move to pos->dy
{
	int y1;
	int upy,downy, mxc = 0, visrow = 0;
	mxc = win->rownum;                //max row
	upy = win->y + FR + 10;           //up
	downy = win->y+win->h-1-FR-10;  //bottom
	y1 = dy;
	if (y1 < upy)                //corner case
		y1 = upy;
	win->firstrow = (y1 - upy)*mxc / (downy-upy+1);    //ideal pos
	visrow = visiblerowcount(win);
	if (win->firstrow + visrow > mxc)          //down do not exceed
	{
		win->firstrow = mxc - visrow;
	}
	if (win->firstrow < 0)               //up do not exceed
		win->firstrow = 0;
}

int onWindow_horz_bar_click(WINDOW* win, int x, int y)   //judge if horz click
{
	int x1, x2, y1, y2;
	x1 = win->x;//four corners of the window
	y1 = win->y;
	x2 = win->x + win->w - 1;
	y2 = win->y + win->h - 1;
	if (buttonpress(x1 + 80, y2 - FR - 5, x2 - FR - 5, y2, x, y))    //press pos
		return 1;
	else
		return 0;
}
int onWindow_vert_bar_click(WINDOW* win, int x, int y)    //judge if horz click
{
	int x1, x2, y1, y2;
	x1 = win->x;//four corners of the window
	y1 = win->y;
	x2 = win->x + win->w - 1;
	y2 = win->y + win->h - 1;
	if (buttonpress(x2 - FR - 5, y1 + FR + 5, x2, y2 - FR - 5, x, y))  //press pos
		return 1;
	else
		return 0;
}
int clickWindow(WINDOW* win,int *fr, int* fc,  int* crow1, int* ccol1, int* crow2, int* ccol2)	//handle click document window issues
{
	int x1 ,y1, x2, y2 ;		//four corners of the window
	int x  ,y;
	int mx = mousex, my = mousey;
	ROWNODE* r;
	if (!win)
		return 0;
	x1 = win->x, y1 = win->y, x2 = win->x + win->w - 1, y2=win->y + win->h - 1;   //corners
	x = x1 + FR + 5, y = y1 + FR + 5;                        //text rect left top
	
	if (!buttonpress(x1, y1, x2, y2, mx, my))      //not on this win
		return 0;

	if (buttonpress(x1 + FR, y1, x2 - FR, y1 + FR, mx, my))		//click on title label
	{
		win->mode = 3;
		return 3;					//drag mode
	}
	if (buttonpress(x1, y1, x1 + FR, y1 + FR, mx, my))			//LT
	{
		win->mode = 4;
		return 4;
	}
	else if (buttonpress(x2 - FR, y2 - FR, x2, y2, mx, my))		//RB
	{
		win->mode = 5;
		return 5;
	}
	if (buttonpress(x1, y2-FR, x1 + FR, y2 , mx, my))			//LB
	{
		win->mode = 6;
		return 6;
	}
	if (buttonpress(x2-FR, y1, x2 , y1 + FR, mx, my))				//RT
	{
		win->mode = 7;
		return 7;
	}
	
	if (buttonpress(x1 + FR+5, y1 + FR+5, x2 - FR-5, y2 - FR-5, mx, my))//click on text
	{
		mx -= x;                //get relative pos
		my -= y;
		if (mx < 0)            //corner case
			mx = 0;
		if (my < 0)           //corner case
			my = 0;
		mx /= TEXTWIDTH;    //map to row,col
		my /= TEXTHEIGHT;
		mx += win->firstcol;   //real r,c in doc
		my += win->firstrow;

		if (my >= win->rownum)        //do not exceed range
			my = win->rownum - 1;
		r = queryrowindex(win, my);
		if (mx > r->len)         //do not exceed col range
			mx = r->len;
		
		if (prev_press)  //range select
		{
			if (*crow1 == -1)           
			{
				*crow1 = my;
				*ccol1 = mx;
				if (*ccol1 == r->len)   //set crow1 ccol1 pos
					*ccol1 = r->len - 1;
				*crow2 = *ccol2 = -1;
			}
			else
			{
				*crow2 = my;        //set crow2 ccol2 pos
				*ccol2 = mx;

				if (*ccol2 == r->len)
					*ccol2 = r->len - 1;

			}
		}
		else  //single click
		{
			*fr = my;     //cursor
			*fc = mx;
			*crow1 = *crow2 = *ccol1 = *ccol2 = -1;      //clear select
		}
		/*if (*crow1 >= 0 && *crow2 >= 0 && *ccol1 >= 0 && *ccol2 >= 0)
		{
			if (*crow1 > * crow2)
			{
				swap(crow1, crow2);
				swap(ccol1, ccol2);
			}
			else if (*crow1 == *crow2 && *ccol1 > * ccol2)
				swap(ccol1, ccol2);
		}
		*/
	}
	//printf("%d %d %d %d\n",*crow1,*ccol1,*crow2,*ccol2);
	if (win->mode == 1)      //click on top
	{
		if (onWindow_horz_bar_click(win, mousex, mousey))    //bar horz
		{
			bar_horz(win, mousex);
		}

		else if (onWindow_vert_bar_click(win, mousex, mousey))    //bar vert
		{
			bar_vert(win, mousey);
		}
	}
	if (win->mode == 1)      //click on top
	{
		return 1;
	}
	else if (win->mode == 2) //click on back win,need top exchange
	{
		return 2;
	}
}

void dragWindow(WINDOW* win, int dx, int dy)  //handle window drag
{
	int xmin ,xmax,ymin,ymax;		//the legitimate range of x,y
	xmin = 20;
	xmax = 620 - win->w;
	ymin = 41;
	ymax = 460 - win->h;
	win->x += dx;
	win->y += dy;
	if (win->x < xmin)
		win->x = xmin;
	if (win->x > xmax)
		win->x = xmax;
	if (win->y < ymin)
		win->y = ymin;
	if (win->y > ymax)
		win->y = ymax;
}
void zoomlt(WINDOW* win, int dx, int dy)			//zoom when licked on left top
{
	int wmin, wmax, hmin, hmax,x2,y2;
	x2 = win->x+win->w-1;							//fix right bottom corner
	y2 = win->y + win->h-1;
	wmin = 200;                        //legitimate range
	wmax = x2-20;
	hmin = 2*FR+12+TEXTHEIGHT;
	hmax = y2-40;
	win->w -= dx;
	win->h -= dy;
	if (win->w < wmin)             //do not break the win min max size
		win->w = wmin;
	if (win->w > wmax)
		win->w = wmax;
	if (win->h < hmin)
		win->h= hmin;
	if (win->h > hmax)
		win->h = hmax;
	win->x = x2 - win->w + 1;
	win->y = y2-win->h+1;
}
void zoomrb(WINDOW* win, int dx, int dy)
{
	int wmin, wmax, hmin, hmax, x1, y1;
	x1 = win->x ;             //LT corner
	y1 = win->y ;
	wmin = 200;                   //legitimate range
	wmax = 620-x1;
	hmin = 2 * FR + 12 + TEXTHEIGHT;
	hmax = 459-y1;
	win->w += dx;
	win->h += dy;
	if (win->w < wmin)           //do not break the win min max size
		win->w = wmin;
	if (win->w > wmax)
		win->w = wmax;
	if (win->h < hmin)
		win->h = hmin;
	if (win->h > hmax)
		win->h = hmax;
}
void zoomrt(WINDOW* win, int dx, int dy)
{
	int wmin, wmax, hmin, hmax, x1, y2;
	x1 = win->x;
	y2 = win->y + win->h - 1;
	wmin = 200;                    //legitimate range
	wmax = 620 - x1;
	hmin = 2 * FR + 12 + TEXTHEIGHT;
	hmax = y2 - 40;
	win->w += dx;
	win->h -= dy;
	if (win->w < wmin)         //do not break the win min max size
		win->w = wmin;
	if (win->w > wmax)
		win->w = wmax;
	if (win->h < hmin)
		win->h = hmin;
	if (win->h > hmax)
		win->h = hmax;
	win->y = y2 - win->h + 1;       //adjust
}
void zoomlb(WINDOW* win, int dx, int dy)
{
	int wmin, wmax, hmin, hmax, x2, y1;
	x2 = win->x + win->w - 1;
	y1 = win->y;
	wmin = 200;
	wmax = x2-20;
	hmin = 2 * FR + 12 + TEXTHEIGHT;
	hmax = 459 - y1;
	win->w -= dx;
	win->h += dy;
	if (win->w < wmin)        //do not break the win min max size
		win->w = wmin;
	if (win->w > wmax)
		win->w = wmax;
	if (win->h < hmin)
		win->h = hmin;
	if (win->h > hmax)
		win->h = hmax;
	win->x = x2-win->w+1;   //adjust
}
void appendrow(WINDOW* win)   //append row for window
{
	ROWNODE* begin;
	begin = win->begin;
	if (begin == NULL)
	{
		win->begin = newrow();    //alloc
	}
	else
	{
		while (begin->next)      //find last row
			begin = begin->next;
		begin->next = newrow();    //alloc
	}
	win->rownum++;         //maintain count
}
int presskey(WINDOW* win, int* ro, int* co, int key)//return 0 when directional keys
{
	ROWNODE* row,*rownext,*rowprev;
	CHARNODE* col,*colprev ;
	int toplen,downlen;
	row = queryrowindex(win, *ro);
	if(key=='\n')			//Enter
	{
		if (*co == row->len)
			insertrowafter(win,row);			// add a blank to next line
		else if (*co == 0)
			insertrowbefore(win, row);				//add a blank to prev line
		else
		{
			colprev = querycharindex(row->begin, *co-1);
			col = colprev->next;		
			colprev->next = NULL;					//cut now row link
			insertrowafter(win,row);				//add new row
			rownext = row->next;					//push the left of the row to the next row
			rownext->begin = col;
			updaterowlen(row);
			updaterowlen(rownext);
		}
		++* ro;				//adjust cursor position
		*co = 0;
		return 1;
	}
	else if (key == 8)		//backspace
	{
		if (*co == 0)
		{
			if (*ro > 0)   //not the first row, first col
			{
				rowprev= queryrowindex(win, *ro-1);
				connectcharlist(rowprev, row->begin);  //prev row connect the next row
				eraserow(win, row);     //erase this row because empty
				--* ro;             //cursor need to move
				*co = rowprev->len;		//update cursor col
			}
		}
		else        //only erase the char
		{
			erasechar_id(row, *co);
			--* co;     //update cursor col
		}
		return 1;
	}
	else if (key == 0x4B00)		//left
	{
		if(*co)   //move cursor left
			--*co;
		else if(*ro)   //move to prev row
		{
			--*ro;
			*co = queryrowindex(win,*ro)->len;  
		}
		return 0;
		
	}
	else if (key == 0x4800)		//up
	{
		if (*ro)  //move the cursor up
		{
			toplen =  queryrowindex(win, *ro-1)->len;
			if (*co > toplen)  //corner case
				*co = toplen;  //exceed then move to the rightmost
			--*ro;
		}
		return 0;
	}
	else if (key == 0x4D00)			//right
	{
		if (*co<row->len)  //not the rightmost char
			++* co;
		else if (*ro < win->rownum - 1)  //move to begin of next row
		{
			++* ro;
			*co = 0;
		}
		return 0;
	}
	else if (key == 0x5000)		//down
	{
		if (*ro<win->rownum-1)   //not the last row
		{
			downlen = queryrowindex(win, *ro+1)->len;
			if (*co > downlen)
				*co = downlen;     //move to it
			++* ro;
		}
		return 0;
	}
	else			//printable keys
	{
		if (*co == row->len)   //append at the last
			appendchar(row, key);
		else
		{
			col = querycharindex(row->begin,*co);  //find pos
			insertcharbefore(row,col, key); //insert char
		}
		++* co;
		return 1;
	}
}

WINDOW* newwindow(int x,int y,int w,int h,int mode)  //alloc new window
{
	WINDOW* window = (WINDOW*)malloc(sizeof(WINDOW)); //mem
	window->x = x;			//20
	window->y = y;			//100
	window->w =w;			//600
	window->h = h;			//320
	window->mode = mode;   //set mode
	window->rownum = 1;   //default one row
	window->begin = newrow(); //empty one
	window->next = NULL;
	window->firstrow = 0;  //visible at left top
	window->firstcol = 0;
	memset(window->filename, '\0', 40);  //set name empty
	return window;
}
int multiwindowclick(WINDOW* winhead, int* mode, WINDOW** focuswin,int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2) //handle multi window issue
{
	//remember ,ever you modify focus window,the window main should be maintained
	WINDOW * win;
	int tmp;

	tmp = clickWindow(*focuswin, focusrow, focuscol,crow1,ccol1,crow2,ccol2);   //first the focus
	if (tmp >= 3)			//zoom or drag
	{
		*mode = tmp;   //global mode
		return 1;
	}
	else if (tmp == 2)   //need refresh all
		return 1;
	else if (tmp == 1)  //default effect
		return 0;
	else if (tmp == -1)   //need refresh all
		return 1;
	//not on this win
	for (win = winhead; win; win = win->next)
	{
		if (win == *focuswin)  //skip focus
			continue;
		tmp = clickWindow(win, focusrow, focuscol,crow1,ccol1,crow2,ccol2);  //handle this

		if (tmp >= 3)			//zoom or drag
		{
			(*focuswin)->mode = 2;
			*focuswin = win;
			*mode = tmp;
			return 1;
		}
		else if (tmp == 2)   //for back window
		{	
			win->mode = 1;   //set this to top mode
			(*focuswin)->mode = 2;  //set top to back

			*focuswin = win;   //exchange
			*mode = 1;   //normal mode

			return 1;
		}
	}
	return -1;
}
void multiwindowdragzoom(WINDOW** pfocuswin, int mousex, int mousey, int lastx, int lasty)  //handle zoom drag
{
	WINDOW* focuswin = *pfocuswin;
	if (focuswin->mode == 3)               //drag
		dragWindow(focuswin, mousex - lastx, mousey - lasty);
	else if (focuswin->mode == 4)      //LT
		zoomlt(focuswin, mousex - lastx, mousey - lasty);
	else if (focuswin->mode == 5)       //RB
		zoomrb(focuswin, mousex - lastx, mousey - lasty);
	else if (focuswin->mode == 6)       //LB
		zoomlb(focuswin, mousex - lastx, mousey - lasty);
	else if (focuswin->mode == 7)  //RT
		zoomrt(focuswin, mousex - lastx, mousey - lasty);
}
