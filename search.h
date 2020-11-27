#ifndef SEARCH_H
#define SEARCH_H
//MENU-SEARCH FAMILY FUNCTIONS 
#include "docwin.h"
#include "LST.h"
#include "draw.h"
#include "hz.h"
#include "mouse.h"
#include "keyboard.h"
#include "alg.h"
void drawfindwin();
void getfindtext(char* text,int* casesense,int*dir);
void find_opt(WINDOW* focuswin, int* focusrow,int* focuscol, int* ccrow1, int* ccol1, int* crow2, int* col2);
void drawreplacewin();
void getreplacetext(char* text, char* text2, int* casesense, int* dir, int* all);
void replace_all(WINDOW* focuswin, int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2, char text1[], char text2[], int case_sensitive, int dir );
void replace(WINDOW* focuswin, int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2, char text1[], char text2[], int case_sensitive, int dir);
void replace_opt(WINDOW* focuswin, int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2);
void drawjumpwin();
int getjumprow();
void jump_opt(WINDOW* focuswin, int* focusrow, int* focuscol);
#endif
