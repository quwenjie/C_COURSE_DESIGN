#include "keyboard.h"
#ifdef __BORLANDC__
#include <bios.h>
#include <conio.h>
#else
#include <graphics.h>
#include <stdio.h>
#endif

int get_c(void)
{
	int bios;
	if (!kbhit())
		return 0;
#ifdef __BORLANDC__
	bios = bioskey(0);
	if ((bios & 0x00ff) != 0)//������ͨ�ļ�
	{
		if (bios == 0x1c0d)
			return '\n';
		else //ȡ�Ͱ�λ
			return bios & 0x00ff;
	}
	else
	{
		return bios;
	}
#else
	bios = getch();
	if (bios == 13)
		return '\n';
	if (bios == 37+256)		//���ܼ�>=256 ��
		return 0x4B00; 
	if (bios == 38+256)		//���ܼ�>=256 ��
		return 0x4800;
	if (bios == 39+256)		//���ܼ�>=256 ��
		return 0x4D00;
	if (bios == 40+256)		//���ܼ�>=256 ��
		return 0x5000;
	return bios;
#endif
}

