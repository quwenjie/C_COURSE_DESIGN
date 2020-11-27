#include "LST.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
CHARNODE* newchar(char c)       //alloc new char
{
	CHARNODE* ne;
	ne= (CHARNODE*)malloc(sizeof(CHARNODE));    //malloc
	if (ne == NULL)
		exit(0);               //error
	ne->next = 0;               //link
	ne->text = c;         //value
	return ne;
}

ROWNODE* newrow()       //alloc new row
{
	ROWNODE* ne;
	ne = (ROWNODE*)malloc(sizeof(ROWNODE));
	if (ne == NULL)         //error
		exit(0);
	ne->len = 0;          //maintain len
	ne->next = NULL;      //link
	ne->begin = NULL;      //char linked list begin
	return ne;
}
void free_row(ROWNODE* r)      //to free row
{
	free(r);
}
void free_char(CHARNODE* r)       //to free char
{
	free(r);
}
CHARNODE* querycharindex(CHARNODE* lst, int r)   //query char by index
{
	CHARNODE* ret = lst;
	int i = 0;
	for (i = 0; i < r; i++)
		ret = ret->next;
	return ret;
}
ROWNODE* queryrowindex(WINDOW* win, int r)      //query row by index
{
	ROWNODE* ret = win->begin;
	int i = 0;
	for (i = 0; i < r; i++)
		ret = ret->next;
	return ret;
}
void appendchar(ROWNODE* row,char c)       //append char behind row
{
	CHARNODE* begin;
	begin=row->begin;
	if (begin == NULL)       //linked list empty
	{
		row->begin = newchar(c);
	}
	else                   //find the last then insert
	{
		while (begin->next)
			begin = begin->next;
		begin->next = newchar(c);
	}
	row->len++;               //maintain len
}
void insertcharafter(ROWNODE* r, CHARNODE* p, char c)    //insert directly after row
{
	CHARNODE* tmp = p->next;
	p->next = newchar(c);
	p->next->next = tmp;
	r->len++;                  //maintain len
}
void insertrowbefore(WINDOW* win,ROWNODE* p)        //insert row before row
{
	ROWNODE* ne, * tmp, * tmp2;
	win->rownum++;                   //maintain row num
	ne = newrow();
	if (p == win->begin)                //first row special case
	{
		tmp = win->begin;           
		win->begin = ne;               //change new
		ne->next = tmp;
	}
	else                    //find last row
	{
		tmp = win->begin;
		for (; tmp->next != p; tmp = tmp->next);
		tmp2 = tmp->next;
		tmp->next = ne;
		ne->next = tmp2;
	}
}
void insertrowafter(WINDOW* win, ROWNODE* p)      //insert row after
{
	ROWNODE* tmp = p->next;  //next node of current
	win->rownum++;
	p->next = newrow();   
	p->next->next = tmp;  //no need to go through list
}
void eraserow(WINDOW* win, ROWNODE* p)     //erase given row from window
{
	ROWNODE* tmp = win->begin;
	for (; tmp->next != p; tmp = tmp->next);
	tmp->next = p->next;
	free_row(p);
	win->rownum -= 1;          //maintain row count
}
CHARNODE* insertcharbefore(ROWNODE* r, CHARNODE* p, char c)   //insert char before given node
{
	CHARNODE* ne  ,*tmp,*tmp2;
	ne = newchar(c);
	r->len++;         //update len
	if (p == r->begin)          //insert before head
	{
		tmp = r->begin;
		r->begin = ne;
		ne->next = tmp;
	}
	else              //insert before others
	{
		tmp = r->begin;
		for (; tmp->next != p; tmp = tmp->next);     //find previous
		tmp2 = tmp->next;
		tmp->next = ne;
		ne->next = tmp2;
	}
	return ne;
}
void updaterowlen(ROWNODE* r)       //brute force update length
{
	CHARNODE* p = r->begin;
	r->len = 0;
	for (; p; p = p->next)
		r->len++;
}
void connectcharlist(ROWNODE* a, CHARNODE* b)  //connect two row
{
	CHARNODE* p = a->begin;
	if (p)
	{
		for (; p->next; p = p->next);
		p->next = b;
		updaterowlen(a);           //update len
	}
	else
		a->begin= b;         //combine directly
}
void erasechar_id(ROWNODE* row,int id)   //erase by id,1 started
{
	CHARNODE* tmp = row->begin,*tmp2;
	row->len -= 1;
	if (id>1)        //get the previous one
	{
		tmp2=querycharindex(row->begin, id-2);
		tmp = tmp2->next;
		tmp2->next = tmp->next;
		free_char(tmp);
	}
	else                             //erase start
	{
		row->begin = tmp->next;
		free_char(tmp);
	}
}
void erase_segment(ROWNODE* row, int l, int r)   //erase given range
{
	CHARNODE* st, * ed,*p,*q;
	if (l == 0)                      //modify head
	{
		ed = querycharindex(row->begin, r + 1);
		for (p = row->begin; p != ed; p = q)
		{
			q = p->next;
			free_char(p);
		}
		row->begin = ed;
	}
	else                              //obtain [start,end)
	{
		st= querycharindex(row->begin,l-1);
		ed = querycharindex(row->begin, r+1);
		for (p = st->next; p != ed; p = q)
		{
			q = p->next;
			free_char(p);
		}
		st->next = ed;
	}
	row->len -= (r-l+1);
}
void insert_string(ROWNODE* row, int pos,char* text)   //insert a whole string at given pose
{
	CHARNODE* st,*p;
	int i = 1;
	//first insert the first
	if (row->len == 0)               //alloc new link
	{
		st = row->begin = newchar(text[0]);
		row->len++;
	}
	else              
	{
		p = querycharindex(row->begin, pos);         
		st=insertcharbefore(row, p, text[0]);    
	}
	//then insert others after the first inserted
	while (text[i])
	{
		insertcharafter(row, st, text[i]);	
		st = st->next;
		i++;
	}
}

char* getrowstring(ROWNODE* row, int l, int r, int casesensitve)   //cut the row string,convert case
{
	CHARNODE* st,*ed;
	char* ret;
	int i = 0;
	st = querycharindex(row->begin, l);   //query start
	ed= querycharindex(row->begin, r+1);   //query next of end 
	ret = (char*)malloc(r-l+2);
	do
	{
		ret[i++] = st->text;
		st = st->next;
	} while (st != ed);  // obtaining [start,end)
	ret[i++] = '\0';         //append end
	if (!casesensitve)         //convert case
	{
		strlwr(ret);		//lower case
	}
	return ret;
}

int getmaxcol(WINDOW* win)               //count max col for visualize
{
	ROWNODE* rowp;       
	int mxc = 0;
	rowp = win->begin;
	for (; rowp != NULL; rowp = rowp->next)   //find every row's len
	{
		if (mxc < rowp->len)
			mxc = rowp->len;
	}
	return mxc;
}