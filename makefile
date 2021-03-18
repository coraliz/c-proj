
all: assembler

assembler: main.o files.o firstPass.o secondPass.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o
	gcc -g -Wall -ansi -pedantic main.o files.o firstPass.o secondPass.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o  -lm -o assembler

%.c: %.h

%.o: %.c
	gcc -c -g -Wall -ansi -pedantic $^ -o $@

clean:
	rm -rf *.o *.exe *.ent *.ext *.ob