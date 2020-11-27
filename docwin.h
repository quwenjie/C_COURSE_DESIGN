#ifndef DOCWIN_H
#define DOCWIN_H
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include "LST.h"
#include "alg.h"
#define FR 16
#define TEXTHEIGHT 20
#define TEXTWIDTH 10

void drawWindow(WINDOW* win, int crow1, int ccol1, int crow2, int ccol2, ACNode* ac,int focusrow,int focuscol);//handle draw document window issues	
void drawcursor(WINDOW* win,int row,int col,int *flag);
int visiblerowcount(WINDOW* win);
int visiblecolcount(WINDOW* win);

void ACAutomatoninit(ACNode** root);
char* get_highlight(ACNode* root, ROWNODE* row);		//get highlight state


int clickWindow(WINDOW* win,int *fr,int* fc, int* crow1, int* ccol1, int* crow2, int* ccol2);				//handle click document window issues
void get_dragbar_range(int low, int up, int l, int vis, int tot, int* lx, int* rx);
int onWindow_horz_bar_click(WINDOW* win,int x,int y);
int onWindow_vert_bar_click(WINDOW* win, int x, int y);
void bar_horz(WINDOW* win, int dx);
void bar_vert(WINDOW* win, int dy);
int multiwindowclick(WINDOW* winhead, int* mode, WINDOW** focuswin, int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2);

void multiwindowdragzoom(WINDOW** focuswin, int mousex, int mousey, int lastx, int lasty);
void zoomlt(WINDOW* win, int dx, int dy);
void zoomlb(WINDOW* win, int dx, int dy);
void zoomrt(WINDOW* win, int dx, int dy);
void zoomrb(WINDOW* win, int dx, int dy);

WINDOW* newwindow(int x, int y, int w, int h, int mode);
void appendrow(WINDOW* win);

int presskey(WINDOW* win, int* row,int* col,int key);

#endif
