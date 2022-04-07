#define _CRT_SECURE_NO_WARNINGS

#include "../includes/Autorization.h"
#include "../includes/FunctionChecker.h"
#include <string.h>
#include "sqlite3.h"
#include <stdlib.h>
/*
* adds user to USERS table.
*/

int insert_user(char db_name[256], struct user_info* user) {
	sqlite3 *database;
	char *ErrorMessage = 0;
	int rc;
	char sql[256] = "\0";

	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(0);
	}

	char login[50];
	char password[50];
	printf("Input login, please: ");
	scanf("%s", login);
	getchar();
	printf("Input password, please: ");
	scanf("%s", password);
	getchar();
	sprintf(sql, "INSERT INTO USERS (login, password, type) values ('%s','%s','buyer')", login, password);
	rc = sqlite3_exec(database, sql, 0, 0, &ErrorMessage);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", ErrorMessage);
		sqlite3_free(ErrorMessage);
	}
	else
	{
		user->login = malloc(50); // strlen(login) + 1
		strcpy((*user).login, login);
		user->password = malloc(50); // strlen(password) + 1
		strcpy((*user).password, password);
		user->type = malloc(50); // strlen(argv) + 1
		strcpy((*user).type, "buyer");
		printf("Nice to see you %s!", (*user).login);
	}
	sqlite3_close(database);
	return 0;
}

/*
 *add CD to COMPACT_DISK table. 
*/

int insert_compact_disc(char db_name[256]) {
	sqlite3 *database;
	char *zErrMsg = 0;
	int rc;
	char sql[256 * 5] = "\0";

	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(0);
	}

	int code;
	char date[16], company[256];
	float price;
	printf("Input code, please: ");
	scanf("%d", &code);
	getchar();
	printf("\nInput production date, please(yyyy-mm-dd): ");
	scanf("%s", date);
	getchar();
	printf("\nInput company, please: ");
	fgets(company, 256, stdin);
	company[strcspn(company, "\n")] = '\0';
	printf("\nInput price, please: ");
	scanf("%f", &price);
	getchar();
	sprintf(sql, "BEGIN TRANSACTION;" \
		"INSERT INTO COMPACT_DISC (code, production_date, company, price) values (%d,'%s','%s',%.2f);", 
		code, date, company, price);
	int choise;
	char title[256], author[256], performer[256];
	do {
		printf("\nInput song title: ");
		fgets(title, 256, stdin);
		title[strcspn(title, "\n")] = '\0';
		printf("\nInput song author: ");
		fgets(author, 256, stdin);
		author[strcspn(author, "\n")] = '\0';
		printf("\nInput song performer: ");
		fgets(performer, 256, stdin);
		performer[strcspn(performer, "\n")] = '\0';
		sprintf(sql, "%s INSERT INTO MUSIC_PERFORMER (compact_code, title, author, performer) values (%d, '%s','%s','%s');", sql,code, title, author, performer);
		printf("\nInput 1 to add another song or input 0 to exit: ");
		scanf("%d", &choise);
		getchar();
	} while (choise == 1);
	sprintf(sql, "%s COMMIT;",sql);
	rc = sqlite3_exec(database, sql, 0, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		printf("\nSuccess!\n");
	}
	sqlite3_close(database);
	return 0;
}

/*
	add information to RECEIPT_SALE table.
*/

int insert_receipt_sale(char db_name[256]){
	sqlite3 *database;
	char *zErrMsg = 0;
	int rc;

	rc = sqlite3_open(db_name, &database);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		return(1);
	}

	char date[16];
	int oper_code, cd_code, quantity;
	char sql[256] = "\0";
	printf("Input date of operation(yyyy-mm-dd): ");
	scanf("%s", date);
	getchar();
	printf("\nInput the operation code (1 - receipt, 2 - sale): ");
	scanf("%d", &oper_code);
	getchar();
	if (oper_code < 1 && oper_code > 2) {
		printf("\nInvalid value!\n");
		sqlite3_close(database);
		return 1;
	}
	printf("\nInput CD code: ");
	scanf("%d", &cd_code);
	getchar();
	printf("\nInput quantity: ");
	scanf("%d", &quantity);
	getchar();
	if (oper_code==2 && quantity_check(database, zErrMsg, rc, quantity, cd_code)) {
		printf("\nInvalid value!\n");
		sqlite3_close(database);
		return 1;
	}
	sprintf(sql, "INSERT INTO RECEIPT_SALE (operation_date, operation_code, compact_disk, quantity) values ('%s', %d, %d, %d);" \
		"INSERT INTO RELATIONS (id_operation, code) values ((SELECT id_operation FROM RECEIPT_SALE ORDER BY id_operation DESC LIMIT 1), %d)",
		date, oper_code, cd_code, quantity, cd_code);

	rc = sqlite3_exec(database, sql, 0, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		printf("\nSuccess!\n");
	}
	sqlite3_close(database);
	return 0;
}
