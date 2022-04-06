#ifndef AUTORIZATION_LAB_5
#define AUTORIZATION_LAB_5

#include <stdio.h>

struct user_info {
	char* login;
	char* password;
	char* type;
};

void ask_login_and_password(char **login, char **password);
int check_login_and_password(char db_name[256], char* login, char* password, struct user_info *user);
#endif