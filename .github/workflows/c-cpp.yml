bin/task: obj/main.o obj/Autorization.o obj/DeleteOperation.o obj/InsertOperation.o obj/SelectOperation.o obj/UpdateOperation.o obj/FunctionChecker.o
	gcc -l sqlite3 -o bin/task obj/main.o obj/Autorization.o obj/DeleteOperation.o obj/InsertOperation.o obj/SelectOperation.o obj/UpdateOperation.o obj/FunctionChecker.o

obj/main.o: src/main.c
	gcc -l sqlite3 -c src/main.c -o obj/main.o

obj/Autorization.o: src/Autorization.c
	gcc -l sqlite3 -c src/Autorization.c -o obj/Autorization.o

obj/DeleteOperation.o: src/DeleteOperation.c
	gcc -l sqlite3 -c src/DeleteOperation.c -o obj/DeleteOperation.o

obj/InsertOperation.o: src/InsertOperation.c
	gcc -l sqlite3 -c src/InsertOperation.c -o obj/InsertOperation.o

obj/SelectOperation.o: src/SelectOperation.c
	gcc -l sqlite3 -c src/SelectOperation.c -o obj/SelectOperation.o

obj/UpdateOperation.o: src/UpdateOperation.c
	gcc -l sqlite3 -c src/UpdateOperation.c -o obj/UpdateOperation.o

obj/FunctionChecker.o: src/FunctionChecker.c
	gcc -l sqlite3 -c src/FunctionChecker.c -o obj/FunctionChecker.o

clean:
	rm -f *.o bin/task
check:
        shell: /usr/bin/bash -e {0}
distcheck:
        shell: /usr/bin/bash -e {0}
