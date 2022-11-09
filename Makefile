CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: build

build: s21_cat

s21_cat: 
	$(CC) $(CFLAGS) src/s21_cat.c -o s21_cat

rebuild: clean build

clean:
	rm -rf *.o
	rm -f s21_cat
