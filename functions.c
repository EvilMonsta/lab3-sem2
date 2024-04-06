#include <stdio.h>
#include"structures.h"
#include<string.h>
#include<malloc.h>
#include <limits.h>

int TaskNumCheck(int tasknum){
    if(tasknum != 1 && tasknum != 2){
        printf ("ERROR");
        return 0;
    }
    return tasknum;
}

float checkFloat() {
    float number;
    while (scanf("%f", &number) == 0 || getchar() != '\n') {
        printf("\nincorrect input!");
        rewind(stdin);
    }
    return number;
}

int checkInt() {
    int number;
    while (scanf("%d", &number) == 0 || getchar() != '\n') {
        printf("\nincorrect input!");
        rewind(stdin);
    }
    return number;
}

int ContinueWish(int tasknumber){
    int wish = tasknumber;
    if(tasknumber==0){
        return wish;
    }
    printf("\nWant to continue work with program?\n1 - Yes\n0 - No\nYour choice: ");
    scanf("%d",&wish);
    if(wish != 0 && wish != 1){
        printf("ERROR");
        return 0;
    }
    return wish;
}

int extractIntegerPart(float num) {
    union FloatBits floatUsingNumber;
    floatUsingNumber.floatNumber = num;
    int exponent = floatUsingNumber.parts.exponent - 127;
    if (exponent < 0) {
        return 0;
    }
    unsigned int mantissa = floatUsingNumber.parts.mantissa | (1 << 23);
    unsigned int integerPart = mantissa >> (23 - exponent);
    return integerPart;
}

float taskOneInput(){
    float floatNumber;
    printf("Enter float number:\n");
    floatNumber = checkFloat();
    return floatNumber;
}

int CompareTwoWords(char* word1, char* word2){
    if(strlen(word1)!= strlen(word2)){
        return 0;
    }
    else for(int i = 0; i < strlen(word1); i ++){
            if(word1[i]!=word2[i]){
                return 0;
            }
        }
    return 1;
}

void WordCancellation(char*string, int length){
    for(int i = 0; i < length;i++ ){
        string[i]='\0';
    }
}

int findStringNumber(FILE *taskFile, char* buffer){
    int stringCounter = 0;
    while((fgets(buffer, 100, taskFile))!=NULL){
        stringCounter++;
    }
    rewind(taskFile);
    return stringCounter;
}

void spaceFinder(char*word){
    for(int i = 0; i < strlen(word);i++){
        if(word[i]==' '){
            word[i] = '_';
        }
    }
}

void invertSpace(char*word){
    for(int i = 0; i < strlen(word);i++){
        if(word[i]=='_'){
            word[i] = ' ';
        }
    }
}

int findWordNumber(char*buffer, int length){
    char IndefWord[100];
    int IndefWordLength = 0, wordCounter = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ' ' || buffer[i] == '\0' || buffer[i] == '\n') {
            wordCounter++;
            WordCancellation(IndefWord,IndefWordLength);
            IndefWordLength = 0;
        } else {
            IndefWordLength++;
            IndefWord[IndefWordLength - 1] = buffer[i];
        }
    }
    return wordCounter;
}

void readFromStrToStruct(char*buffer, char*IndefWord, int bufferLength, struct Car* cars, int currentCar){
    int flag = 0, wordCounter = 0, IndefWordLength = 0;
    for (int i = 0; i < bufferLength; i++) {
        if (buffer[i] == ' ' || buffer[i] == '\0' || buffer[i] == '\n') {
            wordCounter ++;
            if(flag > 0){
                int  j = (wordCounter - 2)/4;
                if(flag == 1) strcpy(cars[currentCar].equipments[j].bodyType, IndefWord);
                if(flag == 2) cars[currentCar].equipments[j].engineCapacity = strtof(IndefWord, NULL);
                if(flag == 3) cars[currentCar].equipments[j].startReleaseYear = atoi(IndefWord);
                if(flag == 4) cars[currentCar].equipments[j].endReleaseYear = atoi(IndefWord);
                if(flag!= 4) flag++;
                else flag=1;
            } else if(flag == 0){
                cars[currentCar].model = (char*)malloc(strlen(IndefWord));
                invertSpace(IndefWord);
                strcpy(cars[currentCar].model,IndefWord);
                flag++;
            }
            WordCancellation(IndefWord,IndefWordLength);
            IndefWordLength = 0;
        } else {
            IndefWordLength++;
            IndefWord[IndefWordLength - 1] = buffer[i];
        }
    }
}

