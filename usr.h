#ifndef USR_H
#define USR_H
typedef struct {
	char username[100];
	char code[100];
}usr;

int inusrs(usr u);
void writeusr(usr u);
void initusr(usr u);
void getstring(char* begin, char* end, char* s);
#endif
