#include <graphics.h>
#include <string.h>
#include "docwin.h"
#include "mouse.h"
#include "hz.h"
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"
#include "docwin.h"
#include "alg.h"
void get_dragbar_range(int low, int up, int l, int vis, int tot,int* lx,int* rx)		//cal scroll bar draw range
{
	int range, r;
	if (tot <= 0)					                                                      //tot col/row must be positive
		tot = 1;
	r = l +vis;
	range= up -low - 10;																	//length
	*lx=range*l/tot;                                                                    //block
	if (r > tot)                                                                       //do not exceed
		r = tot ;
	*rx = range * r / tot;                                         
	*lx = (*lx) + low+5;                                                            
	*rx = (*rx) + low + 5;
}
void draw_horz_dragbar(int x1,int y1,int x2,int y2,int l,int viscol,int tot)		//x方向的拖动条绘制
{
	int lx, rx;
	get_dragbar_range(x1, x2, l, viscol, tot,&lx,&rx);
	drawbar(x1, y1, x2, y2, CYAN);                                //draw back
	drawbar(lx, y1, rx, y2, LIGHTGRAY);                           //draw bar
}
void draw_vert_dragbar(int x1, int y1, int x2, int y2, int l, int visrow, int tot)		//y方向的拖动条绘制
{
	int ly, ry;
	get_dragbar_range(y1, y2, l, visrow, tot, &ly, &ry);
	drawbar(x1, y1, x2, y2, CYAN);                  //draw back
	drawbar(x1, ly, x2, ry, LIGHTGRAY);                  //draw bar
}
void drawWindow(WINDOW* win, int crow1, int ccol1, int crow2, int ccol2,ACNode* ac, int focusrow, int focuscol) //handle draw document window issues
{

	int x1, y1, x2, y2;
	int x0, x, y, r = 0, c = 0,mxc=0,viscol=0,visrow=0,i=0;
	ROWNODE* rowp;
	CHARNODE* charp;
	char* highlight,tmp[15];
	if (!win)
		return;
	x1 = win->x;//four corners of the window
	y1 = win->y;
	x2 = win->x + win->w - 1;
	y2 = win->y + win->h - 1;

	x0 = x1 + FR + 5;		//start of text output
	y = y1 + FR + 5;
	rowp = win->begin;
	mxc = getmaxcol(win);
	//printf("DRAW  %d %d %d %d\n", crow1, ccol1, crow2, ccol2);
	if (crow1 >= 0 && crow2 >= 0 && ccol1 >= 0 && ccol2 >= 0)             //avoid illegal values of select range
	{
		if (crow1 > crow2)
		{
			swap(&crow1, &crow2);
			swap(&ccol1, &ccol2);
		}
		else if (crow1 == crow2 && ccol1 > ccol2)
			swap(&ccol1, &ccol2);
	}
	

	if (win->mode <= 2)                   //top or back, not drag zoom
	{
		drawbar(x1, y1, x2, y2, BLUE);				//main blue window
		setcolor(WHITE);
		setlinestyle(SOLID_LINE,0, 1);
		line(x1 + FR/2,y1+FR/2,x2-FR/2,y1+FR/2);            //draw the inner frame
		line(x1 + FR / 2, y1 + FR / 2, x1 + FR / 2, y2 -FR / 2);
		line(x1 + FR / 2, y2 - FR / 2, x1+19,y2-FR/2);
		line(x1 + 59, y2 - FR / 2, x1 + 79, y2 - FR / 2);
		line(x2-FR/2,y1+FR/2,x2-FR/2,y1+FR+5);
		
		line(x1 + FR , y2 - FR , x1 + 19, y2 - FR );     //draw outer frame
		line(x1 + 59, y2 - FR, x1 + 79, y2 - FR);
		line(x1 + FR , y1 + FR , x2 - FR , y1 + FR );
		line(x1 + FR , y1 + FR , x1 + FR , y2 - FR );
		line(x2 - FR , y1 + FR, x2 - FR , y1 + FR + 5);

		sprintf(tmp, "%d,%d", focusrow+1,focuscol+1);    //show the row num,col num
	
		drawtext(x1 + 29, y2-FR-5,tmp,16,20,8,WHITE);

		for (; rowp != NULL; rowp = rowp->next)
		{
			if (r < win->firstrow)                 //draw start from first row
			{
				r += 1;
				continue;
			}
			
			if (y + TEXTHEIGHT > y2 - FR - 5)				//do not draw out of border
				break;
			highlight = get_highlight(ac,rowp);          //get highlight state
			charp = rowp->begin;           
			x = x0;
			c = 0;
			if (charp != NULL)
			{
				for (; charp != NULL; charp = charp->next)
				{
					if (c < win->firstcol)            //start from the first col
					{
						c += 1;
						continue;
					}
					if (x + TEXTWIDTH > x2 - FR - 5)        //do not draw out of border
						break;
					if (r > crow1 || (r == crow1 && c >= ccol1))      //selected area ,use special color
					{
						if (r < crow2 || (r == crow2 && c <= ccol2))
						{
							drawbar(x, y, x + TEXTWIDTH - 1, y + TEXTHEIGHT - 1, CYAN);
						}
					}
					if (highlight[c])                  //hight text render
						drawchar(x, y, charp->text, RED);
					else
						drawchar(x, y, charp->text, WHITE);

					x += TEXTWIDTH;
					c += 1;
				}
			}
			free(highlight);             //free memory of state
			r += 1;
			y += TEXTHEIGHT;
		}
		viscol = visiblecolcount(win);           //count visible col,row
		visrow = visiblerowcount(win);
		draw_horz_dragbar(x1 + 80, y2 - FR - 5, x2 - FR - 5, y2, win->firstcol, viscol,mxc);       //draw bar
		draw_vert_dragbar(x2 - FR - 5, y1 + FR + 5, x2, y2 - FR - 5, win->firstrow, visrow, win->rownum );
	}
	else
	{
		drawrect(x1 + FR / 2, y1 + FR / 2, x2 - FR / 2, y2 - FR / 2, SOLID_LINE, 3, CYAN);    //for drag zoom mode special draw
	}
}

void drawcursor(WINDOW* win, int row, int col, int* flag)        //show the cursor
{
	int x1, y1, x, y,vr,vc;
	x1 = win->x;                      //four corners of the window
	y1 = win->y;
	x = x1 + FR + 5;		//start of text output
	y = y1 + FR + 5;
	row -= win->firstrow;
	col -= win->firstcol;
	vr = visiblerowcount(win);
	vc = visiblecolcount(win);
	if (row >= vr || row < 0 || col >= vc || col < 0)         //cursor not in screen
		return;
	x = x + col * TEXTWIDTH;                        //cal x,y
	y = y + row * TEXTHEIGHT;
	if (*flag)                                  //line shine flag 
		setcolor(WHITE);                        //show   
	else
		setcolor(BLUE);                         //hide
	line(x, y + 2, x, y + TEXTHEIGHT - 2);          //shining line
	(*flag) ^= 1;                                //state reverse
}

int visiblecolcount(WINDOW* win)      //visible count
{
	int  range, ret;
	range = win->w - 10 - 2 * FR;    //range on window
	ret = range / TEXTWIDTH;         
	return ret;  
}

int visiblerowcount(WINDOW* win)   //visible count
{
	int  range, ret;
	range = win->h - 10 - 2 * FR;       //range on window
	ret = range / TEXTHEIGHT;              
	return ret;
}
