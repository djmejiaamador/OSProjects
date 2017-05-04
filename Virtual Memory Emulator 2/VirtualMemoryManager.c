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
    int pages[TLB_SIZE];
   
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
int tlb_hit = 0;
int tlb_counter = 0;

FILE    *address_file;
FILE    *backing_store;

char  char_buff[PAGE_TABLE_SIZE];

/*  get the physical address from TLB of Page table. Which ever has the value*/
int find(struct TLB* tlb, int logical){

    int pageNum = getPageNum(logical);
    int offset = getOffset(logical);
    int frame = -1;

    //Check TLB
    int j;
    for(j = 0 ; j < TLB_SIZE; j++ ){
        if(tlb->pages[j] == pageNum){
            frame = tlb->frames[j];
            tlb_hit++;
        }
    }

    //check page table
    if( frame == -1){
        if(pageTable[pageNum] == -1){
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
        tlb_counter++;
    }

    return (frame * PAGE_TABLE_SIZE) + offset ;

}

/* this reads disk and writes to physical memomry array */
int readDisk(int pageNum, int offSet){
   
    // sets the file to the starting point--- page number * 256
    if (fseek(backing_store, pageNum * PAGE_TABLE_SIZE, SEEK_SET) != 0) {
        fprintf(stderr,"Could not read file\n");
    }
    
    // writes 256 bits of info into the character buffer
    if (fread(char_buff, sizeof(signed char), PAGE_TABLE_SIZE, backing_store) == 0) {
        fprintf(stderr, "Could not read file\n");        
    }

    // get available frame number by looking for a unused index in frame array
    int availableFrame;
    for( int i = 0 ; i < PAGE_TABLE_SIZE ; i++){
        if(validFrame[i] == -1){
            validFrame[i] = 0;
            availableFrame = i;
            break;
        }
    }

    int startingIndex = PAGE_TABLE_SIZE * availableFrame; // starting index for each frame

    // Loads physical memory with info in character buffer
    int i;
    for(i = 0; i < PAGE_TABLE_SIZE; i++){
        physicalMemory[startingIndex++] = char_buff[i];
    }
      
    // becuase we had to read the file 
    pagefault++;
    pageTable[pageNum] = availableFrame;
    return availableFrame;


}

/* Calculates offset */
int getOffset(int num){
    double fullNum, intPart, decPart;

    fullNum = (double)num/256;
    //printf("Full number is: %f\n", fullNum);
    decPart = modf(fullNum, &intPart);

    double offset = decPart * 256;
    //printf("decimal is %f\n", decPart );
    return (int)offset;
}

/* Calculates page number */
int getPageNum(int num){
    return num/PAGE_TABLE_SIZE;
}

/* Sets all indexes in a given array to -1 */
void initializeData( int  *data, int size){
    int i = 0;
    for(i = 0 ; i < size ; i++){
        data[i] = -1 ;
    }
}


/* Prints given array */
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
    char line[CHAR_BUFF_SIZE];

    address_file = fopen ( "addresses.txt", "r" );
    backing_store = fopen ( "BACKING_STORE.bin", "r" );
    
    if(address_file == NULL){
        printf("Something went wrong when opening file\n");
        return -1;
    }

    /* This reads each line in the address file */
    while(  fgets(line,CHAR_BUFF_SIZE, address_file) != NULL ){
        int physical_Addr = find(&tlb,atoi(line));
        printf("Logical Address: %d  Physical memomry:  %d   value: %d \n",atoi(line), physical_Addr, physicalMemory[physical_Addr] );
        numOfTranslation++;
    }
    printf("/*------------ Final Data ---------------*/\n");
    printf("number of Translation: %d\n", numOfTranslation);
    printf("Number of Page Faults: %d\n", pagefault);
    printf("Number of page fault rate %f\n", (float)pagefault/(float)numOfTranslation);
    printf("Number of TLB Hits: %d\n", tlb_hit);
    printf("Number of TLB rate %f\n", (float)tlb_hit/(float)numOfTranslation);

    fclose(address_file);
    fclose(backing_store);

   return 0;
   
}