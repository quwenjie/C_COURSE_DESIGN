#ifndef CLOSEWINDOW_H
#define CLOSEWINDOW_H
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
void close_a_window(WINDOW** focuswin, WINDOW** win);    //�رյ�ǰ����
int close_all_windows(WINDOW* win);                     //�ر����д���
WINDOW* popwin(WINDOW* win, WINDOW* focus);       //ɾ�����������ж�Ӧ����
#endif
