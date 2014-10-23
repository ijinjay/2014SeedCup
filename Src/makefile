# 标志
CC = gcc
CFLAGS = -Wall -std=c99
rm = del /q

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

SeedCup: regularExpression.o fileHandle.o SeedCup.o
	$(CC) $(CFLAGS) $^ -o SeedCup.exe

clean:
	$(rm) *.o SeedCup.exe output*.txt
install:
	copy /b SeedCup.exe ..\bin\SeedCup.exe
