maman14: main.o files.o firstPass.o secondPass.o errorHandler.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o
	gcc -g -Wall -ansi -pedantic main.o files.o firstPass.o secondPass.o errorHandler.o symbolsList.o utilsFirstPass.o dataList.o entryList.o operations.o wordsList.o  -lm -o maman14

main.o:	main.c structs.h
	gcc -c -g -Wall -ansi -pedantic main.c -o main.o

files.o: files.c
	gcc -c -g -Wall -ansi -pedantic files.c -o files.o

firstPass.o: firstPass.c
	gcc -c -g -Wall -ansi -pedantic firstPass.c -o firstPass.o

secondPass.o: secondPass.c
	gcc -c -g -Wall -ansi -pedantic secondPass.c -o secondPass.o

errorHandler.o: errorHandler.c
	gcc -c -g -Wall -ansi -pedantic errorHandler.c -o errorHandler.o

symbolsList.o: symbolsList.c
	gcc -c -g -Wall -ansi -pedantic symbolsList.c -o symbolsList.o

utilsFirstPass.o: utilsFirstPass.c
	gcc -c -g -Wall -ansi -pedantic utilsFirstPass.c -o utilsFirstPass.o

dataList.o: dataList.c
	gcc -c -g -Wall -ansi -pedantic dataList.c -o dataList.o

entryList.o: entryList.c
	gcc -c -g -Wall -ansi -pedantic entryList.c -o entryList.o

operations.o: operations.c
	gcc -c -g -Wall -ansi -pedantic operations.c -lm -o operations.o

wordsList.o: wordsList.c
	gcc -c -g -Wall -ansi -pedantic wordsList.c -o wordsList.o
