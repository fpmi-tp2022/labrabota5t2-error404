#define _CRT_SECURE_NO_WARNINGS

#include "../includes/DeleteOperation.h"
#include "../includes/Autorization.h"
#include <string.h>
#include "sqlite3.h"
#include <stdlib.h>
int* mass;

int callback_id_operation(void *data, int argc, char **argv, char **azColName) {
	int index = 0;
	for (int i = 0; i < argc; i++) {
		mass[index] = atoi(argv[i]);
		index++;
	}
	return 0;
}

int callback_count(void *data, int argc, char **argv, char **azColName) {
	int index = 0;
	int * count = (int*)data;
	*count = atoi(argv[0]);
	return 0;
}

/*
 * remove user from USERS table
*/

int delete_users(char db_name[256], struct user_info *user) {
	sqlite3 *database;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(0);
	}
	char temp_login[80] = "\0";
	strcpy(temp_login, (*user).login);
	if (!strcmp((*user).type, "admin")) {
		printf("\nEnter the login name of the account you want to delete: ");
		scanf("%s", temp_login);
	}
	char sql[256] = "\0";
	sprintf(sql, "DELETE FROM USERS WHERE login = '%s'", temp_login);
	rc = sqlite3_exec(database, sql, 0, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(database);
	if (!strcmp(temp_login, (*user).login)) {
		return 1;
	}
	return 0;
}

/*
 * remove last record from RECEIPT_SALE table
*/

int delete_last_record(char db_name[256]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	char* sql = "DELETE FROM RELATIONS WHERE id_operation = (SELECT id_operation FROM RECEIPT_SALE ORDER BY id_operation DESC LIMIT 1);" \
		"DELETE FROM RECEIPT_SALE WHERE id_operation = (SELECT id_operation FROM RECEIPT_SALE ORDER BY id_operation DESC LIMIT 1);";
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}

/*
 * remove CD from COMPACT_DISK table
*/

int delete_compact_disk(char db_name[256]) {
	int index = 0;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(db_name, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	int code;
	int count;
	printf("Input disk code: ");
	scanf("%d", &code);
	getchar();
	char sql[256] = "\0";
	sprintf(sql, "SELECT count() FROM RELATIONS WHERE code = %d;", code);
	rc = sqlite3_exec(db, sql, callback_count, &count, &zErrMsg);
	mass = (int*)malloc(count * sizeof(int));
	sprintf(sql, "SELECT id_operation FROM RELATIONS WHERE code = %d;", code);
	rc = sqlite3_exec(db, sql, callback_id_operation, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return 1;
	}
	for (int i = 0; i < count; i++) {
		sprintf(sql, "DELETE FROM RECEIPT_SALE WHERE id_operation = %d;", mass[i]);
		rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			sqlite3_close(db);
			return 1;
		}
	}
	sprintf(sql, "DELETE FROM COMPACT_DISC WHERE code = %d;", code);
	sprintf(sql, "%s DELETE FROM MUSIC_PERFORMER WHERE compact_code = %d;", sql, code);
	sprintf(sql, "%s DELETE FROM RELATIONS WHERE code = %d;", sql, code);
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return 1;
	}
	sqlite3_close(db);
	return 0;
}
