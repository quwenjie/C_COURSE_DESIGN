#ifndef ALG_H
#define ALG_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//define some public algorithms here
void swap(int* a, int* b);
void buildnext(char* str, int next[]);			//build array next for kmp algorithm
int kmp_match(char* mainstr, char* templatestr);		//use kmp algorithm to do string match
#define MAX_CHAR 26
struct ACNode
{
	ACNode* fail;
	ACNode* tr[MAX_CHAR];
	int e;
}; 
ACNode* ACnewnode();
void ACinsert(ACNode* u, char* s);
void ACbuild(ACNode* root);
char* ACquery(ACNode* root, char* t);
#endif
