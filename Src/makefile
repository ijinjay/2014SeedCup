# 标志
CC = gcc
CFLAGS = -Wall -std=c99
rm = rm -rf

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< -g

SeedCup: regularExpression.o fileHandle.o SeedCup.o
	$(CC) $(CFLAGS) $^ -o SeedCup -g

clean:
	$(rm) *.o SeedCup.exe output*.txt
install:
	cp SeedCup ..\bin\SeedCup

