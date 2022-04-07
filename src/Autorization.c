#define _CRT_SECURE_NO_WARNINGS
#define WRONG_PAS_LOG -1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../includes/Autorization.h"
#include "sqlite3.h"

int callback_login(void *data, int argc, char **argv, char **azColName) {
	struct user_info* user = (struct user_info*)data;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(azColName[i], "login")) {
			user->login = malloc(50);
			strcpy(user->login, argv[i]);
		}
		if (!strcmp(azColName[i], "password")) {
			user->password = malloc(50);
			strcpy(user->password, argv[i]);
		}
		if (!strcmp(azColName[i], "type")) {
			user->type = malloc(50);
			strcpy(user->type, argv[i]);
		}
	}
	return 0;
}

int callback_last_id(void *data, int argc, char **argv, char **azColName) {
	int * user = (int*)data;
	*user = atoi(argv[0]);
	return 0;
}

void ask_login_and_password(char **login, char **password) {
	*login = malloc(80);
	*password = malloc(80);
	printf("Input your login: ");
	scanf("%s", *login);
	getchar();
	printf("\nInput your password: ");
	scanf("%s", *password);
	getchar();
}

int check_login_and_password(char db_name[256], char* login, char* password, struct user_info *user) {
	sqlite3 *database;
	char *ErrorMessage = 0;
	int rc;

	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(0);
	}
	char sql[256] = "SELECT id from USERS order by id desc limit 1";
	int count;
	rc = sqlite3_exec(database, sql, callback_last_id, &count, &ErrorMessage);
	for (int i = 0;i <= count ; i++) {
		sprintf(sql, "SELECT login, password, type from USERS WHERE id = %i", i);
		struct user_info userTemp;
		userTemp.login = NULL;
		userTemp.password = NULL;
		userTemp.type = NULL;
		rc = sqlite3_exec(database, sql, callback_login, &userTemp, &ErrorMessage);
		if (userTemp.login == NULL || userTemp.password == NULL || userTemp.type == NULL) {
			continue;
		}
		if (rc != SQLITE_OK) {
			printf("Error SQLITE.");
			return SQLITE_ERROR;
		}
		if (!strcmp(login, userTemp.login) && !strcmp(password, userTemp.password)) {
			sqlite3_close(database);
			*user = userTemp;
			printf("Welcom %s!\n", (*user).login);
			return SQLITE_OK;
		}
	}
	printf("Wrong login or password!\n");
	return WRONG_PAS_LOG;
}
