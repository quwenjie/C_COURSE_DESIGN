#include "search.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "refresh.h"
#ifdef __BORLANDC__
#include <dos.h>
#endif
extern int mousex, mousey, mousepress;
void drawfindwin()            //draw find ui
{
	drawbar(170, 150, 470, 330, LIGHTGRAY);
	drawbar(240, 180, 380, 210, WHITE);
	drawbar(450, 150, 470, 170, RED);
	drawtext(453, 153, "x", 16, 5, 5, BLACK);
	drawbar(400, 180, 440, 210, YELLOW);
	drawtext(175, 183, "内容:", 16, 20, 10, BLACK);
	drawtext(403, 183, "查找", 16, 20, 10, BLACK);

	drawrect(172,220,370,245,SOLID_LINE,1,BLACK);
	circle(235,235,8);                                      // the circle check box
	fillcircle(235, 235, 4, BLACK);                        //default state
	drawtext(175, 225, "方向", 16, 20, 10, BLACK);
	drawtext(255, 225, "向下", 16, 20, 10, BLACK);
	circle(305, 235, 8);                            //the circle checkbox
	drawtext(325, 225, "向上", 16, 20, 10, BLACK);

	circle(185, 265, 8);
	fillcircle(185, 265, 4, BLACK);                      //default state
	drawtext(205, 255, "区分大小写", 16, 20, 10, BLACK);
}

