//»спользу€ оператор Select, выдать следующую информацию:

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/Autorization.h"
#include "sqlite3.h"
#include "includes/SelectOperation.h"


char **arrCharTwo;
int *arrIntOne;
int **arrIntTwo;
double *arrDoubleOne;
int sumRes;
int arraySize;
int arrSizeCR;
int cb;

// standart callback

static int callback(void *data, int argc, char **argv, char **azColName) {
	for (int i = 0; i < argc; i++) {
		printf("%20s || ", argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


// callback for arrSizeCR

static int callbackArrSizeCR(void *data, int argc, char **argv, char **azColName) {
	arrSizeCR++;
	return 0;
}


// callback for sumRes

static int callbackSumRes(void *data, int argc, char **argv, char **azColName) {
	sumRes += atoi(argv[0]);
	return 0;
}


// callback for arraySize

static int callbackArraySize(void *data, int argc, char **argv, char **azColName) {
	arraySize++;
	return 0;
}


// callback for maxCompact

static int callbackMC(void *data, int argc, char **argv, char **azColName) {
	arrIntTwo[cb] = (int*)malloc(2 * sizeof(int));
	arrIntTwo[cb][0] = atoi(argv[0]);
	arrIntTwo[cb][1] = atoi(argv[1]);
	cb++;
	return 0;
}


// callback for popularPerformer

static int callbackPP(void *data, int argc, char **argv, char **azColName) {
	arrCharTwo[cb] = (char*)malloc(50 * sizeof(char));
	strcpy(arrCharTwo[cb], argv[0]);
	cb++;
	return 0;
}


// callback for result in popularPerformer

static int callbackResPP(void *data, int argc, char **argv, char **azColName) {
	arrIntOne[cb] = atoi(argv[0]);
	cb++;
	return 0;
}


// callback for popularAuthor

static int callbackPA(void *data, int argc, char **argv, char **azColName) {
	arrIntOne[cb] = atoi(argv[0]);
	arrDoubleOne[cb] = atof(argv[1]);
	cb++;
	return 0;
}


/* по всем компактам Ц сведени€ о количестве проданных
и оставшихс€ компактов одного вида по убыванию разницы */

int compactRemaind(char db_name[256]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	char sql[256] = "";
	sprintf(sql, "SELECT code from COMPACT_DISC");
	arraySize = 0;
	cb = 0;
	rc = sqlite3_exec(db, sql, callbackArraySize, 0, &zErrMsg);
	arrIntOne = (int*)calloc(arraySize, sizeof(int));
	rc = sqlite3_exec(db, sql, callbackResPP, 0, &zErrMsg);

	int **discs = (int**)malloc(arraySize * sizeof(int));
	for (int i = 0; i < arraySize; i++) {
		discs[i] = (int*)malloc(2 * sizeof(int));
		sumRes = 0;
		char sqlSold[500];
		sprintf(sqlSold, "SELECT quantity FROM RECEIPT_SALE WHERE compact_disk = %d AND operation_code = 2", arrIntOne[i]);
		rc = sqlite3_exec(db, sqlSold, callbackSumRes, 0, &zErrMsg);
		discs[i][0] = sumRes;
		sumRes = 0;
		char sqlRec[500];
		sprintf(sqlRec, "SELECT quantity FROM RECEIPT_SALE WHERE compact_disk = %d AND operation_code = 1", arrIntOne[i]);
		rc = sqlite3_exec(db, sqlRec, callbackSumRes, 0, &zErrMsg);
		discs[i][1] = sumRes - discs[i][0];
	}

	for (int i = 0; i < arraySize; i++) {
		for (int j = 1; j < arraySize; j++) {
			if (discs[j][0] > discs[j - 1][0]) {
				int temp = discs[j][0];
				discs[j][0] = discs[j - 1][0];
				discs[j - 1][0] = temp;

				temp = discs[j][1];
				discs[j][1] = discs[j - 1][1];
				discs[j - 1][1] = temp;

				temp = arrIntOne[j];
				arrIntOne[j] = arrIntOne[j - 1];
				arrIntOne[j - 1] = temp;
			}
			else if (discs[j][0] == discs[j - 1][0]) {
				int temp = discs[j][1];
				discs[j][1] = discs[j - 1][1];
				discs[j - 1][1] = temp;

				temp = arrIntOne[j];
				arrIntOne[j] = arrIntOne[j - 1];
				arrIntOne[j - 1] = temp;
			}
		}
	}

	printf("%20s || %20s || %25s || \n", "code", "number of discs sold", "number of remaining discs");
	printf("-----------------------------------------------------------------------------\n");
	for (int i = 0; i < arraySize; i++) {
		printf("%20d || %20d || %25d || \n", arrIntOne[i], discs[i][0], discs[i][1]);
	}

	sqlite3_close(db);
	return 0;
}


/* по указанному компакту Ц сведени€ о количестве
и стоимости компактов, проданных за указанный период */

int quantityCostPeriod(char db_name[256]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	int code;
	char dateFirst[11];
	char dateSecond[11];
	printf("Enter code of the compact disk: ");
	scanf("%d", &code);
	printf("Enter the first date of period (YYYY-MM-DD): ");
	scanf("%s", dateFirst);
	printf("Enter the second date of period (YYYY-MM-DD): ");
	scanf("%s", dateSecond);

	char sql[500] = "";
	sprintf(sql, "SELECT COMPACT_DISC.code, RECEIPT_SALE.quantity, COMPACT_DISC.price FROM COMPACT_DISC INNER JOIN RELATIONS ON "\
		"COMPACT_DISC.code = RELATIONS.code INNER JOIN RECEIPT_SALE on RELATIONS.id_operation = RECEIPT_SALE.id_operation WHERE COMPACT_DISC.code = %d "\
		"AND RECEIPT_SALE.operation_code = 2 AND RECEIPT_SALE.operation_date >= '%s' AND RECEIPT_SALE.operation_date <= '%s';", code, dateFirst, dateSecond); 
	printf("%20s || %20s || %20s ||\n", "code", "quantity", "price");
	printf("------------------------------------------------------------------------\n");
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Code or date is wrong: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
	}
	sqlite3_close(db);
	return 0;
}


/* по компакту, купленному максимальное количество раз, Ц 
выдать все сведени€ о нем и музыкальных произведени€х(*) */

int maxCompact(char db_name[256]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	char sql[256] = "SELECT compact_disk, quantity FROM RECEIPT_SALE WHERE operation_code = 2";
	arraySize = 0;
	rc = sqlite3_exec(db, sql, callbackArraySize, 0, &zErrMsg);
	arrIntTwo = (int**)malloc(arraySize * sizeof(int*));
	cb = 0;
	rc = sqlite3_exec(db, sql, callbackMC, 0, &zErrMsg);

	int *quantity = (int*)calloc(arraySize, sizeof(int));
	for (int i = 0; i < arraySize; i++) {
		quantity[i] = arrIntTwo[i][1];
		for (int j = 0; j < i; j++) {
			if (arrIntTwo[j][0] == arrIntTwo[i][0]) {
				quantity[i] += arrIntTwo[j][1];
				quantity[j] = 0;
			}
		}
	}
	int max = 0;
	int maxI = 0;
	for (int i = 0; i < arraySize; i++) {
		if (max < quantity[i]) {
			max = quantity[i];
			maxI = i;
		}
	}

	char sqlRes[500];
	sprintf(sqlRes, "SELECT COMPACT_DISC.code, COMPACT_DISC.production_date, COMPACT_DISC.company, COMPACT_DISC.price, MUSIC_PERFORMER.title, "\
		"MUSIC_PERFORMER.author, MUSIC_PERFORMER.performer FROM COMPACT_DISC INNER JOIN MUSIC_PERFORMER ON "\
		"COMPACT_DISC.code = MUSIC_PERFORMER.compact_code WHERE COMPACT_DISC.code = %d", arrIntTwo[maxI][0]);
	printf("%20s || %20s || %20s || %20s || %20s || %20s || %20s || \n","code","production_date", "company", "price", "title", "author", "performer");
	printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	rc = sqlite3_exec(db, sqlRes, callback, 0, &zErrMsg);

	sqlite3_close(db);
	return 0;
}


/* по наиболее попул€рному исполнителю Ц сведени€
о количестве проданных компактов с его произведени€ми(*); */

int popularPerformer(char db_name[256]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	char sql[256] = "SELECT performer FROM MUSIC_PERFORMER";
	arraySize = 0;
	rc = sqlite3_exec(db, sql, callbackArraySize, 0, &zErrMsg);
	arrCharTwo = (char**)malloc(arraySize * sizeof(char*));
	cb = 0;
	rc = sqlite3_exec(db, sql, callbackPP, 0, &zErrMsg);

	int *count = (int*)calloc(arraySize, sizeof(int));
	for (int i = 0; i < arraySize; i++) {
		count[i] = 1;
		for (int j = 0; j < i; j++) {
			if (strcmp(arrCharTwo[j], arrCharTwo[i]) == 0) {
				count[i]++;
				count[j] = 0;
			}
		}
	}
	int max = 0;
	int maxI = 0;
	for (int i = 0; i < arraySize; i++) {
		if (max < count[i]) {
			max = count[i];
			maxI = i;
		}
	}
	char sqlSum[500];
	arraySize = 0;
	cb = 0;
	sprintf(sqlSum, "SELECT DISTINCT RECEIPT_SALE.quantity FROM RECEIPT_SALE INNER JOIN RELATIONS ON RECEIPT_SALE.id_operation = RELATIONS.id_operation "\
		"INNER JOIN COMPACT_DISC ON RELATIONS.CODE = COMPACT_DISC.code INNER JOIN MUSIC_PERFORMER ON COMPACT_DISC.code = MUSIC_PERFORMER.compact_code "\
		"WHERE MUSIC_PERFORMER.performer =  '%s' AND RECEIPT_SALE.operation_code = 2", arrCharTwo[maxI]);
	rc = sqlite3_exec(db, sqlSum, callbackArraySize, 0, &zErrMsg);
	arrIntOne = (int*)calloc(arraySize, sizeof(int));
	rc = sqlite3_exec(db, sqlSum, callbackResPP, 0, &zErrMsg);
	int sum = 0;
	for (int i = 0; i < arraySize; i++) {
		sum += arrIntOne[i];
	}
	printf("%20s || %20s || \n", "performer", "number of discs sold");
	printf("------------------------------------------------\n");
	printf("%20s || %20d || \n", arrCharTwo[maxI], sum);
	
	sqlite3_close(db);
	return 0;
}


/* по каждому автору Ц сведени€ о количестве проданных 
компактов с его запис€ми и сумме полученных денег*/

int popularAuthor(char db_name[256]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	char sql[256] = "SELECT author FROM MUSIC_PERFORMER";
	arraySize = 0;
	rc = sqlite3_exec(db, sql, callbackArraySize, 0, &zErrMsg);
	arrCharTwo = (char**)malloc(arraySize * sizeof(char*));
	cb = 0;
	rc = sqlite3_exec(db, sql, callbackPP, 0, &zErrMsg);

	for (int i = 0; i < arraySize; i++) {
		for (int j = 0; j < i; j++) {
			if (strcmp(arrCharTwo[j], arrCharTwo[i]) == 0) {
				strcpy(arrCharTwo[j], "nullptr");
			}
		}
	}
	for (int i = 0; i < arraySize; i++) {
		printf("%s\n", arrCharTwo[i]);
	}

	printf("%20s || %20s || %20s || \n", "author", "number of discs sold", "amount");
	printf("------------------------------------------------------------------------\n");

	for (int i = 0; i < arraySize; i++) {
		if (strcmp(arrCharTwo[i], "nullptr") == 0)
			continue;
		char sqlRes[500];
		arrSizeCR = 0;
		cb = 0;
		sprintf(sqlRes, "SELECT DISTINCT RECEIPT_SALE.quantity, COMPACT_DISC.price FROM RECEIPT_SALE INNER JOIN RELATIONS ON "\
			"RECEIPT_SALE.id_operation = RELATIONS.id_operation INNER JOIN COMPACT_DISC ON RELATIONS.CODE = COMPACT_DISC.code INNER JOIN MUSIC_PERFORMER "\
			"ON COMPACT_DISC.code = MUSIC_PERFORMER.compact_code WHERE MUSIC_PERFORMER.author =  '%s' AND RECEIPT_SALE.operation_code = 2", arrCharTwo[i]);
		rc = sqlite3_exec(db, sqlRes, callbackArrSizeCR, 0, &zErrMsg);
		arrIntOne = (int*)calloc(arrSizeCR, sizeof(int));
		arrDoubleOne = (double*)calloc(arrSizeCR, sizeof(double));
		rc = sqlite3_exec(db, sqlRes, callbackPA, 0, &zErrMsg);
		int sum = 0;
		double amount = 0;
		for (int q = 0; q < arrSizeCR; q++) {
			sum += arrIntOne[q];
			amount += arrIntOne[q] * arrDoubleOne[q];
		}
		printf("%20s || %20d || %20f || \n", arrCharTwo[i], sum, amount);
	}

	sqlite3_close(db);
	return 0;
}


/* —оздать функцию, котора€ за указанный период определ€ет количество поступивших
и проданных компактов по каждому виду.¬ качестве параметра передать начальную дату
периода и конечную дату периода.–езультаты занести в специальную таблицу.*/

int receivedSold(char db_name[256], char *dateFirst, char* dateSecond) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	char sql[256] = "";
	sprintf(sql, "SELECT code from COMPACT_DISC");
	arraySize = 0;
	cb = 0;
	rc = sqlite3_exec(db, sql, callbackArraySize, 0, &zErrMsg);
	arrIntOne = (int*)calloc(arraySize, sizeof(int));
	rc = sqlite3_exec(db, sql, callbackResPP, 0, &zErrMsg);

	int **discs = (int**)malloc(arraySize * sizeof(int));
	for (int i = 0; i < arraySize; i++) {
		discs[i] = (int*)malloc(2 * sizeof(int));
		sumRes = 0;
		char sqlSold[500];
		sprintf(sqlSold, "SELECT quantity FROM RECEIPT_SALE WHERE compact_disk = %d AND operation_code = 2 AND "\
			"operation_date > '%s' AND operation_date < '%s'", arrIntOne[i], dateFirst, dateSecond);
		rc = sqlite3_exec(db, sqlSold, callbackSumRes, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Date is wrong: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			return 0;
		}
		discs[i][0] = sumRes;
		sumRes = 0;
		char sqlRec[500];
		sprintf(sqlRec, "SELECT quantity FROM RECEIPT_SALE WHERE compact_disk = %d AND operation_code = 1 "\
			"AND operation_date > '%s' AND operation_date < '%s'", arrIntOne[i], dateFirst, dateSecond);
		rc = sqlite3_exec(db, sqlRec, callbackSumRes, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Date is wrong: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
			return 0;
		}
		discs[i][1] = sumRes;
	}

	char sqlRes[500];
	sprintf(sqlRes, "CREATE TABLE SOLD_RECEIVED(code integer primary key, number_of_discs_sold integer, number_of_received_discs integer)");
	rc = sqlite3_exec(db, sqlRes, 0, 0, &zErrMsg);

	printf("%20s || %20s || %25s || \n", "code", "number of discs sold", "number of received discs");
	printf("-----------------------------------------------------------------------------\n");
	for (int i = 0; i < arraySize; i++) {
		printf("%20d || %20d || %25d || \n", arrIntOne[i], discs[i][0], discs[i][1]);
		char sqlInsert[500];
		sprintf(sqlInsert, "INSERT INTO SOLD_RECEIVED(code, number_of_discs_sold, "\
			"number_of_received_discs) VALUES(%d, %d, %d)",arrIntOne[i], discs[i][0], discs[i][1]);
		rc = sqlite3_exec(db, sqlInsert, 0, 0, &zErrMsg);
	}
	sqlite3_close(db);
	return 0;
}


/* —оздать функцию, котора€ по заданному коду компакта выводит информацию о
результатах его продажи за указанный период(*) */

int compactPeriod(char db_name[256], int code, char *dateFirst, char *dateSecond) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(db_name, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}

	char sql[500] = "";
	sprintf(sql, "SELECT RECEIPT_SALE.compact_disk, RECEIPT_SALE.operation_date, RECEIPT_SALE.quantity FROM RECEIPT_SALE "\
		"WHERE RECEIPT_SALE.compact_disk = %d AND RECEIPT_SALE.operation_code = 2 AND RECEIPT_SALE.operation_date >= '%s' "\
		"AND RECEIPT_SALE.operation_date <= '%s';", code, dateFirst, dateSecond);
	printf("%20s || %20s || %20s ||\n", "code", "date","quantity");
	printf("------------------------------------------------------------------------\n");
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "Code or date is wrong: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return 0;
	}
	sqlite3_close(db);
	return 0;
}