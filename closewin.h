#ifndef CLOSEWINDOW_H
#define CLOSEWINDOW_H
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
void close_a_window(WINDOW** focuswin, WINDOW** win);    //关闭当前窗口
int close_all_windows(WINDOW* win);                     //关闭所有窗口
WINDOW* popwin(WINDOW* win, WINDOW* focus);       //删除窗口链表中对应窗口
#endif
