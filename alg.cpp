#include "alg.h"
#include "ctype.h"
void swap(int* a, int* b)
{
	int t;
	t = *a;
	*a = *b;
	*b = t;
}
void buildnext(char* str, int next[])
{
	int x = 1, now = 0;
	next[0] = 0;
	while (str[x])
	{
		if (str[x] == str[now])
		{
			now += 1;
			next[x++] = now;

		}
		else if (now)
		{
			now = next[now - 1];
		}
		else
		{
			next[x++] = 0;
		}
	}
}
int kmp_match(char* mainstr, char* templatestr)
{
	int len2 = strlen(templatestr);
	int* next;
	int tar = 0, pos = 0;
	next = (int*)malloc((len2) * sizeof(int));
	buildnext(templatestr, next);
	while (mainstr[tar])
	{
		if (mainstr[tar] == templatestr[pos])
		{
			tar++;
			pos++;
		}
		else if (pos)
		{
			pos = next[pos - 1];
		}
		else
		{
			tar += 1;
		}
		if (!templatestr[pos])
		{
			free(next);
			return tar - pos;
		}
	}
	free(next);
	return -1;
}
ACNode* ACnewnode()
{
	ACNode* ret;
	ret =(ACNode*) malloc(sizeof(ACNode));
	memset(ret, 0, sizeof(ACNode));
	return ret;
}
void ACinsert(ACNode* root, char* s)
{
	int i;
	char tmp;
	ACNode* u=root;
	for (i = 0; s[i]; ++i)
	{
		tmp = s[i] - 'a';
		if (!u->tr[tmp])
		{
			u->tr[tmp] = ACnewnode();
			u->tr[tmp]->fail = root;
		}
		u = u->tr[tmp];
	}
	u->e=strlen(s);
}
void ACbuild(ACNode* root) 
{
	ACNode *q[200],*u;
	int l = 0, r = 0;
	root->fail = root;
	for (int i = 0; i < MAX_CHAR; ++i) 
	{
		if (root->tr[i]) 
		{
			q[r++]= root->tr[i];
		}
	}
	while (l<r) 
	{
		u = q[l++];
		if(!u->fail)
			u->fail = root;
		for (int i = 0; i < MAX_CHAR; ++i)
		{
			if (u->tr[i])
			{
				u->tr[i]->fail = u->fail->tr[i];
				q[r++]=u->tr[i];
			}
			else
			{
				u->tr[i] = u->fail->tr[i];
			}
		}
	}
}

char* ACquery(ACNode* root,char* t) 
{
	int i,fd=0,k=0,len;
	ACNode* u = root,*j;
	
	char tmp,* ret;
	len = strlen(t);
	ret = (char*)malloc(len);
	memset(ret, 0, len);
	for (i = 0; t[i]; i++) 
	{
		fd = 0;
		tmp = t[i]-'a';
		if (tmp < 0 || tmp >= 26)			//there is only 26 letters in the TRIE,	we definitely don't have this letter.
		{
			u = root;
			continue;
		}
		u = u->tr[tmp];
		if (u == NULL)
			u = root;
		for (j = u; j &&j!=j->fail ; j = j->fail) 
		{
			if (j->e)
			{
				if (i >= j->e && (isdigit(t[i - j->e]) || isalpha(t[i - j->e])))		//the letter before illegal
				{
					continue;
				}
				if (i + 1 < len && (isdigit(t[i + 1]) || isalpha(t[i + 1])))           //the letter after illegal
				{
					continue;
				}
				for (k = i - (j->e) + 1; k <= i; k++)
				{
					ret[k] = 1;
				}
			}
		}
	}
	return ret;
}