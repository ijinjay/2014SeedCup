# 标志
CC = gcc
CFLAGS = -Wall -std=c99
rm = rm -rf
# windows 平台
# rm = del /q

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

SeedCup: regularExpression.o fileHandle.o SeedCup.o
	$(CC) $(CFLAGS) $^ -o SeedCup 

clean:
	$(rm) *.o SeedCup.exe output*.txt
install:
	cp SeedCup ..\bin\SeedCup

