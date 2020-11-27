#include "hz.h"
#include "lst.h"
#include "findwin.h"
#include "text.h"
#ifdef _MSC_VER
#pragma warning(disable:4996)
#else
#include <dos.h>
#endif
#include <graphics.h>
#include <string.h>
#include <stdio.h>

/***************************************getstring****************************************
function:get string between adress begin and address end
parameters:
	begin
	end
	s:origin string
return:
	none
*******************************************************************************************/
void getstring(char* begin, char* end, char* s)
{
	char* p;
	int i;
	for (i = 0, p = begin; p <= end; p++, i++)
		s[i] = *p;
	s[i] = '\0';
}

/***************************************write_text****************************************
function:select all text in focuswin
parameters:
	wp:win
	s:file name
return:
	none
*******************************************************************************************/
void write_text(WINDOW* wp, char* s)
{
	FILE* fp;
	fp = fopen(s, "w");
	ROWNODE* rp;
	CHARNODE* cp;
	for (rp = wp->begin; rp; rp = rp->next)
	{
		for (cp = rp->begin; cp; cp = cp->next)
			fputc(cp->text, fp);
		fputc('\n', fp);
	}
	fclose(fp);
}

/***************************************two_row****************************************
function:cut a row into two rows
parameters:
	col:cut posisition
return:
	none
*******************************************************************************************/
void two_row(WINDOW* win, ROWNODE* row, int col)
{
	int i;
	CHARNODE* cn;
	insertrowafter(win, row);
	for (i = col; i <= row->len; )
	{

		if (row->begin)
		{
			cn = querycharindex(row->begin, i - 1);
			appendchar(row->next, cn->text);
			cutchar(row, i);
		}
		else
			i++;
	}
}

/*************************************append_string****************************************
function:append a string behind certain row
parameters:
	buffer:text to append
	row:row to be appended
return:
	none
*******************************************************************************************/
void append_string(char* buffer, ROWNODE* row)
{
	int i = 0;
	for (i = 0; buffer[i] != '\n'; i++)
		appendchar(row, buffer[i]);
}

/*************************************append_string****************************************
function:append a string behind certain row
parameters:
	buffer:text to append
	row:row to be appended
return:
	none
*******************************************************************************************/
void add_two_row(WINDOW* win, ROWNODE* r1, ROWNODE* r2)
{
	CHARNODE* cn;
	if (r2->begin == NULL)
	{
		cut_a_row(win, find_row_id(win, r2));
		return;
	}
	for (cn = r2->begin; cn; cn = cn->next)
		appendchar(r1, cn->text);
	cut_a_row(win, find_row_id(win, r2));
}

/*************************************fput_row****************************************
function:transfer the rows into char text
parameters:
	c1:row begin
	c2:row end
	row:address of row begin
	buffer:text container
return:
	none
*******************************************************************************************/
void fput_row(int c1, int c2, ROWNODE* row, char* buffer)
{
	CHARNODE* cn;
	int i, j;

	for (i = c1, j = 0; i <= c2; i++, j++)
	{
		cn = querycharindex(row->begin, i - 1);
		if (!cn)
		{
			buffer[j] = '\n';
			buffer[j + 1] = '\0';
			return;
		}
		else
		{
			buffer[j] = cn->text;
		}
	}
	buffer[j] = '\n';
	buffer[j + 1] = '\0';
}

/*************************************cut_row****************************************
function:cut the row's part
parameters:
	c1:column begin
	c2:column end
	row:row address
return:
	none
*******************************************************************************************/
void cut_row(int c1, int c2, ROWNODE* row)
{
	int i;
	CHARNODE* cn, * cnb;
	for (i = c1; i <=c2; i++)
		cutchar(row, c1);
}

/*************************************cutchar****************************************
function:cut a char from the row
parameters:
	rn:row address
	id:char position
return:
	none
*******************************************************************************************/
void cutchar(ROWNODE* rn, int id)
{
	int i;
	CHARNODE* cn, * cn1, * cn2;
	if (id == rn->len)
	{
		if (id == 1)
		{
			cn1 = (rn->begin)->next;
			free_char(rn->begin);
			rn->begin = cn1;
			rn->len--;
		}
		else
		{
			cn = querycharindex(rn->begin, id - 2);
			cn1 = cn->next;
			free_char(cn1);
			cn->next = NULL;
			rn->len--;
		}
	}
	else if (id == 1)
	{
		cn1 = (rn->begin)->next;
		free_char(rn->begin);
		rn->begin = cn1;
		rn->len--;
	}
	else
	{
		cn = querycharindex(rn->begin, id - 2);
		cn1 = cn->next;
		cn2 = cn1->next;
		free_char(cn1);
		cn->next = cn2;
		rn->len--;
	}
}

/*************************************cut_a_row****************************************
function:cut a row from a window
parameters:
	focuswin:
	id:row position
return:
	none
*******************************************************************************************/
void cut_a_row(WINDOW* focuswin, int id)
{
	ROWNODE* rnb, * rn, * rne;
	if (id == focuswin->rownum)
	{
		//for (rn = focuswin->begin; (rn->next)->next; rn = rn->next);
		rn = queryrowindex(focuswin, id - 2);
		rne = rn->next;
		free_row(rne);
		rn->next = NULL;
		focuswin->rownum--;
	}
	else
	{
		rnb = queryrowindex(focuswin, id - 2);
		rn = queryrowindex(focuswin, id - 1);
		rne = queryrowindex(focuswin, id);
		rnb->next = rne;
		free_row(rn);
		focuswin->rownum--;
	}
}

/*************************************find_row_id****************************************
function:append a string behind certain row
parameters:
	row:row address
return:
	row id
*******************************************************************************************/
int find_row_id(WINDOW* focuswin, ROWNODE* row)
{
	int i = 1;
	ROWNODE* rn = focuswin->begin;
	while (1)
	{
		if (rn == row)
			return i;
		rn = rn->next;
		i++;
	}
}

