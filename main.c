#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "includes/Autorization.h"
#include "includes/DeleteOperation.h"
#include "includes/UpdateOperation.h"
#include "includes/SelectOperation.h"
#include "includes/InsertOperation.h"

char db_name[256] = "musicSalon.db";

int main() {
	printf("Press:\n"\
		"1 - To log in\n"\
		"2 - To register\n"\
		"0 - Exit\n"\
		"Your choice: ");
	int choice;
	int value;
	scanf("%d", &choice);
	getchar();
	struct user_info* user = (struct user_info*)malloc(sizeof(struct user_info));
	if (choice == 1){
		int res = -1;
		while (res != 0) {
			char* temp_login;
			char* temp_password;
			ask_login_and_password(&temp_login, &temp_password);
			res = check_login_and_password(db_name, temp_login, temp_password, user);
		}
	}
	else if (choice == 2) {
		insert_user(db_name, user);
	}else if (choice == 0){
		return 0;
	}else {
		printf("Wrong command!\n");
		return 1;
	}
	if (strcmp((*user).type, "buyer") == 0) {
		while (choice != 0) {
			printf("Press:\n"\
				"1 - View\n"\
				"2 - Update\n"\
				"3 - Delete\n"\
				"0 - Exit\n"\
				"Your choice: ");
			scanf("%d", &choice);
			getchar();
			switch (choice)
			{
			case 0:
				return 0;
			case 1:
				printf("Press:\n"\
					"1 - View the most popular sale\n"\
					"2 - View the most popular performer\n"\
					"3 - View all sales for the selected period\n"\
					"0 - Exit\n"\
					"Your choice: ");
				scanf("%d", &value);
				getchar();
				if (value == 1) {
					maxCompact(db_name);
				}
				else if (value == 2) {
					popularPerformer(db_name);
				}
				else if (value == 3){
					printf("Enter compact ID: ");
					int code;
					scanf("%d", &code);
					getchar();
					char dateFirst[11];
					char dateSecond[11];
					printf("Enter the first date of period (YYYY-MM-DD): ");
					scanf("%s", dateFirst);
					getchar();
					printf("Enter the second date of period (YYYY-MM-DD): ");
					scanf("%s", dateSecond);
					getchar();
					compactPeriod(db_name, code, dateFirst, dateSecond);
				}
				else if (value != 0) {
					printf("Wrong command");
				}
				break;
			case 2:
				printf("Press:\n"\
					"1 - Update user\n"\
					"0 - Exit\n"\
					"Your choice: ");
				scanf("%d", &value);
				getchar();
				if (value == 1) {
					update_users(db_name, user);
				}
				else if (value != 0) {
					printf("Wrong command");
				}
				break;
			case 3:
				printf("Press:\n"\
					"1 - Delete user\n"\
					"0 - Exit\n"\
					"Your choice: ");
				scanf("%d", &value);
				getchar();
				if (value == 1) {
					delete_users(db_name, user);
				}
				else if (value != 0) {
					printf("Wrong command!");
				}
				break;
			default:
				printf("Wrong command!\n");
				break;
			}
		}
	}
	if (strcmp((*user).type, "admin") == 0) {
		while (choice != 0) {
			printf("Enter:\n"\
				"1 - View\n"\
				"2 - Add\n"\
				"3 - Update\n"\
				"4 - Delete\n"\
				"0 - Exit\n"\
				"Your choice: ");
			scanf("%d", &choice);
			getchar();
			switch (choice)
			{
			case 0:
				return 0;
			case 1:
				printf("Press:\n"\
					"1 - View information on the number of sold and remaining compacts of the same type\n"\
					"2 - View information about the number and cost of compacts sold for a specified period\n"\
					"3 - View compact, bought the maximum number of times\n"\
					"4 - View the most popular performer\n"\
					"5 - View information about the number of sold compacts with records of all authors and the amount of money received\n"\
					"6 - View the number of received and sold compacts for each type\n"\
					"7 - View sales results for a specified period\n"\
					"0 - Exit\n"\
					"Your choice: ");
				scanf("%d", &value);
				getchar();
				if (value == 1) {
					compactRemaind(db_name);
				}
				else if (value == 2) {
					quantityCostPeriod(db_name);
				}
				else if (value == 3) {
					maxCompact(db_name);
				}
				else if(value == 4) {
					popularPerformer(db_name);
				}
				else if (value == 5) {
					popularAuthor(db_name);
				}
				else if (value == 6) {
					printf("Enter the first date of period (YYYY-MM-DD): ");
					char dateFirst[11];
					scanf("%s", dateFirst);
					getchar();
					printf("Enter the second date of period (YYYY-MM-DD): ");
					char dateSecond[11];
					scanf("%s", dateSecond);
					getchar();
					receivedSold(db_name, dateFirst, dateSecond);
				}
				else if (value == 7) {
					printf("Enter compact ID: ");
					int code;
					scanf("%d", &code);
					getchar();
					printf("Enter the first date of period (YYYY-MM-DD): ");
					char dFirst[11];
					scanf("%s", dFirst);
					getchar();
					printf("Enter the second date of period (YYYY-MM-DD): ");
					char dSecond[11];
					scanf("%s", dSecond);
					getchar();
					compactPeriod(db_name, code, dFirst, dSecond);
				}
				else if (value != 0) {
					printf("Wrong command!");
				}

				break;				
			case 2:
				printf("Press:\n"\
					"1 - Add new compact disc\n"\
					"2 - Add new operation\n"\
					"0 - Exit\n"\
					"Your choice: ");
				scanf("%d", &value);
				getchar();
				if (value == 1) {
					insert_compact_disc(db_name);
				}
				else if (value == 2) {
					insert_receipt_sale(db_name);
				}
				else if (value != 0) {
					printf("Wrong command!");
				}
				break;
			case 3:
				printf("Press:\n"\
					"1 - Update user\n"\
					"2 - Update information about compact disc\n"\
					"3 - Update title, author or music performer\n"\
					"4 - Update information about operation\n"\
					"0 - Exit\n"\
					"Your choice: ");
				scanf("%d", &value);
				getchar();
				if (value == 1) {
					update_users(db_name, user);
				}
				else if (value == 2) {
					update_compact_disc(db_name);
				}
				else if (value == 3) {
					update_misuc_performer(db_name);
				}
				else if (value == 4) {
					update_receipt_sale(db_name);
				}
				else if (value != 0) {
					printf("Wrong command!");
				}
				break;
			case 4:
				printf("Press:\n"\
					"1 - Delete user\n"\
					"2 - Delete compact disc\n"\
					"3 - Delete last operation\n"\
					"0 - Exit\n"\
					"Your choice: ");
				scanf("%d", &value);
				getchar();
				if (value == 1) {
					if (delete_users(db_name, user) == 1)
						return 1;
				}
				else if (value == 2) {
					delete_compact_disk(db_name);
				}
				else if (value == 3) {
					delete_last_record(db_name);
				}
				else if (value != 0) {
					printf("Wrong command!");
				}			
				break;
			default:
				printf("Wrong command!\n");
				break;
			}
		}
	}
	return 0;
}