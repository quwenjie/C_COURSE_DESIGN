#ifndef FILE_OP_H
#define FILE_OP_H
void del_same_files(char* fn);
void update_files(char* fn);
int file_num(char s[12][21]);
//int click_files_menu(int mx, int my, char s[12][21]);
void open_click_file(int x, int y, char s[12][21], WINDOW** wp,WINDOW** focuswin);
int  open_file_name(WINDOW* win, char* s, WINDOW** focuswin);
WINDOW* open_file(WINDOW** wp);
int check_file(char* fn, WINDOW* wp);
WINDOW* new_file(WINDOW** wp);
void save_file(WINDOW* wp);
void save_as(WINDOW* wp);
void save_all(WINDOW* wp);
#endif
