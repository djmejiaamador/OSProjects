#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define CYLINDER_SIZE 5000
// #define REQUEST 1000
#define REQUEST 10

void printArray(int *data, int size);
void initializeData( int  *data, int size);
int generateRequest();
int cscan(int* request,int start );
int scan(int* request,int start);
int FirstComeFirstServe(int* request,int start);
int shortestSeekingTimeFirst(int* request,int start);
int* sort(int * ranArray);



//int request[1000];


int request[10]= {2069, 1212, 2296, 2800, 544,1618, 356,1523, 4965, 3681}; 


int generateRequest(){
	// unsigned int seed = CYLINDER_SIZE;
	// srand(seed);
	// int i;
	// for(i = 0; i < 1000; i++){
	// 	request[i] = rand() % 5000;
	// }

	// printArray(request,1000);


	return 1;
}

int firstComeFirstServe(int* request,int start){
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	int i;
	int distance = 0;
	initializeData(request_serviced, REQUEST);

	for( i = 0; i < REQUEST; i++){

		if( request_serviced[i] == -1 ) {
			printf("request %d \n", request[i]);
			request_serviced[i] = 1;
			cylinders += abs(request[i] - head);
			head = request[i];
			
		}
	}
	return cylinders;
}

int shortestSeekingTimeFirst(int* request,int start){
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	
	int reorder[REQUEST];
	// must find min distance
	int minDistance = CYLINDER_SIZE+1;
	for( i = 0; i < REQUEST; i++){
		if( abs(head-request[i]) < min ) {
			minDistance = request[i];	
		}
	}
	printf(" shorsters seeking is:%d\n",minDistance );


	
	return cylinders;
}

int cscan(int* request, int start ){
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	int i;
	int distance = 0;
	initializeData(request_serviced, REQUEST);


	request = sort(request);
	for( i = 0; i < REQUEST; i++){

		if( (request[i] > head) && (request_serviced[i] == -1) ) {
			printf("request %d \n", request[i]);
			request_serviced[i] = 1;
			cylinders += request[i] - head;
			head = request[i];
			
		}
	}

	//after it reached the highest got to max
	cylinders += 4999 - head;
	// from max to min 
	cylinders += 4999;
	// HEAD NOW 0	
	head = 0;

	// now go to max
	for( i = 0; i < REQUEST; i++){
		if(request[i] > head && request_serviced[i] == -1){
			printf("request %d \n", request[i]);
			request_serviced[i] = 1;
			cylinders += request[i] - head;
			head = request[i];
		}
	}

	return cylinders;
	
}

int scan(int* request, int start ){
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	int i;
	int distance = 0;
	initializeData(request_serviced, REQUEST);

	request = sort(request);
	printf("---------Scan---------\n");

	//adding all values < head 
	for( i = REQUEST-1; i >=0; i--){
		if( (request[i] < head) && (request_serviced[i] == -1) ) {
			request_serviced[i] = 1;
			cylinders += head-request[i];
			head = request[i];
		}
	}

	//go to 0
	cylinders += head;
	
	// HEAD NOW 0	
	head = 0;

	// now adding all values > head 
	for( i = REQUEST-1; i >=0; i--){
		if( (request[i] > head) && (request_serviced[i] == -1) ) {
			request_serviced[i] = 1;
			cylinders += request[i] - head;
			head = request[i];
		}
	}

	return cylinders;
}



/* Prints given array */
void printArray(int *data, int size){
     int i = 0;
    for(i = 0 ; i < size ; i++){
        printf("data[%d] is: %d\n", i, data[i]);
    }
}

/* Sets all indexes in a given array to -1 */
void initializeData( int  *data, int size){
    int i = 0;
    for(i = 0 ; i < size ; i++){
        data[i] = -1 ;
    }
}


int* sort(int * ranArray) {

	int i = 0;
	int j = 0;
	int* aTest = malloc(sizeof(int) * sizeof(ranArray)/ranArray[0]);  

	for(i = 0; i < 10; i++) {
		aTest[i] = 0;
	}
	for(i = 0; i < 10; i++) {
		aTest[i] = ranArray[i];
	}

	int a;
	for (i = 0; i < 10; ++i) {
       	for (j = i + 1; j < 10; ++j) {
            if (aTest[i] > aTest[j]) {
                a =  aTest[i];
                aTest[i] = 	aTest[j];
                aTest[j] = a;
            }
        }
    }

	return aTest;
}




int main(int argc, char **argv)
{
	
	printf("argc is: %d\n", argc);
	
	if( argc > 2){
		printf("Only one input please\n");
		return -1;
	}
	int i;
	for( i = 0 ; i < argc ; i++){
		printf(" argv[%d] %s\n",i, argv[i]);
        
    }

	generateRequest();
	printf("first come, first serve is:%d\n", firstComeFirstServe(request,atoi(argv[1])) );
	printf("shortest Seeking Time First:%d\n", shortestSeekingTimeFirst(request,atoi(argv[1])) );
	printf("CScan is: %d\n", cscan(request, atoi(argv[1]) ) );
	printf("Scan is: %d\n", scan(request, atoi(argv[1]) ) );

}