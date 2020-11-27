#include "docwin.h"
#include "alg.h"
#include <stdlib.h>
#define KEYWORD_NUM 32
void ACAutomatoninit(ACNode** root)
{
	char keyword[35][10] = {"int","float","double","char","void","short","long",
					"signed","unsigned","struct","union","typedef","sizeof","auto",
					"static","register","extern","const","volatile","return",
					"continue","break","goto","if","else","case","switch","default",
					"for","do","while"};
	int i = 0;
	*root = ACnewnode();
	for (i = 0; i < KEYWORD_NUM; i++)
	{
		ACinsert(*root, keyword[i]);
	}
	ACbuild(*root);
}
char* get_highlight(ACNode* root,ROWNODE* row)		//0 no 1 highlight
{
	int len = row->len;
	if (len == 0)
		return NULL;
	char* buf,*str;
	str = getrowstring(row, 0, len - 1, 0);
	buf=ACquery(root,str);
	free(str);
	return buf;
}