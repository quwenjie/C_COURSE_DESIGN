#include <stdio.h>
#include "LST.h"
#include "findwin.h"

/***************************************dzwindow****************************************
function:
	judge if there is window being dragging or zooming or not
parameters:
	wp:winhead
return:
	state:1 for finding such window,0 for none
****************************************************************************************/
int dzwindow(WINDOW* wp)
{
	WINDOW* p;
	for (p = wp; p; p = p->next)
	{
		if (p->mode >= 3)
			return 1;
	}
	return 0;
}

/***************************************chosenwin****************************************
function:
	find chosen window
parameters:
	wp:winhead
return:
	chosen window's address
****************************************************************************************/
WINDOW* chosenwin(WINDOW* wp)
{
	WINDOW* p;
	for (p = wp; p; p = p->next)
	{
		if (p->mode == 1)
			return p;
	}
	return NULL;
}