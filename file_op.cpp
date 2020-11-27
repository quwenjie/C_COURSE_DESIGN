#include "docwin.h"
#include "lst.h"
#include "mouse.h"
#include "hz.h"
#include "keyboard.h"
#include "usr.h"
#include "draw.h"
#include "text.h"
#include "findwin.h"
#include "file_op.h"

#ifdef _MSC_VER
#pragma warning(disable:4996)
#else
#include <dos.h>
#endif
#include <graphics.h>
#include <stdio.h>
#include <string.h>

extern usr u;
extern int mousex, mousey, mousepress;
char str[2] = "2";

/***************************************file_num****************************************
function:calculate the number of opened files
parameters:
	s:menu contents
return:
	number
*******************************************************************************************/
int file_num(char s[12][21])
{
	int i=0;
	while (strcmp(s[i], "\0") != 0)
	{
		i++;
	}
	return i;
}

/***************************************open_click_file****************************************
function:open related file when clicking file in the file submenu
parameters:
	(x,y):position of mouse
	s:contents
	wp:winhead
	focuswin:top win
return:
	none
*******************************************************************************************/
void open_click_file(int x, int y, char s[12][21], WINDOW** wp, WINDOW** focuswin)
{
	WINDOW* ww;
	int num, i = 5;
	num = file_num(s);
	for (i = 5; i < num; i++)
	{
		if (buttonpress(80, 155 + 20 * (i - 5), 130, 175 + 20 * (i - 5), x, y))
		{
			if (open_file_name(*wp, s[i], focuswin))
			{
				ww = new_file(wp);
				check_file(s[i], ww);
				*focuswin = ww;
				return;
			}
		}
	}
}

/***************************************open_file_name****************************************
function:open the file by its name
parameters:
	s:file name
	wp:winhead
	focuswin:top win
return:
	state:1 for open,0 for file being existed
*******************************************************************************************/
int  open_file_name(WINDOW* win, char* s,WINDOW** focuswin)
{
	WINDOW* wp;
	for (wp = win; wp; wp = wp->next)
	{
		if (wp->filename && (strcmp(s, wp->filename) == 0))
		{
			*focuswin = wp;
			return 0;
		}
	}
	return 1;
}

/***************************************del_same_files**************************************
function:
	when new opened files were written into the user file,file names in user file may be 
repeated.this function is used to delete the same file names.	
parameters:
	fn:file name of user file
return:
	none
*******************************************************************************************/
void del_same_files(char* fn)
{
	FILE* fp;
	int i = 0, n, j;
	char buffer[20][100];
	fp=fopen(fn,"r");
	//calculate row number & content in buffer
	while (fgets(buffer[i], 100, fp))
	{
		i++;
	}
	fclose(fp);
	n = i;
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (strcmp(buffer[i], buffer[j]) == 0&&strcmp(buffer[i],"\n")!=0)
			{
				strcpy(buffer[i], "\n");//empty
			}
		}
	}
	fp = fopen(fn, "w");//renew the user file
	for (i = 0; i < n; i++)
	{
		if (strcmp(buffer[i], "\n") != 0)
		{
			fputs(buffer[i], fp);
			fputc('\n', fp);
		}
	}
	fclose(fp);
}

/***************************************update_files**************************************
function:
	update showing files opening files is triggered	
parameters:
	fn:file name of user file
return:
	none
*******************************************************************************************/
void update_files(char* fn)
{
	FILE* fp;
	fp = fopen(u.username, "a+");
	fputs(fn, fp);
	fputc('\n', fp);
	fclose(fp);
	del_same_files(u.username);

}