void getfindtext(char* text, int* casesense, int* dir)           //get user input
{
	int i=0,*p,x = 250, flag = 0;
	char ch;
	drawfindwin();
	while (1)
	{
		mousemove();
		
		if (mousepress)
		{
			if (buttonpress(450, 150, 470, 170, mousex, mousey) )
				return ;
			if (buttonpress(400, 180, 440, 210, mousex, mousey) )
			{
				if (text[0] == '\0')
				{
					drawbar(240, 300, 470, 330, LIGHTGRAY);
					drawtext(250, 302, "内容不能为空!", 16, 20, 10, RED);    //handle null text
				}
				else
					return;                            //we got what we need
			}
			if (buttonpress(240, 180, 380, 210, mousex, mousey))         //input box focus 
			{
				flag = 1;
				drawbar(225, 180, 230, 210, RED);
			}
			else
			{
				flag = 0;
				drawbar(225, 180, 230, 210, LIGHTGRAY);
			}
			if (buttonpress(226, 226, 244, 244, mousex, mousey))       //choose direction
			{ 
				if (*dir == 1)
				{
					*dir = 0;
					fillcircle(235, 235, 4, BLACK);                 //refresh the checkbox
					
					fillcircle(305, 235, 4, LIGHTGRAY);
				}
			}
			else if(buttonpress(296, 226, 314, 244, mousex, mousey))
			{ 
				if (*dir == 0)
				{
					*dir = 1;

					fillcircle(235, 235, 4, LIGHTGRAY);      //refresh the checkbox
					
					fillcircle(305, 235, 4, BLACK);
				}
			}
			else if (buttonpress(176, 256, 194,274, mousex, mousey))   //case checkbox
			{
				if (*casesense == 1)
				{
					*casesense = 0;
					fillcircle(185, 265, 4, LIGHTGRAY);         //refresh the checkbox
				}
				else
				{
					*casesense = 1;
					fillcircle(185, 265, 4, BLACK);
				}
			}
		}

		ch = get_c();             //get user input
		if (ch && flag)
		{
			if (ch == 8 || ch == 127)         //backspace or delete
			{
				x -= 10;
				i--;
				if (i >= 0)
				{
					text[i] = '\0';
					drawbar(x, 180, x + 10, 210, WHITE);     //clear the pos
				}
				else              //do not exceed
				{
					x = 250;
					i = 0;
				}
			}
			if (ch != 8 && ch != 127)         //normal input
			{
				text[i++] = ch;     //add new char
				text[i] = '\0';
				drawletter(x, 180, ch);     //draw
				x += 10;
			}
	}
		drawmouse();

#ifdef _MSC_VER
		delay_ms(100);
#else
		delay(100);
#endif
	}
}
void find_opt(WINDOW* focuswin,int* focusrow, int* focuscol,int* crow1,int* ccol1,int*crow2,int*ccol2)  //find option
{
	char text[50],attention[100],*temp;
	ROWNODE* row;
	int fd,offset,len,i,case_sensitive=1,dir=0;		//1,0
	getfindtext(text, &case_sensitive, &dir);          //get find text
	if (!case_sensitive)
	{
		strlwr(text);            //exchange case
	}
	len = strlen(text);
	if (dir == 0)
	{
		for (i = *focusrow; i < focuswin->rownum; i++)
		{
			row = queryrowindex(focuswin, i);         //get row
			if (row->len == 0)
				continue;
			if (i == *focusrow)                      //handle special case
			{
				offset = *focuscol;
				if (offset == row->len)
					continue;
			}
			else
				offset = 0;                    //otherwise start from left
			temp = getrowstring(row, offset, row->len - 1, case_sensitive);         //cut 
			fd = kmp_match(temp, text);       //kmp match
			if (fd != -1)              //found
			{
				*crow1 = i;                 
				*crow2 = i;
				*ccol1 = offset+fd;
				*ccol2 = offset+fd + len - 1;      //set select range
				*focusrow = i;
				*focuscol = offset+fd+len;            
				//printf("JUMP TO %d %d %d %d\n", *crow1, *crow2, *ccol1, *ccol2);
				update_first_rowcol(focuswin, *focusrow, *focuscol);      //update your perspective
				//printf("FOCUS WIN %d %d %d %d\n",focuswin->firstcol,focuswin->firstrow, *focusrow, *focuscol);
				return;
			}
			free(temp);
		}
	}
	else
	{
		for (i = *focusrow; i >=0; i--)       //backwards
		{
			row = queryrowindex(focuswin, i);  //get row
			if (row->len == 0)
				continue;
			if (i == *focusrow)                     //special case
				temp = getrowstring(row, 0, *focuscol-1, case_sensitive);
			else
				temp = getrowstring(row, 0, row->len - 1, case_sensitive);
			fd = kmp_match(temp, text);             //kmp match
			if (fd != -1)        //matched
			{
				*crow1 = i;
				*crow2 = i;
				*ccol1 = fd;
				*ccol2 = fd + len - 1;
				*focusrow = i;
				*focuscol = fd+len;
				//printf("JUMP TO %d %d %d %d\n", *crow1, *crow2, *ccol1, *ccol2);
				update_first_rowcol(focuswin, *focusrow, *focuscol);       //update your perspective
				//printf("FOCUS WIN %d %d %d %d\n",focuswin->firstcol,focuswin->firstrow, *focusrow, *focuscol);
				return;
			} 
			free(temp);     //clean memory
		}
	}
	//sprintf(attention,"找不到\"%s\"!", text);
	//drawattention(attention);
	*crow1 = *crow2= *ccol1= *ccol2=-1;
	*focuscol=*focusrow = 0;
}
void drawreplacewin()      //draw replace window
{
	drawbar(160, 150, 500, 350, LIGHTGRAY);
	drawbar(240, 180, 380, 210, WHITE);         //input box
	drawbar(240, 240, 380, 270, WHITE);
	drawbar(480, 150, 500, 170, RED);
	drawtext(483, 153, "x", 16, 5, 5, BLACK);
	drawbar(400, 180, 440, 210, YELLOW);
	drawbar(400, 240, 480, 270, YELLOW);
	drawtext(165, 183, "内容:", 16, 20, 10, BLACK);
	drawtext(165, 243, "替换为:", 16, 20, 10, BLACK);
	drawtext(403, 183, "替换", 16, 20, 10, BLACK);
	drawtext(403, 243, "全部替换", 16, 20, 10, BLACK);

	drawrect(172, 280, 370, 305, SOLID_LINE, 1, BLACK);
	circle(235, 295, 8);
	fillcircle(235, 295, 4, BLACK);           //checkbox
	drawtext(175, 285, "方向", 16, 20, 10, BLACK);
	drawtext(255, 285, "向下", 16, 20, 10, BLACK);
	circle(305, 295, 8);                         //checkbox
	drawtext(325, 285, "向上", 16, 20, 10, BLACK);

	circle(185, 325, 8);
	fillcircle(185, 325, 4, BLACK);
	drawtext(205, 315, "区分大小写", 16, 20, 10, BLACK);
}
void getreplacetext(char* text,char* text2, int* casesense, int* dir,int* all)     //get replace text
{
	int len1=0,len2=0, *p,*x,x1 = 250,x2=250, flag = 0;   //pointer x point to now input x
	char ch,*ps;            //point to now string
	drawreplacewin();            //draw replace window
	while (1)
	{
		mousemove();

		if (mousepress)
		{
			if (buttonpress(480, 150, 500, 170, mousex, mousey))    //close
				return;
			if (buttonpress(400, 180, 440, 210, mousex, mousey))   //handle ok
			{
				if (text[0] == '\0' )
				{
					drawbar(240, 300, 470, 330, LIGHTGRAY);
					drawtext(250, 302, "内容不能为空!", 16, 20, 10, RED);
				}
				else
				{
					*all = 0;         //single
					return;
				}
			}
			if (buttonpress(400, 240, 440, 270, mousex, mousey))    //handle all ok
			{
				if (text[0] == '\0')
				{
					drawbar(240, 300, 470, 330, LIGHTGRAY);
					drawtext(250, 302, "内容不能为空!", 16, 20, 10, RED);
				}
				else
				{
					*all = 1;       //all
					return;
				}
			}
			if (buttonpress(240, 180, 380, 210, mousex, mousey))
			{
				flag = 1;
				drawbar(225, 180, 230, 210, RED);
				drawbar(225, 240, 230, 270, LIGHTGRAY);
			}
			else if (buttonpress(240, 240, 380, 270, mousex, mousey))
			{
				flag = 2;
				drawbar(225, 180, 230, 210, LIGHTGRAY);
				drawbar(225, 240, 230, 270, RED);
			}
			else
			{
				flag = 0;
				drawbar(225, 180, 230, 210, LIGHTGRAY);
				drawbar(225, 240, 230, 270, LIGHTGRAY);
			}
			if (buttonpress(226, 286, 244, 304, mousex, mousey))
			{
				if (*dir == 1)
				{
					*dir = 0;
					fillcircle(235, 295, 4, BLACK);

					fillcircle(305, 295, 4, LIGHTGRAY);
				}
			}
			else if (buttonpress(296, 286, 314, 304, mousex, mousey))
			{
				if (*dir == 0)
				{
					*dir = 1;

					fillcircle(235, 295, 4, LIGHTGRAY);

					fillcircle(305, 295, 4, BLACK);
				}
			}
			else if (buttonpress(176, 316, 194,334, mousex, mousey))
			{
				if (*casesense == 1)
				{
					*casesense = 0;
					fillcircle(185, 325, 4, LIGHTGRAY);
				}
				else
				{
					*casesense = 1;
					fillcircle(185, 325, 4, BLACK);
				}
			}
		}
		ch = get_c();           //get input char
		if (ch && flag)
		{
			if (flag == 1)        //handle input 1
			{
				ps = text;
				p = &len1;
				x = &x1;
			}
			else         //handle input 2
			{
				ps = text2;
				p = &len2;
				x = &x2;
			}
			if (ch == 8 || ch == 127)     //backspace / delete
			{
				(*x) -= 10;
				(*p)--;
				if (*p >= 0)
				{
					ps[*p] = '\0';
					drawbar(*x, 120+flag*60, (*x) + 10, 150+flag*60, WHITE);
				}
				else 
				{
					*x = 250;
					*p = 0;
				}
			}
			if (ch != 8 && ch != 127)      //otherwise
			{
				ps[(*p)++] = ch;
				ps[*p] = '\0';
				drawletter(*x, 120+flag*60, ch);
				(*x) += 10;
			}
		}
		drawmouse();

#ifdef _MSC_VER
		delay_ms(100);
#else
		delay(100);
#endif
	}
}
void replace(WINDOW* focuswin, int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2, char text1[],char text2[], int case_sensitive, int dir)
{
	int fd=-1,len,len2,i,offset=0;
	ROWNODE* row;
	char* temp = NULL;
	len = strlen(text1);
	len2 = strlen(text2);
	if (dir == 0)
	{
		for (i = *focusrow; i < focuswin->rownum; i++)
		{
			row = queryrowindex(focuswin, i);
			if (row->len == 0)      //corner case
				continue;
			if (i == *focusrow)                //handle now row
			{
				offset = *focuscol;
				if (offset == row->len)
					continue;
			}
			else
				offset = 0;
			temp = getrowstring(row, offset, row->len - 1, case_sensitive);  //get row string
			fd = kmp_match(temp, text1);        //match
			if (fd != -1)
			{
				erase_segment(row, offset + fd, offset + fd + len - 1); //erase segment
				insert_string(row,offset+fd,text2);                    //insert string
				*crow1 = -1;                                         //set col,row
				*crow2 = -1;
				*ccol1 = -1;
				*ccol2 = -1;
				*focusrow = i;                                     //set focus after
				*focuscol = offset + fd + len2;
				return;            //only once
			}
			free(temp);
		}
	}
	else
	{
		for (i = *focusrow; i >= 0; i--)
		{
			row = queryrowindex(focuswin, i);
			if (row->len == 0)    //corner case
				continue;
			if (i == *focusrow)     //handle now row
				temp = getrowstring(row, 0, *focuscol - 1, case_sensitive);
			else
				temp = getrowstring(row, 0, row->len - 1, case_sensitive);
			fd = kmp_match(temp, text1); //match
			if (fd != -1) //matched
			{
				erase_segment(row, offset + fd, offset + fd + len - 1); //erase segment
				insert_string(row, offset + fd, text2);              //insert string
				*crow1 = -1;                                          //set col,row
				*crow2 = -1;
				*ccol1 = -1;
				*ccol2 = -1;
				*focusrow = i;
				*focuscol = offset + fd + len2;        //set focus after
				return;       //only once
			}
			free(temp);
		}
	}
}
void replace_all(WINDOW* focuswin, int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2, char text1[], char text2[], int case_sensitive, int dir)
{
	int fd = -1, len, len2, i, offset = 0;
	ROWNODE* row;
	char* temp = NULL;
	len = strlen(text1);       //get length
	len2 = strlen(text2);     //get length
	if (dir == 0)
	{
		for (i = *focusrow; i < focuswin->rownum; i++)
		{
			row = queryrowindex(focuswin, i);   //get row
			if (row->len == 0)         //corner case
				continue;
			if (i == *focusrow)     //handle now row
			{
				offset = *focuscol;
				if (offset == row->len)
					continue;
			}
			else                   //normal case
				offset = 0;
			while (1)
			{
				temp = getrowstring(row, offset, row->len - 1, case_sensitive);   //get string
				fd = kmp_match(temp, text1);           //match first
				free(temp);                       //free every time
				if (fd != -1)
				{
					erase_segment(row, offset + fd, offset + fd + len - 1);  //erase first
					insert_string(row, offset + fd, text2);          //insert 
					*crow1 = -1;                                       //set row,col
					*crow2 = -1;
					*ccol1 = -1;
					*ccol2 = -1;
					*focusrow = i;
					*focuscol = offset + fd + len2;               //set focus after
					offset += fd + len2;                  //recalc now offset
					if (offset == row->len)                  //at the end
						break;
				}
				else
					break;
			}
			
		}
	}
	else
	{
		for (i = *focusrow; i >= 0; i--)
		{
			row = queryrowindex(focuswin, i);   //get row
			if (row->len == 0)         //corner case
				continue;
			if (i == *focusrow)       //handle now row
				temp = getrowstring(row, 0, *focuscol - 1, case_sensitive);  //get string
			else      //normal case
				temp = getrowstring(row, 0, row->len - 1, case_sensitive); 
			fd = kmp_match(temp, text1);         //match first
			if (fd != -1)
			{
				erase_segment(row, offset + fd, offset + fd + len - 1);   //erase segment
				insert_string(row, offset + fd, text2);                    //insert string
				*crow1 = -1;        //set row col
				*crow2 = -1;
				*ccol1 = -1;
				*ccol2 = -1;
				*focusrow = i;
				*focuscol = offset + fd + len2;               //set cursor
				
			}
			free(temp);
		}
	}
}
void replace_opt(WINDOW* focuswin, int* focusrow, int* focuscol, int* crow1, int* ccol1, int* crow2, int* ccol2)  //replace option
{
	char text1[50],text2[50], * temp;
	int all=0,case_sensitive = 1, dir = 0;		//1,0
	getreplacetext(text1,text2, &case_sensitive, &dir,&all); //get user input
	if (!case_sensitive)       //convert text
	{
		strlwr(text1);
	}
	if (all)
		replace_all(focuswin, focusrow, focuscol, crow1, ccol1, crow2, ccol2, text1,text2,case_sensitive,dir ); //replace all
	else
		replace(focuswin, focusrow, focuscol, crow1, ccol1, crow2, ccol2, text1,text2, case_sensitive, dir);  //replace single
	*crow1 = *crow2 = *ccol1 = *ccol2 = -1;
}
void drawjumpwin()   //draw jump win
{
	drawbar(170, 150, 470, 330, LIGHTGRAY);
	drawbar(240, 180, 380, 210, WHITE);
	drawbar(450, 150, 470, 170, RED);
	drawtext(453, 153, "x", 16, 5, 5, BLACK);
	drawbar(400, 180, 440, 210, YELLOW);
	drawtext(175, 183, "行号:", 16, 20, 10, BLACK);
	drawtext(403, 183, "跳转", 16, 20, 10, BLACK);
}

