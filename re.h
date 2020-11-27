#ifndef RE_H
typedef struct RE_OP
{
	int pattern;
	WINDOW* wp;
	//11 for cut,12 for paste,21 for close a window,22 for close all window,31 for open a window
	//attention!!! 41 for add a character,42 for delete a character
	//51 for revocation,52 for renewal
	ROWNODE* begin;
	int col1, col2, row1, row2;
	RE_OP* next;
	RE_OP* last;

}RE_OP;

typedef struct 
{
	RE_OP* p;
	int len;
}RE_PARA;

void init_re_para(RE_PARA* rep);
void redo(RE_PARA* rep);
void undo(RE_PARA* rep);
void judge_redo(RE_OP* rp);
void reverse_cut(RE_OP* rp);
void reverse_paste(RE_OP* rp);
void put_text(RE_OP* rp, char* str);
void record_cut(WINDOW* win, int r1,int c1, int r2, int c2, RE_PARA* rep);
void record_paste(WINDOW* win, int r1, int c1, int r2, int cb, int ce, RE_PARA* rep);
RE_OP* new_re(RE_PARA* rep);
void judge_undo(RE_OP* rp);
ROWNODE* invert_rn_text(WINDOW* win, int r1, int c1, int r2, int c2);
#endif 

