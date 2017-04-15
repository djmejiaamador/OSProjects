#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CHAR_BUFF_SIZE 10
#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16


/*---- STRUCT DECLARATION ------*/
struct TLB
{
    int frames[TLB_SIZE];
    //initializeData(frames,TLB_SIZE);
    int pages[TLB_SIZE];
    //initializeData(pages,PAGE_TABLE_SIZE);
};

typedef struct TLB TLB;


/* ------------ METHODS DECLARATIONS ----------*/
int getPageNum( int num );
void printArray( int  *data, int size);
void initializeData( int  *data, int size);
int getPhysical(int num);
int getOffset(int num);
int find( TLB* tlb ,int logical);
int readDisk(int pageNum, int offSet);


/* ------------VARIABLE DECLARATIONS ---------*/

int pageTable[PAGE_TABLE_SIZE];
int validFrame[PAGE_TABLE_SIZE];
int tlb[TLB_SIZE][TLB_SIZE];
int physicalMemory[65536];
int translation = 0;
int pagefault = 0;
int tlb_miss = 0;
int tlb_counter = 0;

FILE    *address_file;
FILE    *backing_store;

char  char_buff[PAGE_TABLE_SIZE];





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

/*  get gets the physical address */

int find(struct TLB* tlb, int logical){

    int pageNum = getPageNum(logical);
    int offset = getOffset(logical);
    int frame = -1;

    //Check TLB
    int j;
    for(j = 0 ; j < TLB_SIZE; j++ ){
        if(tlb->pages[j] == pageNum){
            frame = tlb->frames[j];
        }
    }

    //check page table
    if( frame == -1){
        if(pageTable[pageNum] == -1){
            pagefault ++;
            frame = readDisk(pageNum, offset);
        }else{
            frame = pageTable[pageNum];
        }

        // after frame has been found load that page and frame into tlb
        // use of modulo to have it act as a circular array
        // will only update if need arises
        // last in last out ? 
        tlb->pages[tlb_counter%TLB_SIZE] = pageNum;
        tlb->frames[tlb_counter%TLB_SIZE] = frame; 

    }
    

   

    //printf(" is: %d \n", frame);
    return (frame * PAGE_TABLE_SIZE) + offset ;

}

/* this reads disk and writes to physical memomry array */
int readDisk(int pageNum, int offSet){
   
    //TODO  REWORD theses 
    // first seek to byte CHUNK in the backing store
    // SEEK_SET in fseek() seeks from the beginning of the file
    if (fseek(backing_store, pageNum * PAGE_TABLE_SIZE, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking in backing store\n");
    }
    
    // read CHUNK bytes from the backing store to the buffer
    if (fread(char_buff, sizeof(signed char), PAGE_TABLE_SIZE, backing_store) == 0) {
        fprintf(stderr, "Error reading from backing store\n");        
    }

    // get available frame number
    int availableFrame;
    for( int i = 0 ; i < PAGE_TABLE_SIZE ; i++){
        if(validFrame[i] == -1){
            validFrame[i] = 0;
            availableFrame = i;
            break;
        }
    }


    int startingIndex = PAGE_TABLE_SIZE * availableFrame;


    int i;
    for(i = 0; i < PAGE_TABLE_SIZE; i++){
        physicalMemory[startingIndex++] = char_buff[i];
    }
    
    

    pagefault++;
    pageTable[pageNum] = availableFrame;
    return availableFrame;


}

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

     TLB tlb;

    initializeData(pageTable, PAGE_TABLE_SIZE);
    initializeData(validFrame, PAGE_TABLE_SIZE);
    initializeData(tlb.frames, TLB_SIZE);
    initializeData(tlb.pages, TLB_SIZE);

    int numOfTranslation  = 0; 


    //printArray(pageTable, PAGE_TABLE_SIZE);

    char line[CHAR_BUFF_SIZE];
    
    address_file = fopen ( "addresses.txt", "r" );
    backing_store = fopen ( "BACKING_STORE.bin", "r" );
    if(address_file == NULL){
        printf("Something went wrong when opening file\n");
        return -1;
    }

    /* This reads each line in the file */
    while(  fgets(line,CHAR_BUFF_SIZE, address_file) != NULL ){
        int pageNum = getPageNum(atoi(line));
        int offNum = getOffset(atoi(line));
        int physical_Addr = find(&tlb,atoi(line));

        // printf("Logical Address: %d  Page Num: %d   offSet: %d   value:  %d  \n",atoi(line), pageNum, offNum, find(atoi(line)) );
        //printf("Logical Address: %d  Page Num: %d   offSet: %d   Physical Address:  %d   value: %d \n",atoi(line), pageNum, offNum, find(atoi(line)), physicalMemory[find(atoi(line))] );
        printf("Logical Address: %d  Physical memomry:  %d   value: %d \n",atoi(line), physical_Addr, physicalMemory[physical_Addr] );

        numOfTranslation++;


    }



    printf("/*------------ Final Data ---------------*/\n");


    printf("number of Translation: %d\n", numOfTranslation);
    printf("Number of Page Faults: %d\n", pagefault);

   return 0;
   
}