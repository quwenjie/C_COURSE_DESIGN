#ifndef TEXT_H
#define TEXT_H
void write_text(WINDOW* wp, char* s);
void two_row(WINDOW* win, ROWNODE* row, int col);
void append_string(char* buffer, ROWNODE* row);
void add_two_row(WINDOW* win, ROWNODE* r1, ROWNODE* r2);
void fput_row(int c1, int c2, ROWNODE* row, char* buffer);
void cut_row(int c1, int c2, ROWNODE* row);
void cutchar(ROWNODE* rn, int id);
void cut_a_row(WINDOW* focuswin, int id);
int find_row_id(WINDOW* focuswin, ROWNODE* row);
void getstring(char* begin, char* end, char* s);
#endif