void readFromFileToStruct(int stringNumber, char*buffer, char* IndefWord, FILE*taskFile, struct Car* cars){
    int bufferLength = 0;
    for(int currentCar = 0; currentCar < stringNumber; currentCar++){
        fgets(buffer, 100
              , taskFile);
        bufferLength = strlen(buffer);
        cars[currentCar].equipmentsCount = (findWordNumber(buffer, bufferLength) - 1)/4;
        cars[currentCar].equipments = (struct Equipment*)malloc(cars[currentCar].equipmentsCount * sizeof(struct Equipment));
        readFromStrToStruct(buffer, IndefWord,bufferLength,cars,currentCar);
    }
}

int addNewCar(struct Car*cars, int carCount){
    char buffer[100];
    printf("Enter model:\n");
    gets(buffer);
    cars[carCount].model = (char*)malloc(strlen(buffer));
    strcpy(cars[carCount].model,buffer);
    spaceFinder(cars[carCount].model);
    printf("Enter number of equipments:\n");
    cars[carCount].equipmentsCount = checkInt();
    cars[carCount].equipments = (struct Equipment*)malloc(cars[carCount].equipmentsCount * sizeof(struct Equipment));
    for(int i = 0;i < cars[carCount].equipmentsCount; i ++){
        printf("Enter bodytype:\n");
        scanf("%s",cars[carCount].equipments[i].bodyType);
        printf("Enter engine capacity:\n");
        cars[carCount].equipments[i].engineCapacity = checkFloat();
        printf("Enter year of release begin:\n");
        cars[carCount].equipments[i].startReleaseYear = checkInt();
        printf("Enter year of release end:\n");
        cars[carCount].equipments[i].endReleaseYear = checkInt();
    }
    carCount++;
    return carCount;
}

void writeToFile(char* filePath, FILE*taskFile,int carCount, struct Car* cars){
    freopen(filePath, "w+",taskFile);
    for (int i = 0; i < carCount; i++) {
        fprintf(taskFile,"%s",cars[i].model);
        fprintf(taskFile,"%c",' ');
        for(int j = 0;j < cars[i].equipmentsCount;j++){
            fprintf(taskFile,"%s %0.1f %d %d ",cars[i].equipments[j].bodyType,cars[i].equipments[j].engineCapacity,cars[i].equipments[j].startReleaseYear,cars[i].equipments[j].endReleaseYear);
        }
        fprintf(taskFile,"%c",'\n');
    }
    fclose(taskFile);
}

void pushArray(int* array, int size, int j){
    if(size>1){
        for(int i = j; i < size - 1;i ++){
            array[i] = array[i+1];
        }
    }
}

void pushArrayFloat(float* array, int size, int j){
    if(size>1){
        for(int i = j; i < size - 1;i ++){
            array[i] = array[i+1];
        }
    }
}

void pushList(char** list, int size, int j){
    if(size>1){
        for(int i = j; i < size - 1;i ++){
            strcpy(list[i],list[i+1]);
        }
    }
}

void showContent(struct Car*cars, int carCount){
    for (int i = 0; i < carCount; i++) {
        invertSpace(cars[i].model);
        printf("\n%d.%s\n",i+1,cars[i].model);
        for(int j = 0;j < cars[i].equipmentsCount;j++){
            printf("%s %0.1f %d %d ",cars[i].equipments[j].bodyType,cars[i].equipments[j].engineCapacity,cars[i].equipments[j].startReleaseYear,cars[i].equipments[j].endReleaseYear);
        }
        spaceFinder(cars[i].model);
    }

    printf("\n");
}

void freeMemory(struct Car*cars, int carCount){
    for (int i = 0; i < carCount; i++) {
        for(int j = 0;j < cars[i].equipmentsCount;j++){
            free(cars[i].equipments);
        }
    }
    free(cars);

}

