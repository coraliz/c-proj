
all: maman14

maman14: main.o files.o firstPass.o secondPass.o errorHandler.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o
	gcc -g -Wall -ansi -pedantic main.o files.o firstPass.o secondPass.o errorHandler.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o  -lm -o maman14

%.c: %.h

%.o: %.c
	gcc -c -g -Wall -ansi -pedantic $^ -o $@

clean:
	rm -rf *.o *.exe