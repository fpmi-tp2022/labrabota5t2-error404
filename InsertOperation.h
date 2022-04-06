
#ifndef INSERT_LAB_5
#define INSERT_LAB_5

#include "Autorization.h"
#include <stdio.h>
#include "sqlite3.h"

int insert_user(char db_name[256], struct user_info* user);
int insert_compact_disc(char db_name[256]);
int insert_receipt_sale(char db_name[256]);

#endif
