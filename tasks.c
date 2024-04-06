#include <stdio.h>
#include"functions.h"
#include<malloc.h>
#include<string.h>
#include "structures.h"
void taskOne(){
    printf("Integer part of float number is: %d\n", extractIntegerPart(taskOneInput()));
}
void taskTwo(){
    char* filePath = "taskfile.txt",buffer[1000],IndefWord[100];
    FILE *taskFile = fopen(filePath, "r+");
    int stringNumber = findStringNumber(taskFile,buffer), carCount = stringNumber;
    struct Car* cars = (struct Car*)malloc(carCount * sizeof(struct Car));
    readFromFileToStruct(stringNumber,buffer,IndefWord,taskFile,cars);
    chooseFunction(cars,carCount,filePath,taskFile);
    freeMemory(cars,carCount);
}