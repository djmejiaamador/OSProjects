#include<stdio.h>
#include <stdlib.h>
#include <string.h>
// #define CYLINDER_SIZE 199
#define CYLINDER_SIZE 5000
#define REQUEST 1000
// #define REQUEST 10

void printArray(int *data, int size);
void initializeData( int  *data, int size);
int generateRequest();

int FirstComeFirstServe(int* request,int start);
int shortestSeekingTimeFirst(int* request,int start);
int scan(int* request,int start);
int look(int* request,int start); 
int cscan(int* request,int start );
int clook(int* request,int start );


int copy_array( int * arr , int* cpy);



int request[REQUEST];

/** for other test */
// int request[REQUEST]= {2069, 1212, 2296, 2800, 544,1618, 356,1523, 4965, 3681}; 
// 
// int request[REQUEST]= {95, 180, 34, 119, 11, 123, 62, 64}; 

int generateRequest(){
	int i;
	for(i = 0; i < REQUEST; i++){
		request[i] = rand() % 5000;
	}

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
		// printf("\thead: %d  next:%d  diference:%d\n", head,request[i],  abs(request[i] - head));
		cylinders += abs(head- request[i]);
		head = request[i];
			
	}
	return cylinders;
}


int shortestSeekingTimeFirst(int* request,int start){
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	int i;
	int index_counter = 0;
	int temp_head = start;

	//int lo = CYLINDER_SIZE+1;
	initializeData(request_serviced,REQUEST);
	while(index_counter < REQUEST){
		int lo = CYLINDER_SIZE;
		int min_index;
		for(i=0; i< REQUEST;i++){
			if( (abs(temp_head - request[i]) < lo) && (request_serviced[i] == -1) ) {
				lo = abs(temp_head - request[i]) ; 
				min_index = i; 
			}
		}
		request_serviced[min_index] = 1;
		cylinders  += abs(temp_head - request[min_index]);
		temp_head= request[min_index];
		index_counter++;
	}

	return cylinders;
}

int cscan(int* request, int start ){
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	int i;
	int distance = 0;
	initializeData(request_serviced, REQUEST);


	int array_cpy[REQUEST];
	copy_array(array_cpy,request);

	// printArray(array_cpy,REQUEST);


	for( i = 0; i < REQUEST; i++){

		if( (array_cpy[i] > head) && (request_serviced[i] == -1) ) {
			// printf("\thead: %d  next:%d  diference:%d\n", head,array_cpy[i],  abs(array_cpy[i] - head));
			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head = array_cpy[i];
			
		}
	}

	// printf("after it reached the highest got to max\n");
	//printf("head: %d  diff to max: %d\n", head, CYLINDER_SIZE - head);
	cylinders += (CYLINDER_SIZE-1) - head;
	// printf("head: %d  diff to max: %d\n", head, CYLINDER_SIZE -1 - head);
	// from max to min 
	cylinders += CYLINDER_SIZE-1;
	// HEAD NOW 0	
	head = 0;
	// printf("\n");
	// now go to max
	for( i = 0; i < REQUEST; i++){
		if(array_cpy[i] > head && request_serviced[i] == -1){
			// printf("\thead: %d  next:%d  diference:%d\n", head,array_cpy[i],  abs(array_cpy[i] - head));

			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head = array_cpy[i];
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

	int array_cpy[REQUEST];
	copy_array(array_cpy,request);
	//adding all values < head 
	for( i = REQUEST-1; i >=0; i--){
		if( (array_cpy[i] < head) && (request_serviced[i] == -1) ) {
			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head =array_cpy[i];
		}
	}

	//go to 0
	cylinders += head;
	
	// HEAD NOW 0	
	head = 0;

	// now adding all values > head 
	for( i = REQUEST-1; i >=0; i--){
		if( (array_cpy[i] > head) && (request_serviced[i] == -1) ) {
			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head = array_cpy[i];
		}
	}

	return cylinders;
}

int clook(int* request,int start ){
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	int i;
	int distance = 0;
	initializeData(request_serviced, REQUEST);

	int array_cpy[REQUEST];
	copy_array(array_cpy,request);

	for( i = 0; i < REQUEST; i++){
		//printf("request %d \n", array_cpy[i]);
		if( (array_cpy[i] > head) && (request_serviced[i] == -1) ) {
			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head = array_cpy[i];
		}
	}

	// now go to max

	for( i = 0; i < REQUEST; i++){
		if(request_serviced[i] == -1 ) {
			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head = array_cpy[i];
		}
	}

	return cylinders;
}


int look(int* request,int start){
	
	int head = start;
	int cylinders = 0; 
	int request_serviced[REQUEST];
	int i;
	int distance = 0;
	initializeData(request_serviced, REQUEST);

	int array_cpy[REQUEST];
	copy_array(array_cpy,request);
	
	//adding all values < head 
	for( i = REQUEST-1; i >=0; i--){
		if( (array_cpy[i] < head) && (request_serviced[i] == -1) ) {
			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head = array_cpy[i];
		}
	}


	// now adding all values > head 
	for( i = REQUEST-1; i >=0; i--){
		if( (array_cpy[i] > head) && (request_serviced[i] == -1) ) {
			request_serviced[i] = 1;
			cylinders += abs(array_cpy[i] - head);
			head = array_cpy[i];
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

int copy_array( int * arr , int* cpy){	
	int i = 0;
	int j = 0;
	for(i = 0; i < REQUEST; i++) {
		arr[i] = 0;
	}
	for(i = 0; i < REQUEST; i++) {
		arr[i] = cpy[i];
	}

	int a;
	for (i = 0; i < REQUEST; ++i) {
       	for (j = i + 1; j < REQUEST; ++j) {
            if (arr[i] > arr[j]) {
                a =  arr[i];
                arr[i] = 	arr[j];
                arr[j] = a;
            }
        }
    }
    return 1;
}



int main(int argc, char **argv)
{
	
	printf("argc is: %d\n", argc);

	int position = atoi(argv[1]);
	printf("position: %d\n", position);
	
	if( argc > 2){
		printf("Only one input please\n");
		return -1;
	}
	if( (position > CYLINDER_SIZE ) || (position < 0)   ){
		printf("make sure the positong is between 0 and 5000\n");
		return -1;
	}
	int i;

	generateRequest();
	printf("fcfs is:%d\n", firstComeFirstServe(request,atoi(argv[1])) );
	printf("sstf:%d\n", shortestSeekingTimeFirst(request,atoi(argv[1]) )) ;
	printf("Scan is: %d\n", scan(request, atoi(argv[1]) ) );
	printf("LOOK is: %d\n", look(request, atoi(argv[1]) ) );
	printf("CScan is: %d\n", cscan(request, atoi(argv[1]) ) );
	printf("C-LOOK is: %d\n", clook(request, atoi(argv[1]) ) );

}