/***************************************open_files**************************************
function:
	click the "open file" button then trigger this function
parameters:
	wp:pointer to winhead pointer,make changing winhead globally possible
procedure:
	iput box event --> check --> new window
return:
	renewed winhead pointer
*****************************************************************************************/
WINDOW* open_file(WINDOW** wp)
{
	int  i = 0, x = 250, flag = 0, m;
	char ch, filename[20];
	WINDOW* ww;
	WINDOW* www;
	FILE* fp;
	memset(filename, '\0', 20);
	drawsubwin("请输入您想要打开的文件名称", "比如:yzs.cpp");

	while (1)
	{
		mousemove();
		if (buttonpress(450, 150, 470, 170, mousex, mousey) && mousepress)
			return NULL;
		if (buttonpress(400, 270, 440, 300, mousex, mousey) && mousepress)
		{
			//check
			if (filename[0] == '\0')
			{
				drawbar(240, 300, 470, 330, LIGHTGRAY);
				drawtext(250, 302, "文件名不能为空!", 16, 20, 10, RED);
			}
			else if (!strchr(filename, '.'))
			{
				drawbar(240, 300, 470, 330, LIGHTGRAY);
				drawtext(250, 302, "文件名要带后缀!", 16, 20, 10, RED);
			}
			else if (open_file_name(*wp, filename,&www) == 0)
			{
				drawbar(240, 300, 470, 330, LIGHTGRAY);
				drawtext(250, 302, "该文件已打开!", 16, 20, 10, RED);
			}
			else if ((fp = fopen(filename, "r+"))&& open_file_name(*wp, filename,&www))
			{
				//new window
				fclose(fp);
				if (wp)
					ww = new_file(wp);
				else
				{
					*wp = newwindow(20, 150, 600, 300, 1);
					ww = new_file(wp);
				}
				check_file(filename, ww);
				update_files(filename);
				return ww;
			}
			else if (!(fp = fopen(filename, "r+")))
			{
				drawbar(240, 300, 470, 330, LIGHTGRAY);
				drawtext(250, 302, "该文件不存在!", 16, 20, 10, RED);
			}
		}
		if (mousepress)
		{
			if (buttonpress(240, 270, 380, 300, mousex, mousey) && mousepress)
			{
				flag = 1;
				drawbar(225, 270, 230, 300, RED);
			}
			else
			{
				flag = 0;
				drawbar(225, 270, 230, 300, LIGHTGRAY);
			}
		}
		ch = get_c();
		if (ch && flag)
		{
			if (ch == 8 || ch == 127)
			{
				x -= 10;
				i--;
				if (i >= 0) {
					filename[i] = '\0';
					drawbar(x, 270, x + 10, 300, WHITE);
				}
				else {
					x = 250;
					i = 0;
				}
			}
			if (ch != 8 && ch != 127)
			{
				filename[i] = ch;
				drawletter(x, 270, ch);
				i++;
				x += 10;
			}
		}
		drawmouse();

#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
}

/***************************************check_file**************************************
function:
	add the file content to the new window	
parameters:
	fn:file name
	wp:new window
return:
	state:0 for file missing,12 for adding sucessfully
*******************************************************************************************/
int check_file(char* fn, WINDOW* wp)
{
	FILE* fp;
	char ch;
	ROWNODE* rn1, * rn2;
	CHARNODE* cn1, * cn2;
	cn1 = newchar('\0');
	if (!(fp = fopen(fn, "r+")))
		return 0;
	else
	{
		strcpy(wp->filename, fn);
		rn1 = wp->begin = newrow();
		do
		{
			ch = fgetc(fp);
			if (ch == EOF)
			{
				return 12;
			}
			else if (ch == '\n')
			{
				rn2 = newrow();
				rn1->next = rn2;
				rn1 = rn2;
				wp->rownum++;
			}
			else
			{
				if (rn1->begin == NULL)
				{
					cn1 = rn1->begin = newchar(ch);
					rn1->len = 1;
				}
				else
				{
					cn2 = newchar(ch);
					cn1->next = cn2;
					cn1 = cn2;
					rn1->len++;
				}
			}
		} while (1);
	}
}

/***************************************new_file**************************************
function:
	new window to the window chain	
parameters:
	wp:pointer to pointer
procedure:
	name new file without saving for 'nonamex' to classify them easier
return:
	new window
****************************************************************************************/
WINDOW* new_file(WINDOW** wp)
{
	static int y;
	
	WINDOW* p;
	//none
	if (!(*wp))
	{
		*wp = newwindow(20, 150, 600, 300, 1);
		strcat((*wp)->filename, "noname");
		strcat((*wp)->filename, str);
		str[0] += 1;
		return *wp;
	}

	//modify windows' state
	for (p = *wp; p->next; p = p->next)
	{
		p->mode = 2;
	}
	p->next = newwindow(20, 230 + y, 600, 100 + y, 1);
	strcat((p->next)->filename, "noname");
	strcat((p->next)->filename, str);
	str[0] += 1;
	y = y + 10;
	return p->next;
}

/***************************************save_file**************************************
function:
	save the certain window's content	
parameters:
	wp:window to save
procedure:
	#nonamex --> save as
	#name existed -->modify contents
return:
	none
****************************************************************************************/
void save_file(WINDOW* wp)
{
	FILE* fp;
	char s[1][50] = { "该文件已经损坏,请到根目录下检查!" };
	if (!strstr(wp->filename,"noname"))
	{
		if (fp = fopen(wp->filename, "w"))
		{
			fclose(fp);
			write_text(wp, wp->filename);
			update_files(wp->filename);
		}
		else
			drawattention("注意",s,1);
	}
	else
	{
		save_as(wp);
	}
}

/***************************************save_as****************************************
function:
	save the window as a file with input file name	
parameters:
	wp:window to be saved as xxx
procedure:
	#file name already existed --> replace it or not
	#new name --> new file but won't open it in this function/put it in the window chain
return:
	none
****************************************************************************************/
void save_as(WINDOW* wp)
{
	int  i = 0, x = 250, flag = 0;
	int flag2;
	char str[20] = { '\0' };
	strcpy(str, wp->filename);
	strcat(str, "另存为文件名");
	char filename[100], ch;
	memset(filename, '\0', 100);
	drawsubwin(str, "比如:yzs.cpp");
	while (1)
	{
		mousemove();
		if (buttonpress(450, 150, 470, 170, mousex, mousey) && mousepress)
			return;
		if (buttonpress(400, 270, 440, 300, mousex, mousey) && mousepress)
		{
			if (filename[0] == '\0')
			{
				drawbar(240, 300, 470, 330, LIGHTGRAY);
				drawtext(250, 302, "文件名不能为空!", 16, 20, 10, RED);
			}
			else if (!strchr(filename, '.'))
			{
				drawbar(240, 300, 470, 330, LIGHTGRAY);
				drawtext(250, 302, "文件名要带后缀!", 16, 20, 10, RED);
			}
			else
			{
				if (fopen(filename, "r"))
				{
					if (drawyesorno("该文件已经存在", "确定要替换它吗?"))
					{
						strcpy(wp->filename, filename);
						save_file(wp);
						while (1)
						{
							mousemove();
							if(!mousepress)
								break;
							drawmouse();
#ifdef _MSC_VER
							delay_ms(50);
#else
							delay(50);
#endif
						}
						return;
					}
					else
					{
						while (1)
						{
							mousemove();
							if(!mousepress)
								break;
							drawmouse();
#ifdef _MSC_VER
							delay_ms(50);
#else
							delay(50);
#endif
						}
						return;
					}	
				}
				else
				{
					strcpy(wp->filename, filename);
					save_file(wp);
					while (1)
					{
						mousemove();
						if(!mousepress)
							break;
						drawmouse();
#ifdef _MSC_VER
						delay_ms(50);
#else
						delay(50);
#endif
					}
					return;
				}
			}
		}
		if (mousepress)
		{
			if (buttonpress(240, 270, 380, 300, mousex, mousey) && mousepress)
			{
				flag = 1;
				drawbar(225, 270, 230, 300, RED);
			}
			else
			{
				flag = 0;
				drawbar(225, 270, 230, 300, LIGHTGRAY);
			}
		}
		ch = get_c();
		if (ch && flag)
		{
			if (ch == 8 || ch == 127)
			{
				x -= 10;
				i--;
				if (i >= 0) {
					filename[i] = '\0';
					drawbar(x, 270, x + 10, 300, WHITE);
				}
				else {
					x = 250;
					i = 0;
				}
			}
			if (ch != 8 && ch != 127)
			{
				filename[i] = ch;
				drawletter(x, 270, ch);
				i++;
				x += 10;
			}
		}
		drawmouse();

#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif
	}
	mydelay(1000);
}

/***************************************save_all****************************************
function:
	save all showing windows/window chain
parameters:
	wp:winhead
return:
	none
****************************************************************************************/
void save_all(WINDOW* wp)
{
	WINDOW* ww;
	for (ww = wp; ww; ww = ww->next)
	{
		if (!strstr(ww->filename, "noname"))
			save_file(ww);

		else
			save_as(ww);
	}
}