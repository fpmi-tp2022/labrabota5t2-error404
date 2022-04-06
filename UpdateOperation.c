#define _CRT_SECURE_NO_WARNINGS
#include "includes/Autorization.h"
#include "includes/FunctionChecker.h"
#include <string.h>
#include "sqlite3.h"

int callback_code(void *data, int argc, char **argv, char **azColName) {
	int * count = (int*)data;
	*count = atoi(argv[0]);
	return 0;
}

/*
 * updating user login, password and, if administrator, type
*/

int update_users(char db_name[256], struct user_info* user) {
	sqlite3 *database;
	char *ErrorMessage = 0;
	int rc;
	char sql[256] = "\0";

	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(0);
	}
	int change;
	char temp[80];
	printf("What do you want to change?\nEnter 1 - change login\nEnter 2 - change password\nEnter 0 - exit\n");
	if (!strcmp((*user).type, "admin")){
		printf("Enter 3 - change type\n");
	}
	scanf("%d", &change);
	getchar();
	switch (change)
	{
	case(0):
		sqlite3_close(database);
		return 0;
	case (1):
		printf("\nInput new login: ");
		scanf("%s", temp);
		getchar();
		sprintf(sql, "UPDATE USERS SET login = '%s' WHERE login = '%s'", temp,(*user).login);
		strcpy(user->login, temp);
		break;
	case (2):
		printf("\nInput new password: ");
		scanf("%s", temp);
		getchar();
		sprintf(sql, "UPDATE USERS SET password = '%s' WHERE password = '%s'", temp, (*user).password);
		strcpy(user->password, temp);
		break;
	default:
		if (!strcmp((*user).type, "admin") && change == 3) {
			printf("\nEnter 1 - set buyer\nEnter 2 - set admin\n");
			int res;
			scanf("%d", &res);
			getchar();
			printf("\nEnter user login: ");
			char user_login[80];
			scanf("%s", user_login);
			getchar();
			switch (res)
			{
			case(1):
				sprintf(sql, "UPDATE USERS SET type = 'buyer' WHERE login = '%s'", user_login);
				strcpy(user->type, "buyer");
				break;
			case(2):
				sprintf(sql, "UPDATE USERS SET type = 'admin' WHERE login = '%s'", user_login);
				strcpy(user->type, "admin");
				break;
			default:
				printf("\nInvalid command.\n");
				return 1;
			}
		}
		else
		{
			printf("\nInvalid command.\n");
			return 1;
		}
		break;
	}
	rc = sqlite3_exec(database, sql, 0, 0, &ErrorMessage);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Not found: %s\n", ErrorMessage);
		sqlite3_free(ErrorMessage);
	}
	else
	{
		printf("\nSuccess!\n");
	}
	sqlite3_close(database);
	return 0;
}

/*
 * updating of a compact disk on columns of the company and the price.
*/

int update_compact_disc(char db_name[256]) {
	sqlite3 *database;
	char *ErrorMessage = 0;
	int rc;
	char sql[256] = "\0";


	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(0);
	}

	int change,code;
	printf("What do you want to change?\nEnter 1 - change company\nEnter 2 - change price\nEnter 0 - exit\n");
	scanf("%d", &change);
	getchar();
	
	switch (change)
	{
	case(1):
		printf("Enter disc code: ");
		scanf("%d", &code);
		getchar();
		printf("\nEnter new name company: ");
		char tempStr[256];
		fgets(tempStr, 256, stdin);
		tempStr[strcspn(tempStr, "\n")] = '\0';
		sprintf(sql, "UPDATE COMPACT_DISC SET company = '%s' WHERE code = %d", tempStr, code);
		break;
	case(2):
		printf("Enter disc code: ");
		scanf("%d", &code);
		getchar();
		printf("\nEnter new price: ");
		float tempFl;
		scanf("%f", &tempFl);
		getchar();
		sprintf(sql, "UPDATE COMPACT_DISC SET price = '%.2f' WHERE code = %d", tempFl, code);
		break;
	case(0):
		sqlite3_close(database);
		return 0;
	default:
		break;
	}

	rc = sqlite3_exec(database, sql, 0, 0, &ErrorMessage);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", ErrorMessage);
		sqlite3_free(ErrorMessage);
		return 1;
	}
	else
	{
		printf("\nSuccess!\n");
	}
	sqlite3_close(database);
	return 0;
}