int deleteElement(struct Car*cars, int carCount, int flag){
    int index;
    if(flag == -1){
        printf("Enter index of element to delete it");
        scanf("%d",&index);
        if(index<=0){
            printf("ERROR");
            exit(-1);
        }
        index = index - 1;
    }else index = flag;
    for(int i = index; i < carCount-1; i ++){
        WordCancellation(cars[i].model,strlen(cars[i].model));
        strcpy(cars[i].model,cars[i+1].model);
        cars[i].equipmentsCount = cars[i+1].equipmentsCount;
        cars[i].equipments = (struct Equipment*)realloc(cars[i].equipments,cars[i].equipmentsCount * sizeof(struct Equipment));
        for(int j = 0; j < cars[i].equipmentsCount;j++){
            WordCancellation(cars[i].equipments[j].bodyType,strlen(cars[i].equipments[j].bodyType));
            strcpy(cars[i].equipments[j].bodyType,cars[i+1].equipments[j].bodyType);
            cars[i].equipments[j].engineCapacity=cars[i+1].equipments[j].engineCapacity;
            cars[i].equipments[j].startReleaseYear = cars[i+1].equipments[j].startReleaseYear;
            cars[i].equipments[j].endReleaseYear=cars[i+1].equipments[j].endReleaseYear;
        }
    }
    return carCount-1;
}

int deleteModelsWithMinLifeCycle(struct Car*cars, int carCount){
    int *indexesToDelete = (int*)malloc(carCount*sizeof(int)),indexCount = 0,minCycleValue = INT_MAX;
    for(int i = 0; i < carCount;i ++,indexesToDelete[i] = 0){
        for(int j = 0; j < cars[i].equipmentsCount;j++){
            if(cars[i].equipments[j].endReleaseYear - cars[i].equipments[j].startReleaseYear < minCycleValue){
                minCycleValue = cars[i].equipments[j].endReleaseYear - cars[i].equipments[j].startReleaseYear;
                indexCount++;
                indexesToDelete[i] = indexCount;
            }
            else if(cars[i].equipments[j].endReleaseYear - cars[i].equipments[j].startReleaseYear == minCycleValue){
                indexesToDelete[i] = indexCount;
            }
        }
    }
    for(int i = 0; i < carCount; i ++){
        if(indexesToDelete[i] == indexCount){
            pushArray(indexesToDelete,carCount,i);
            carCount = deleteElement(cars, carCount,i);
            i--;
        }
    }
    free(indexesToDelete);
    return carCount;
}

int findAllEquipments(struct Car*cars,int carCount){
    int allEquiments = 0;
    for(int i = 0; i < carCount; i ++) {
        for (int j = 0; j < cars[i].equipmentsCount; j++)
            allEquiments++;
    }
    return allEquiments;
}

void writeLifeCyclesToArray(struct Car*cars, int carCount, float *equipmentLifeCycle){
    int counter = 0;
    for(int i = 0; i < carCount; i ++){
        for(int j = 0; j < cars[i].equipmentsCount;j++,counter++){
            equipmentLifeCycle[counter] = (float)(cars[i].equipments[j].endReleaseYear - cars[i].equipments[j].startReleaseYear);
        }
    }
}

void BubbleSort(float *array, char**list, int size){
    float tmp;
    char* tempWord = (char*) calloc(30,1);
    for(int i = size - 1; i >= 0; i--) {
        for(int j = 0; j < i; j++) {
            if(array[j] < array[j+1])
            {
                tmp = array[j];
                strcpy(tempWord,list[j]);
                array[j] = array[j+1];
                strcpy(list[j],list[j+1]);
                array[j+1] = tmp;
                strcpy(list[j+1],tempWord);
            }
        }
    }
}

int wordRepeater(char** bodytypeList,int allEquipments, char* word){
    int repeater = 0;
    for(int i = 0;i < allEquipments;i++) {
        if(CompareTwoWords(word,bodytypeList[i])){
            repeater++;
        }
    }
    return repeater;
}

