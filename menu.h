#ifndef MENU_H
#define MENU_H
#include "LST.h"
#include "re.h"
void edit(WINDOW* focuswin, int* r1, int* c1, int* r2, int* c2, int rr, int cc, RE_PARA* rep);
void file(WINDOW** wp, WINDOW** focuswin, char s[12][21]);
int menu(WINDOW** wp, WINDOW** focuswin, int* r1, int* c1, int* r2, int* c2, int* rr, int* cc, RE_PARA* rep);
void close(WINDOW** focuswin, WINDOW** win);
void help();
void search(WINDOW* focuswin, int* focusrow, int* focuscol, int* ccrow1, int* ccol1, int* crow2, int* col2);
#endif


