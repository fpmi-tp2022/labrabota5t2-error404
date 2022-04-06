#ifndef FUNC_LAB_5
#define FUNC_LAB_5

#include <stdio.h>
#include "sqlite3.h"
#include "Autorization.h"

int quantity_check(sqlite3 *db, char *zErrMsg, int rc, int new_quantity, int code);

#endif

