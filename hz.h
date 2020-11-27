#ifndef HZ_H
#define HZ_H
#include <graphics.h>
#ifndef __BORLANDC__
#define settextstyle(x,y,z)   setfont(-z*2,0,"Arial")
#endif
void drawtext(int x, int y, char* s, int fontsize, int partChinese, int partEnglish, int color);
void drawchar(int x, int y, char c,int col);
#endif