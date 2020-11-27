#include "hz.h"
#include "mouse.h"
#include "docwin.h"
#include "keyboard.h"
#include "lst.h"
#include "menu.h"
#include "findwin.h"
#include "draw.h"
#include "refresh.h"
#include "login.h"
#include "search.h"
#include "alg.h"
#include <stdio.h>
#ifdef _MSC_VER
#pragma warning(disable:4996)
#else
#include <dos.h>
#endif
#define SHOW_CONSOLE
#include <graphics.h>

extern int mousex, mousey, mousepress,prev_mx,prev_my;
int main()
{
	int gd = VGA, gm = VGAHI;                           //environment
	int  lastx = -1, lasty = -1, tmp,tmp1=0, mode = 1;  //mouse
	int focusrow = 0, focuscol = 0, cursorflag = 1;      //cursor
	int input;                                           //key
	int crow1 = -1, crow2 = -1, ccol1 = -1, ccol2 = -1;   //select range
	int prevpress = 0,refreshstate=3;                    //refresh optimize

	RE_PARA re;
	
	WINDOW* winhead, * win = 0, * focuswin = 0;    //window
	ACNode* ac ;                                    //ac automation
	ACAutomatoninit(&ac);
	initgraph(&gd, &gm, "C:\\BORLANDC\\BGI");            //init graph window
	mouseinit();
	initbg();
	login();
	refreshbg();
	init_re_para(&re);
	winhead = newwindow(20, 150, 400, 300, 1);         //init first window
	strcpy(winhead->filename, "noname1"); 
	focuswin = winhead;
	while (1)
	{
		//show filename 
		mousemove();

		//refresh
		if (winhead)            //have window
			refresh(focuswin->filename, winhead);
		else
			drawbar(0, 0, 640, 20, WHITE);

		//handle click menu issue
		tmp1 = menu(&winhead, &focuswin, &crow1, &ccol1, &crow2, &ccol2, &focusrow, &focuscol, &re);
		if (tmp1)      //refresh state have 2 ,all 
		{
			refreshstate |= 2;
		}
		if (tmp1 == 1)        //after using menu,refresh focus point
		{
			focusrow = focuscol = 0;
		}

		if (mousepress)
		{
			if (!tmp1)			//no one clicks the menu
			{
				if (mode < 3)
				{
					//printf("AI1: %d %x\n", tmp1, focuswin);
					tmp = multiwindowclick(winhead, &mode, &focuswin, &focusrow, &focuscol, &crow1, &ccol1, &crow2, &ccol2); //multi window click
					//printf("AI2: %d %x\n",tmp1,focuswin);
					if (tmp == 1)
						refreshstate |= 2;     //if top window changed refresh all
					else if (tmp == 0)
						refreshstate |= 1;     //else refresh top
				}
				else
				{
					multiwindowdragzoom(&focuswin, mousex, mousey, prev_mx, prev_my);    //multi window drag 
					refreshstate |= 2;        //refresh all
				}
			}
			if (lastx == -1)  //record click start
			{
				lastx = mousex;
				lasty = mousey;
			}
		}
		else
		{
			if (mode >= 3)       //exchange mode to normal
			{
				focuswin->mode = 1;
				mode = 1;
				refreshstate |= 2;
			}
			lastx = -1;
			lasty = -1;
		}
		input = get_c();  //handle keyboard input
		if (input != 0)
		{
			presskey(focuswin, &focusrow, &focuscol, input);   //handle key press
			update_first_rowcol(focuswin, focusrow, focuscol); //adjust first,row,col
			refreshstate |= 1;
		}
		refresh_windows(winhead, focuswin, crow1, ccol1, crow2, ccol2, focusrow, focuscol, &cursorflag,&refreshstate,ac);  //render windows
		refreshstate = 0;          //clear flag
		drawmouse();
		//printf("MO SHI SHIBIE %d %d\n", focuswin->firstrow, focuswin->firstcol);
#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif

	}

	closegraph();      //end
	return 0;
}