#define CONFIG_DEBUG
#pragma warning(disable:4996)
#include "usr.h"
#include<string.h>
#include<stdio.h>

/*************************************inusrs***************************************
function:check if the user is in the users list
parameters:
	usr:user container
return:
	state:1 for in,0 for not in
*******************************************************************************************/
int inusrs(usr u)
{
	FILE* fp;
	char buffer[100], s[100];
	fp = fopen("usrs.txt", "a+");
	while (fgets(buffer, 100, fp))
	{
		if (strchr(buffer, ' '))
		{
			getstring(buffer, strchr(buffer, ' ') - 1, s);
			if (strcmp(u.username, s) == 0) {
				getstring(strchr(buffer, ' ') + 1, strchr(buffer, '$') - 1, s);
				if (strcmp(u.code, s) == 0)
					return 1;
			}
		}
	}
	fclose(fp);
	return 0;
}

/*************************************writeusr****************************************
function:write user's information in corresponding file
parameters:
	usr:user container
return:
	none
*******************************************************************************************/
void writeusr(usr u)
{
	FILE* fp;
	fp = fopen("usrs.txt", "a+");
	fputs(u.username, fp);
	fputc(' ', fp);
	fputs(u.code, fp);
	fputc('$', fp);
	fputc('\n', fp);
	fclose(fp);
}

void initusr(usr u)
{
	memset(u.username, '\0', 100);
	memset(u.code, '\0', 100);
}