int writeListWithTypicalCycle(char** bodytypeList, float * equipmentLifeCycle, int allEquipments){
    int flag = 0, repeater;
    for(int i = 0;i < allEquipments-1;i++) {
        repeater = wordRepeater(bodytypeList, allEquipments, bodytypeList[i]);
        for (int j = i + 1; j < allEquipments; j++) {
            if (CompareTwoWords(bodytypeList[i], bodytypeList[j])) {
                equipmentLifeCycle[i] += equipmentLifeCycle[j];
                pushList(bodytypeList, allEquipments, j);
                pushArrayFloat(equipmentLifeCycle, allEquipments, j);
                allEquipments--;
                flag = 1;
                j--;
            }
        }
        equipmentLifeCycle[i] = equipmentLifeCycle[i] / (float)repeater;
        if (flag == 1) {
            i--;
            flag = 0;
        }
    }
    return allEquipments;
}


int createListOfModels(struct Car*cars, int carCount, float*equipmentLifeCycle, char** bodytypeList, int allEquipments){
    allEquipments = findAllEquipments(cars,carCount);
    int counter = 0;
    for(int i = 0; i < carCount; i ++){
        for(int j = 0; j < cars[i].equipmentsCount;j++,counter++){
            bodytypeList[counter]=(char*)malloc(strlen(cars[i].equipments[j].bodyType)+1);
            strcpy(bodytypeList[counter],cars[i].equipments[j].bodyType);
        }
    }

    writeLifeCyclesToArray(cars,carCount,equipmentLifeCycle);
    allEquipments = writeListWithTypicalCycle(bodytypeList,equipmentLifeCycle,allEquipments);
    equipmentLifeCycle = (float*)realloc(equipmentLifeCycle,allEquipments*sizeof(float));
    bodytypeList = (char **) realloc(bodytypeList, (allEquipments) * sizeof(char *));
    BubbleSort(equipmentLifeCycle,bodytypeList,allEquipments);
    return allEquipments;
}

void freeArrays(float *equipmentLifeCycle, char** bodytypeList, int allEquipments){
    free(equipmentLifeCycle);
    for(int i = 0;i < allEquipments;i ++)
        free(bodytypeList[i]);
    free(bodytypeList);
}

void printList(float *equipmentLifeCycle, char** bodytypeList, int allEquipments){
        printf("List:\n");
        for(int i = 0;i < allEquipments;i ++){
            printf("%s - %0.1f\n",bodytypeList[i],equipmentLifeCycle[i]);
        }

}

void chooseFunction(struct Car* cars, int carCount, char* filePath, FILE* taskFile){
    int currentChoice = 1, allEquipments = findAllEquipments(cars,carCount),flag = 0;
    float *equipmentLifeCycle = (float*)malloc(allEquipments*sizeof(float));
    char** bodytypeList = (char**)malloc(allEquipments*sizeof(char*));
    while(currentChoice){
        printf("\nPress 1 to add\nPress 2 to delete\nPress 3 to show content\nPress 4 to delete models with min life cycle\nPress 5 to create list of bodytypes\nPress 0 to exit task\n");
        currentChoice = checkInt();
        if(currentChoice == 1) { cars = (struct Car*)realloc(cars,(carCount+1) * sizeof(struct Car));
            carCount = addNewCar(cars,carCount);
            writeToFile(filePath,taskFile,carCount,cars);
        }
        if(currentChoice == 2) { carCount = deleteElement(cars,carCount, -1);
            cars = (struct Car*)realloc(cars,(carCount+1) * sizeof(struct Car));
            writeToFile(filePath,taskFile,carCount,cars);
        }
        if(currentChoice == 3) showContent(cars, carCount);
        if(currentChoice == 4) { carCount = deleteModelsWithMinLifeCycle(cars,carCount);
            cars = (struct Car*)realloc(cars,(carCount+1) * sizeof(struct Car));
            writeToFile(filePath,taskFile,carCount,cars);
        }
        if(currentChoice == 5) { allEquipments = createListOfModels(cars,carCount,equipmentLifeCycle,bodytypeList,allEquipments);
            printList(equipmentLifeCycle,bodytypeList,allEquipments);
        }
        if(currentChoice==0) freeArrays(equipmentLifeCycle,bodytypeList,allEquipments);
    }
    exit(-1);
}

//audi sedan 1.8 1980 2000
//lada SUV 1.9 2000 2010 sedan 3.0 2019 2029
//honda coupe 3.1 2020 2031
//reno crossover 1.2 1980 2020
//Lada_grande SUV 1.2 2005 2015
//Lada_somara sedan 1.5 2005 2015
//reno_ne_predumal body 1.6 1980 2070
