CFLAGS=-O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow
LIBS=-lm
EXECUTAVEL=ESTRUTURADEDADOS2

all: $(EXECUTAVEL)

$(EXECUTAVEL): main.o
	gcc main.o $(LIBS) -o $(EXECUTAVEL)

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean: 
	rm -rf *.o $(EXECUTAVEL) && clear

memcheck: $(EXECUTAVEL)
	valgrind --tool=memcheck --leak-check=full ./$(EXECUTAVEL)
	
run: $(EXECUTAVEL)
	./$(EXECUTAVEL)