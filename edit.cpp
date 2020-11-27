#define CONFIG_DEBUG
#pragma warning(disable:4996)
#include <stdio.h>
#include "lst.h"
#include "text.h"
#include "edit.h"

/***************************************drawsubwin****************************************
function:draw sub window
parameters:
	s1:text 1
	s2:text 2
return:
	none
**********************************************************************************************/
int not_edit(int x)
{
	if (x <= 0)
		return 1;
	else
		return 0;
}

/***************************************selectall****************************************
function:select all text in focuswin
parameters:
	
return:
	none
*******************************************************************************************/
void selectall(WINDOW* focuswin,int* r1, int* c1, int* r2,int * c2)
{
	ROWNODE* lastr;
	*r1 = 0;
	*c1 = 0;
	*r2 = focuswin->rownum-1;

	lastr = queryrowindex(focuswin,*r2);
	*c2 = lastr->len-1;
}

/***************************************paste****************************************
function:paste
parameters:
	(r1,c1):location for pasting
	str:text
return:
	none
****************************************************************************************/
void paste(WINDOW* focuswin, int r1, int c1,char* str)
{
	int flag = 0;
	FILE* fp;
	char buffer[100];
	ROWNODE* rn, * rnb ;

	//judge
	if (not_edit(r1))
		return;

	fp = fopen(str, "r");
	rn = rnb = queryrowindex(focuswin, r1 - 1);
	//cur the rnb
	two_row(focuswin, rnb, c1);


	while (fgets(buffer, 100, fp))
	{
		if (flag == 0)
		{
			append_string(buffer, rn);
			flag = 1;
		}
		else
		{
			insertrowafter(focuswin, rn);
			rn = rn->next;
			append_string(buffer, rn);
		}

	}
	add_two_row(focuswin, rn, rn->next);
	fclose(fp);
}

/***************************************copy****************************************
function:draw sub window
parameters:
	(r1,c1,r2,c2):text location
	str:text
return:
	none
****************************************************************************************/
void copy(WINDOW* focuswin, int r1, int c1, int r2, int c2, char* str)
{
	int i;
	char buffer[100];
	FILE* fp;
	ROWNODE* rn, * rnb, * rne;
	//judge
	if (not_edit(r1))
		return;

	fp = fopen(str, "w");
	rnb = queryrowindex(focuswin, r1 - 1);
	rne = queryrowindex(focuswin, r2 - 1);

	if (rnb == rne)
	{
		fput_row(c1, c2, rnb, buffer);
		fputs(buffer, fp);
		fclose(fp);
		return;
	}

	fput_row(c1, rnb->len, rnb, buffer);
	fputs(buffer, fp);

	for (i = r1 + 1, rn = rnb->next; i < r2; i++, rn = rn->next)
	{
		fput_row(1, rn->len, rn, buffer);
		fputs(buffer, fp);
	}

	fput_row(1, c2, rne, buffer);
	fputs(buffer, fp);

	fclose(fp);
}

/***************************************cut**********************************************
function:cut
parameters:
	(r1,c1,r2,c2):text location
	str:text
return:
	none
*******************************************************************************************/
void cut(WINDOW* focuswin, int r1, int c1, int r2, int c2, char* str)
{
	ROWNODE* rnb, * rne;
	int i;
	//judge
	if (not_edit(r1))
		return;
	copy(focuswin, r1, c1, r2, c2,str);

	rnb = queryrowindex(focuswin, r1 - 1);
	rne = queryrowindex(focuswin, r2 - 1);

	if (rnb == rne)
	{
		cut_row(c1, c2, rnb);
		return;
	}
	for (i = r1; i < r2; i++)
	{
		if (i == r1)
		{
			cut_row(c1, rnb->len, rnb);
		}
		else
		{
			cut_a_row(focuswin, r1 + 1);
		}
	}
	cut_row(1, c2, rnb->next);
	rnb = queryrowindex(focuswin, r1 - 1);
	rne = queryrowindex(focuswin, r1);
	add_two_row(focuswin, rnb, rne);
	if (focuswin->begin == 0)
	{
		focuswin->begin = rnb;
	}
}