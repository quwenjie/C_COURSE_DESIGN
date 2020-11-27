#include "hz.h"
#include "mouse.h"
#include "keyboard.h"
#include "lst.h"
#include "text.h"
#include "usr.h"
#include "draw.h"
#include "login.h"

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#ifdef __BORLANDC__
#include <conio.h>
#include <dos.h>
#include <bios.h>
#endif
#include <graphics.h>
#include <string.h>
#include <stdio.h>

usr u;
extern int mousex, mousey, mousepress;
/***************************************login****************************************
function:
	login & signin
parameters:
	none
return:
	none
****************************************************************************************/
void login()
{
	int i = 0, j = 0, x = 220, y = 220, f1 = 0, f2 = 0;
	char ch;

	initusr(u);
	drawlogin();

	while (1)
	{
		mousemove();
		if (buttonpress(200, 380, 300, 420, mousex, mousey)&&mousepress)
		{
			if(u.username[0]!='\0'&&u.code[0]!='\0')
			{
				if (!inusrs(u)) {
					drawhint("密码不正确或者用户名不存在", "2s后返回主界面");
					login();
					break;
				}
				else
				{
					drawhint("登陆成功!", "2s后进入BC界面");
					break;
				}
			}
			else
			{
				drawhint("用户名和密码均不能为空", "2s后返回主界面");
				login();
				break;
			}
		}

		if (buttonpress(400, 380, 500, 420, mousex, mousey)&&mousepress)
		{
			if(u.username[0]!='\0'&&u.code[0]!='\0')
			{
				writeusr(u);
				drawhint("注册成功!", "2s后返回主界面");
				login();
				break;
			}
			else
			{
				drawhint("用户名和密码均不能为空", "2s后返回主界面");
				login();
				break;
			}
		}
		if (mousepress)
		{
			if (buttonpress(200, 240, 500, 280, mousex, mousey))
			{
				f1 = 1;
				f2 = 0;
				drawbar(185, 240, 190, 280, RED);
				drawbar(185, 300, 190, 340, BLACK);
			}
			else if (buttonpress(200, 300, 500, 340, mousex, mousey))
			{
				f2 = 1;
				f1 = 0;
				drawbar(185, 300, 190, 340, RED);
				drawbar(185, 240, 190, 280, BLACK);
			}
			else
			{
				f1 = f2 = 0;
				//setcolor(0);
				drawbar(185, 240, 190, 280, BLACK);
				drawbar(185, 300, 190, 340, BLACK);
			}
		}

		if (ch = get_c())
		{
			if (f1&&i<=15)
			{
				drawbar(90, 205, 400, 238, BLACK);
				if (ch == 8 || ch == 46)
				{
					x -= 10;
					i--;
					if (i >= 0) {
						u.username[i] = '\0';
						drawbar(x, 240, x + 10, 280, WHITE);
					}
					else {
						x = 220;
						i = 0;
					}
				}
				if (ch != 8 && ch != 46)
				{
					u.username[i] = ch;
					drawletter(x, 250, ch);				
					i++;
					x += 10;
				}
			}
			if (i > 15)
			{
				if (ch != 8 && ch != 46)
				{
					drawtext(150, 210, "用户名和密码均不能超过15位", 16, 15, 10, RED);
				}
				else
				{
					x -= 10;
					i--;
					if (i >= 0) {
						u.username[i] = '\0';
						drawbar(x, 240, x + 10, 280, WHITE);
					}
					else {
						x = 220;
						i = 0;
					}
				}
			}
			if (f2 && j<=15)
			{
				drawbar(90, 205, 400, 238, BLACK);
				if (ch == 8 || ch == 127)
				{
					y -= 10;
					j--;
					if (j >= 0) {
						u.code[j] = '\0';
						drawbar(y, 300, y + 10, 340, WHITE);
					}
					else {
						y = 220;
						j = 0;
					}
				}
				if (ch != 8 && ch != 127)
				{
					u.code[j] = ch;
					drawletter(y, 310, '*');				
					j++;
					y += 10;
				}
			}
			else if (j > 15)
			{
				if (ch != 8 && ch != 127)
				{
					drawtext(150, 210, "用户名和密码均不能超过15位", 16, 15, 10, RED);
				}
				else
				{
					y -= 10;
					j--;
					if (j >= 0) {
						u.code[j] = '\0';
						drawbar(y, 300, y + 10, 340, WHITE);
					}
					else {
						y = 220;
						j = 0;
					}
				}
			}
		}
		drawmouse();
#ifdef _MSC_VER
		delay_ms(50);
#else
		delay(50);
#endif

	}
	cleardevice();
}





