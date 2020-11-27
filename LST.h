#ifndef  LST_H
#define LST_H

struct CHARNODE
{
	CHARNODE* next;			//next node
	char text;			//use 2 bytes to encode a character
};
struct ROWNODE
{
	ROWNODE* next;
	CHARNODE* begin;
	int len;
};

struct WINDOW
{
	int x, y;
	int w, h;
	int mode;			//1:chosen 2:hidden 3:drag 4:zoom 
	ROWNODE* begin;
	int rownum;
	int firstrow,firstcol;
	char filename[40];
	WINDOW* next;
};

CHARNODE* newchar(char c);
CHARNODE* querycharindex(CHARNODE* lst, int r);			//query node by index
void free_row(ROWNODE* r);								//free row node
void free_char(CHARNODE* r);								//free char node
void connectcharlist(ROWNODE* a,CHARNODE* b);			//connect charnode b(maybe a list) after a
void updaterowlen(ROWNODE* p);							//update row len
void insertrowafter(WINDOW* win,ROWNODE* p);			//add empty row after p
void insertrowbefore(WINDOW* win, ROWNODE* p);			//add empty row before p
void eraserow(WINDOW* win, ROWNODE* p);					//erase row node p
void erasechar_id(ROWNODE* row, int id);				//erase char node p
void erase_segment(ROWNODE* row, int l, int r);			//erase string segment
void insert_string(ROWNODE* row, int pos, char* text);	//insert string
void insertcharafter(ROWNODE* r,CHARNODE* p,char c);	//insert a char after p
CHARNODE* insertcharbefore(ROWNODE* r,CHARNODE* p, char c);	//insert a char before p
void appendchar(ROWNODE* row, char c);					//append char
ROWNODE* queryrowindex(WINDOW* win, int r);				//query row by index
ROWNODE* newrow();										//new row
char* getrowstring(ROWNODE* row,int l,int r,int casesensitve);
int getmaxcol(WINDOW* win);								//get max column of window
#endif
