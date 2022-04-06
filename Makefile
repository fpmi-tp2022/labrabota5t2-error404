#make file - this is a comment section
 
all:	#target name
	gcc main.c Autorization.c DeleteOperation.c FunctionChecker.c InsertOperation.c SelectOperation.c shell.c sqlite3.c UpdateOperation.c Autorization.h DeleteOperation.h FunctionChecker.h InsertOperation.h SelectOperation.h shell.h sqlite3.h UpdateOperation.h -o operations
