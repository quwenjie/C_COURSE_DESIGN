#ifndef REFRESH_H
#define REFRESH_H
#include "alg.h"
typedef struct
{
	char fn[20];
	int cy;
	int clickstate;
}CHECKBOX;

void refresh(char* s,WINDOW *win);
void refreshusers(WINDOW* win);
void refresh_windows(WINDOW* winhead, WINDOW* focuswin, int crow1, int ccol1, int crow2, int ccol2, int focusrow, int focuscol, int* cursorflag,int* state, ACNode* ac);
void update_first_rowcol(WINDOW* win, int focusrow, int focuscol);		//刷新 firstrow col 准则：保持光标能被看到
void update_focus_rowcol(WINDOW* win, int *focusrow, int* focuscol);		//刷新 focus row col 准则：保持光标能被看到
void refreshbg();
void request(WINDOW* win);
void open_all(WINDOW* win);
CHECKBOX* initcheckbox(char* s1, int n);
void clickcheckbox(CHECKBOX* cb, int num, int mx, int my);
void open_selected(CHECKBOX* cb, int n, WINDOW* win);
#endif // !REFRESH_H