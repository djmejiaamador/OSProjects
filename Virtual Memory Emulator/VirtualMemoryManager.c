#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CHAR_BUFF 10
#define PAGE_TABLE_SIZE 256
#define P

int pageTable[PAGE_TABLE_SIZE];
int psysicalMemory[65536];

int getPageNum( int num );
void printArray( int  *data, int size);
void initializeData( int  *data, int size);
int getPhysical(int num);
int getOffset(int num);
void find( int logical);


/// dividde by 256 and get page 
// frame numebr - page table [ logical address/ 256]

// frame number + 
// offset = logical address & 255

//

/* 
    *TLB is a structure that has that both page num and frame number  16 . 
    * since there is no gareetee that the index will corresponed in the TLB.
    *Could be a 2d array 
*/


/*
    * Physical address =  logical address / 256 ( bit right shifted by 8) 

    * look into modf



*/


//TODO add Documentation
int getOffset(int num){
    double fullNum, intPart, decPart;

    fullNum = (double)num/256;
    //printf("Full number is: %f\n", fullNum);
    decPart = modf(fullNum, &intPart);

    double offset = decPart * 256;
    //printf("decimal is %f\n", decPart );
    return (int)offset;
}

//TODO add documentation
int getPageNum(int num){
    return num/PAGE_TABLE_SIZE;
}

//TODO add documentation
void initializeData( int  *data, int size){
    int i = 0;
    for(i = 0 ; i < size ; i++){
        data[i] = -1 ;
    }
}


//TODO add documentation
void printArray(int *data, int size){
     int i = 0;
    for(i = 0 ; i < size ; i++){
        printf("data[%d] is: %d\n", i, data[i]);
    }
}


int main (void) {

    initializeData(pageTable, PAGE_TABLE_SIZE);

    //printArray(pageTable, PAGE_TABLE_SIZE);

    char line[CHAR_BUFF];
    static const char filename[] = "addresses.txt";
    FILE *file = fopen ( filename, "r" );
    if(file == NULL){
        printf("Something went wrong when opening file\n");
        return -1;
    }

    //printf("WHAT???\n");
    /* This reads each line in the file */
    while(  fgets(line,CHAR_BUFF, file) != NULL ){
        int pageNum = getPageNum(atoi(line));
        //printf("Page Number: %d\n", pageNum);
        int frameNum = getOffset(atoi(line));
        
        //printf("Frame Number: %d\n", frameNum);
    }

   return 0;
   
}