#ifndef EDIT_H
#define EDIT_H
void paste(WINDOW* focuswin, int r1, int c1, char* str);
void copy(WINDOW* focuswin, int r1, int c1, int r2, int c2, char* str);
void cut(WINDOW* focuswin, int r1, int c1, int r2, int c2, char* str);
void selectall(WINDOW* focuswin, int* r1, int* c1, int* r2, int* c2);
int not_edit(int x);
#endif