int getjumprow()         //get input jump row
{
	int i = 0, * p, x = 250, flag = 0,row;
	char ch,text[30]="";
	drawjumpwin();        //draw
	while (1)
	{
		mousemove();
		if (mousepress)
		{
			if (buttonpress(450, 150, 470, 170, mousex, mousey))      //close
				return -1;
			if (buttonpress(400, 180, 440, 210, mousex, mousey))
			{
				if (text[0] == '\0')
				{
					drawbar(240, 300, 470, 330, LIGHTGRAY);
					drawtext(250, 302, "行号不能为空!", 16, 20, 10, RED);
				}
				else        //return 
				{
					row=atoi(text)-1;			//in data structures ,the line number begins with zero
					return row;
				}
			}
			if (buttonpress(240, 180, 380, 210, mousex, mousey))   //set focus
			{
				flag = 1;
				drawbar(225, 180, 230, 210, RED);
			}
			else
			{
				flag = 0;
				drawbar(225, 180, 230, 210, LIGHTGRAY);
			}
		}

		ch = get_c();
		if (ch && flag)
		{
			if (ch == 8 || ch == 127)      //backspace/ delete
			{
				x -= 10;
				i--;
				if (i >= 0)
				{
					text[i] = '\0';
					drawbar(x, 180, x + 10, 210, WHITE);
				}
				else 
				{ 
					x = 250;
					i = 0;
				}
			}
			else if (ch>='0' && ch<='9')     //only handle number inputs
			{
				text[i++] = ch;
				text[i] = '\0';
				drawletter(x, 180, ch);
				x += 10;
			}
		}
		drawmouse();

#ifdef _MSC_VER
		delay_ms(100);
#else
		delay(100);
#endif
	}
}

void jump_opt(WINDOW* focuswin, int* focusrow, int* focuscol)    //run jump option
{
	int r;
	r=getjumprow();             //get jump row
	*focuscol = 0;                 
	*focusrow = r;              //set focus
	focuswin->firstcol = 0;    //locate first col,row
	focuswin->firstrow = 0;
	update_first_rowcol(focuswin, *focusrow, *focuscol);  //update it
}

