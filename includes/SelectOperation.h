//by Nastassia Katsuba

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Autorization.h"
#include "sqlite3.h"

static int callback(void *data, int argc, char **argv, char **azColName);
static int callbackArrSizeCR(void *data, int argc, char **argv, char **azColName);
static int callbackSumRes(void *data, int argc, char **argv, char **azColName);
static int callbackArraySize(void *data, int argc, char **argv, char **azColName);
static int callbackMC(void *data, int argc, char **argv, char **azColName);
static int callbackPP(void *data, int argc, char **argv, char **azColName);
static int callbackResPP(void *data, int argc, char **argv, char **azColName);
static int callbackPA(void *data, int argc, char **argv, char **azColName);
int compactRemaind(char db_name[256]);
int quantityCostPeriod(char db_name[256]);
int maxCompact(char db_name[256]);
int popularPerformer(char db_name[256]);
int popularAuthor(char db_name[256]);
int receivedSold(char db_name[256], char *dateFirst, char* dateSecond);
int compactPeriod(char db_name[256], int code, char *dateFirst, char *dateSecond);