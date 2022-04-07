#define _CRT_SECURE_NO_WARNINGS

#include "../includes/Autorization.h"
#include <string.h>
#include "sqlite3.h"
#include <stdlib.h>
int callback_sum(void *data, int argc, char **argv, char **azColName) {
	int * count = (int*)data;
	*count = atoi(argv[0]);
	return 0;
}

int number_of_sold(sqlite3 *db, char *zErrMsg, int rc, int code) {
	int count;
	char sql[256] = "SELECT sum(quantity) FROM RECEIPT_SALE WHERE operation_code = 2";
	sprintf(sql, "%s AND compact_disk = %d;", sql, code);
	rc = sqlite3_exec(db, sql, callback_sum, &count, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return (-1);
	}
	return count;
}

int number_of_applicants(sqlite3 *db, char *zErrMsg, int rc, int code) {
	int count;
	char sql[256] = "SELECT sum(quantity) FROM RECEIPT_SALE WHERE operation_code = 1";
	sprintf(sql, "%s AND compact_disk = %d;", sql, code);
	rc = sqlite3_exec(db, sql, callback_sum, &count, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return (-1);
	}
	return count;
}
int quantity_check(sqlite3 *db, char *zErrMsg, int rc, int new_quantity, int code) {
	int sold = number_of_sold(db, zErrMsg, rc, code);
	int applicants = number_of_applicants(db, zErrMsg, rc, code);

	if (applicants - sold >= new_quantity) {
		return 0;
	}
	else
	{
		return 1;
	}

}

