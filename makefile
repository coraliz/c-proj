
all: assembler

assembler:	dataList.o entriesList.o files.o generalUtils.o instructionScan.o instructionsList.o main.o scanUtils.o secondScan.o symbolsList.o firstScan.o
			gcc -g -Wall -ansi -pedantic dataList.o entriesList.o files.o generalUtils.o instructionScan.o instructionsList.o scanUtils.o secondScan.o firstScan.o symbolsList.o main.o -lm -o assembler

%.c: %.h

%.o: %.c
	gcc -c -g -Wall -ansi -pedantic $^ -o $@

clean:
	rm -rf *.o *.exe *.ent *.ext *.ob