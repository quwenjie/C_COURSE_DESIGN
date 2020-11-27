#include "menu.h"
#include "draw.h"
void about()    //show about diaglog
{
	char s[2][50] = {"仿Borland C++文本编辑器","本软件由屈文杰、殷梓珊合作开发。"};
	drawattention("关于作者",s,2);
}
void introduction()   //show introduction dialog
{
	char s[2][50] = { "本软件有文件读写，文本编辑，搜索替换，","语法高亮等功能。" };
	drawattention("介绍",s,2);
}