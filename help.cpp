#include "menu.h"
#include "draw.h"
void about()    //show about diaglog
{
	char s[2][50] = {"��Borland C++�ı��༭��","����������Ľܡ�����ɺ����������"};
	drawattention("��������",s,2);
}
void introduction()   //show introduction dialog
{
	char s[2][50] = { "��������ļ���д���ı��༭�������滻��","�﷨�����ȹ��ܡ�" };
	drawattention("����",s,2);
}