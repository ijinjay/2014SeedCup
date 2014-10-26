# 标志
CC = gcc
CFLAGS = -Wall -std=c99
# Linux & Unix平台
# rm = rm -rf
# cp = cp
# windows 平台
rm = del /q
cp = copy /b

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< 

SeedCup: regularExpression.o fileHandle.o SeedCup.o
	$(CC) $(CFLAGS) $^ -o SeedCup.exe 

clean:
	$(rm) *.o SeedCup.exe output*.txt
install:
	$(cp) SeedCup.exe ..\Bin\SeedCup.exe

