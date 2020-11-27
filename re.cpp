#include <stdio.h>
#include <stdlib.h>
#include "LST.h"
#include "text.h"
#include "edit.h"
#include "re.h"

#ifdef _MSC_VER
#pragma warning(disable:4996)
#else
#include <dos.h>
#endif

/***************************************init_re_para****************************************
function:
	init re-operations' container	
parameters: 
	focuswin:
	rep:editing paragraph container
return:
	none
****************************************************************************************/
void init_re_para(RE_PARA* rep)
{
	rep->len = 0;
	rep->p = NULL;
}

/***************************************redo****************************************
function:
	redo	
parameters: 
	rep:editing paragraph container
return:
	none
****************************************************************************************/
void redo(RE_PARA* rep)
{
	if (rep->p == 0)
		return;
 	//RE_OP* last = (rep->p)->last;
	if (rep->len == 0)
	{
		rep->len++;
		judge_redo(rep->p);
	}
	//rep->p = last;
}

/***************************************undo****************************************
function:
	undo	
parameters: 
	rep:editing paragraph container
return:
	none
**************************************************************************************/
void undo(RE_PARA* rep)
 {
	if (rep->len == 1)
	{
		rep->len--;
		judge_undo(rep->p);
		rep->p = (rep->p)->next; 
	}
}

/***************************************judge_redo*************************************
function:
	judge redo state:11 for cut,12 for paste	
parameters: 
	rep:editing paragraph container
return:
	none
**************************************************************************************/
void judge_redo(RE_OP* rp)
{
	switch (rp->pattern)
	{
		case 11:
			reverse_cut(rp);
			break;
		case 12:
			reverse_paste(rp);
			break;
	}	
}

/***************************************judge_undo*************************************
function:
	judge undo state:11 for cut,12 for paste	
parameters: 
	rep:editing paragraph container
return:
	none
**************************************************************************************/
void judge_undo(RE_OP* rp)
{
	char str[] = "retxt";
	switch (rp->pattern)
	{
		case 11:
			cut(rp->wp, rp->row1, rp->col1, rp->row2, rp->col2, str);
			break;
		case 12:
			paste(rp->wp, rp->row1, rp->col1, str);
			break;
	}
}

/***************************************reverse_cut*************************************
function:
	reverse cut operation	
parameters: 
	rp:operation pointer
return:
	none
**************************************************************************************/
void reverse_cut(RE_OP* rp)
{
	char str[] = "retxt";
	put_text(rp, str);
	paste(rp->wp, rp->row1, rp->col1, str);
}

/***************************************reverse_paste*************************************
function:
	reverse paste operations	
parameters: 
	rp:operation pointer
return:
	none
**************************************************************************************/
void reverse_paste(RE_OP* rp)
{
	char str[] = "retxt";
	//put_text(rp, str);
	cut(rp->wp, rp->row1, rp->col1, rp->row2, rp->col2, str);
}


/***************************************put_text*************************************
function:
	put text in container 	
parameters: 
	rp:operation pointer
	str:retxt name
return:
	none
**************************************************************************************/
void put_text(RE_OP* rp, char* str)
{
	ROWNODE* rn;
	CHARNODE* cn;
	FILE* fp;
	fp = fopen(str, "w");
	for (rn = rp->begin; rn != NULL; rn = rn->next)
	{
		for (cn = rn->begin; cn; cn = cn->next)
		{
			fputc(cn->text, fp);
		}
		fputc('\n', fp);
	}
	fclose(fp);
}

/***************************************record_cut*************************************
function:
	record cut operations	
parameters: 
	win:
	(r1,c1,r2,c2): pointers -- paragraph position for editing
	(rr,cc):pointers -- cursor position for editing
	rep:editing paragraph container
return:
	none
**************************************************************************************/
void record_cut(WINDOW* win, int r1, int c1, int r2, int c2, RE_PARA* rep)
{
	RE_OP* re = new_re(rep);
	re->wp = win;
	re->pattern = 11;
	re->row1 = r1;
	re->row2 = r2;
	re->col1 = c1;
	re->col2 = c2;
	re->begin = invert_rn_text(win, r1, c1, r2, c2);
	rep->p = re;
	rep->len = 0;
}

/***************************************record_paste*************************************
function:
	record paste operations	
parameters: 
	win:
	(r1,c1,r2): paragraph position for editing
	(cb,ce):copy colunms position
	rep:editing paragraph container
return:
	none
**************************************************************************************/
void record_paste(WINDOW* win, int r1, int c1, int r2, int cb, int ce,RE_PARA* rep)
{
	RE_OP* re = new_re(rep);
	re->wp = win;
	re->pattern = 12;
	re->row1 = r1;
	re->row2 = r2 + r1;
	re->col1 = c1;
	if (re->row1 == re->row2)
		re->col2 = re->col1 + ce - cb;
	else 
		re->col2 = ce+1;
	rep->len = 0;
	rep->p = re;
}

/***************************************new_re*************************************
function:
	new re_operations
parameters: 
	rep:editing paragraph container
return:
	none
**************************************************************************************/
RE_OP* new_re(RE_PARA* rep)
{
	RE_OP* re;
	if (rep->p == NULL)
	{
		rep->p = (RE_OP*)malloc(sizeof(RE_OP));
		(rep->p)->last = NULL;
		(rep->p)->next = NULL;
	}
	else
	{
		re = (RE_OP*)malloc(sizeof(RE_OP));
		re->last = rep->p;
		(rep->p)->next = re;
		re->next = NULL;
		rep->p = re;
	}

	return rep->p;
}

/***************************************invert_rn_text*************************************
function:
	use window structure to save text in container
parameters: 
	win:
	(r1,c1,r2,c2): paragraph position for editing
	win:
return:
	begin of the created rownode
**************************************************************************************/
ROWNODE* invert_rn_text(WINDOW* win, int r1, int c1, int r2, int c2)
{
	int flag = 1;
	FILE* fp;
	ROWNODE* rn = newrow();
	ROWNODE* rne = rn;
	ROWNODE* rn1;
	CHARNODE* cn = 0;
	CHARNODE* cn1;
	int i;
	char buffer[100];
	fp = fopen("copytext", "r");
	while (fgets(buffer, 100, fp))
	{
		if (flag == 0)
		{
			rn1 = newrow();
			rn->next = rn1;
			rn = rn1;
		}
		rn->begin = cn1 = newchar(buffer[0]);
		for (i = 1; buffer[i]!='\n'; i++)
		{
			cn = newchar(buffer[i]);
			cn1->next = cn;
			cn1 = cn;
		}
		flag = 0;
	}
	fclose(fp);
	return rne;
}