/*
* updating the table music artist by title, author and artist.
*/

int update_misuc_performer(char db_name[256]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[256] = "\0";

	rc = sqlite3_open(db_name, &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	int change, id;
	char temp[256];
	printf("What do you want to change?\nEnter 1 - change title\nEnter 2 - change author\nEnter 3 - change performer\nEnter 0 - exit\n");
	scanf("%d", &change);
	getchar();

	switch (change)
	{
	case(1):
		printf("Enter id music performer: ");
		scanf("%d", &id);
		getchar();
		printf("\nEnter new title: ");
		fgets(temp, 256, stdin);
		temp[strcspn(temp, "\n")] = '\0';
		sprintf(sql, "UPDATE MUSIC_PERFORMER SET title = '%s' WHERE id = %d", temp, id);
		break;
	case(2):
		printf("Enter id music performer: ");
		scanf("%d", &id);
		getchar();
		printf("\nEnter new author: ");
		fgets(temp, 256, stdin);
		temp[strcspn(temp, "\n")] = '\0';
		sprintf(sql, "UPDATE MUSIC_PERFORMER SET author = '%s' WHERE id = %d", temp, id);
		break;
	case(3):
		printf("Enter id music performer: ");
		scanf("%d", &id);
		getchar();
		printf("\nEnter new performer: ");
		fgets(temp, 256, stdin);
		temp[strcspn(temp, "\n")] = '\0';
		sprintf(sql, "UPDATE MUSIC_PERFORMER SET performer = '%s' WHERE id = %d", temp, id);
		break;
	case(0):
		sqlite3_close(db);
		return 0;
	default:
		break;
	}

	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		printf("\nSuccess!\n");
	}
	sqlite3_close(db);
	return 0;
}

/*
* Updating the receipt_sale table by date of operation and quantity.
*/

int update_receipt_sale(char db_name[256]) {
	sqlite3 *database;
	char *ErrorMessage = 0;
	int rc;
	char sql[256] = "";

	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(0);
	}

	int change, id;
	int cd_code;
	printf("What do you want to change?\nEnter 1 - change date of operation\nEnter 2 - change quantity\nEnter 0 - exit\n");
	scanf("%d", &change);
	getchar();

	switch (change)
	{
	case(1):
		printf("Enter id operation: ");
		scanf("%d", &id);
		getchar();
		char date[16];
		printf("\nEnter new date(yyyy-mm-dd): ");
		scanf("%s", date);
		getchar();
		sprintf(sql, "UPDATE RECEIPT_SALE SET operation_date = '%s' WHERE id_operation = %d", date, id);
		break;
	case(2):
		printf("Enter id operation: ");
		scanf("%d", &id);
		getchar();
		printf("\nEnter new quantity: ");
		int quantity, oper_code;
		scanf("%d", &quantity);
		getchar();
		sprintf(sql, "SELECT compact_disk FROM RECEIPT_SALE WHERE id_operation = %d", id);
		rc = sqlite3_exec(database, sql, callback_code, &cd_code, &ErrorMessage);
		sprintf(sql, "SELECT operation_code FROM RECEIPT_SALE WHERE id_operation = %d", id);
		rc = sqlite3_exec(database, sql, callback_code, &oper_code, &ErrorMessage);
		if (oper_code == 2 && quantity_check(database, ErrorMessage, rc, quantity, cd_code)) {
			printf("\nInvalid value!\n");
			sqlite3_close(database);
			return 1;
		}
		sprintf(sql, "UPDATE RECEIPT_SALE SET quantity = %d WHERE id_operation = %d", quantity, id);
		break;
	case(0):
		sqlite3_close(database);
		return 0;
	default:
		break;
	}

	rc = sqlite3_exec(database, sql, 0, 0, &ErrorMessage);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", ErrorMessage);
		sqlite3_free(ErrorMessage);
	}
	else
	{
		printf("\nSuccess!\n");
	}
	sqlite3_close(database);
	return 0;
}