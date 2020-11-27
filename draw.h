#ifndef DRAW_H
#define DRAW_H
//画图函数和支持画图功能的函数
void drawbar(int x1, int y1, int x2, int y2, int color);
void drawhint(char* s1, char* s2);
void drawlogin();
void drawletter(int x1, int y1, char c);
void drawsubwin(char* s1, char* s2);
void drawsubmenu(int x, char s[][21], int num);
void mydelay(int x);
void drawmenu();
void fillcircle(int x, int y, int r, int col);
void drawbg();
void drawattention(char* title, char s1[][50], int line);
int drawyesorno(char* s1, char* s2);
void drawfilename(char *fn);
void drawrect(int x1,int y1,int x2,int y2,int style,int linesize,int col);
void initbg();
void drawbutton(int colortext, int colorbar, int x1, int y1, int x2, int y2, char* s, int x, int y);
void drawBCtitle();
int showfiles(char s[12][21]);
#endif