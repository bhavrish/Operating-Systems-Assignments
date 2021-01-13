#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define NUM_OF_CYLINDERS 10000
#define NUM_OF_REQUESTS 1000

int cylinderRequestsArr[NUM_OF_REQUESTS]; // array of cylinder requests
int dhStartPosition; // starting index

int compare(const void *x, const void *y) { // comparator function for quick sort
    int a = *((int*) x);
    int b = *((int*) y);

    return a>b ? 1 : -1;
}

int fcfs() {
	int totalHeadMovement = 0; // initialize totalHeadMovement to 0
    int currVal = cylinderRequestsArr[dhStartPosition]; // value at starting index

    // iterate from starting index to end of array
    for(int i=dhStartPosition; i<NUM_OF_REQUESTS; i++) {
        totalHeadMovement += abs(cylinderRequestsArr[i] - currVal); // add absolute value of difference between value at ith index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[i]; // update currVal to value at ith index
    }
    	
    // iterate from 0 to starting index
    for(int j=0; j<dhStartPosition; j++) {
        totalHeadMovement += abs(cylinderRequestsArr[j] - currVal); // add absolute value of difference between value at jth index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[j]; // update currVal to value at jth index
    }	
        
    return totalHeadMovement;
}

int sstf() {
	int totalHeadMovement = 0; // initialize totalHeadMovement to 0
    int currVal = cylinderRequestsArr[dhStartPosition]; // value at starting index
    int higherPtr = dhStartPosition+1; // higherPtr starts at 1 more than starting index
    int lowerPtr = dhStartPosition-1; // lowerPtr starts at 1 less than starting index

    // iterate 998 times since we are not counting the starting index
    for(int i=NUM_OF_REQUESTS-2; i >= 0; i--) {
        // if difference to lowerPtr from currVal <= difference to higherPtr from currVal
        if (abs(cylinderRequestsArr[lowerPtr] - currVal) <= abs(cylinderRequestsArr[higherPtr] - currVal)) {
            totalHeadMovement += abs(cylinderRequestsArr[lowerPtr] - currVal); // add absolute value of difference between value at lowerPtr index and currVal to totalHeadMovement
            currVal = cylinderRequestsArr[lowerPtr]; // update currVal to value at lowerPtr index
            lowerPtr--; // decrement lowerPtr
        }

        // if difference to higherPtr from currVal < difference to lowerPtr from currVal
        else {
            totalHeadMovement += abs(cylinderRequestsArr[higherPtr] - currVal); // add absolute value of difference between value at higherPtr index and currVal to totalHeadMovement
            currVal = cylinderRequestsArr[higherPtr]; // update currVal to value at higherPtr index
            higherPtr++; // increment lowerPtr
        }
    }

    return totalHeadMovement;
}

int scan() {
	int totalHeadMovement = 0; // initialize totalHeadMovement to 0
    int currVal = cylinderRequestsArr[dhStartPosition]; // value at starting index

    // iterate from 1 below the starting position to 0th element in sorted array
    for(int i=dhStartPosition-1; i>=0; i--) {
        totalHeadMovement += abs(cylinderRequestsArr[i] - currVal); // add absolute value of difference between value at ith index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[i]; // update currVal to value at ith index
    }

    totalHeadMovement += currVal; // add difference from currVal to left end of disk (0) to currVal
    currVal = 0; // set currVal equal to left end of disk

    // iterate from 1 above the starting position to the end of sorted array
    for(int j=dhStartPosition+1; j<NUM_OF_REQUESTS; j++) {
        totalHeadMovement += abs(cylinderRequestsArr[j] - currVal); // add absolute value of difference between value at jth index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[j]; // update currVal to value at jth index
    }

    return totalHeadMovement;
}

int cscan() {
	int totalHeadMovement = 0; // initialize totalHeadMovement to 0
    int currVal = cylinderRequestsArr[dhStartPosition]; // value at starting index

    // iterate from 1 above the starting position to the end of sorted array
    for(int i=dhStartPosition+1; i<NUM_OF_REQUESTS; i++) {
        totalHeadMovement += abs(cylinderRequestsArr[i] - currVal); // add absolute value of difference between value at ith index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[i]; // update currVal to value at ith index
    }

    totalHeadMovement += (9999 - currVal + 9999); // add difference from right end of disk (9999) to currVal as well as the distance it takes to get to left end of disk (9999)
	currVal = 0; // set currVal equal to left end of disk
    
    // iterate from 0th element in sorted array to starting position
    for(int j=0; j<dhStartPosition; j++) {
        totalHeadMovement += abs(cylinderRequestsArr[j] - currVal); // add absolute value of difference between value at jth index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[j]; // update currVal to value at jth index
    }
    
    return totalHeadMovement;
}

int look() {
	int totalHeadMovement = 0; // initialize totalHeadMovement to 0
    int currVal = cylinderRequestsArr[dhStartPosition]; // value at starting index

    // iterate from 1 above the starting position to the end of sorted array
    for(int i=dhStartPosition+1; i<NUM_OF_REQUESTS; i++) {
        totalHeadMovement += abs(cylinderRequestsArr[i] - currVal); // add absolute value of difference between value at ith index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[i]; // update currVal to value at ith index
    }
    
    // iterate from 1 below the starting position to 0th element in sorted array
    for(int j=dhStartPosition-1; j>=0; j--) {
        totalHeadMovement += abs(cylinderRequestsArr[j] - currVal); // add absolute value of difference between value at jth index and currVal to totalHeadMovement
        currVal = cylinderRequestsArr[j]; // update currVal to value at jth index
    }
       
    return totalHeadMovement;
}

int main(int argc, char *argv[]) {
    if (argc != 2) { // need 2nd argument to be the starting position
        printf("Please enter the starting position.\n");
        return 1;
    }

    dhStartPosition = atoi(argv[1]); // convert starting position as string to integer
    
    for(int i=0; i<NUM_OF_REQUESTS; i++) // run loop 1000 times
        cylinderRequestsArr[i] = rand()%NUM_OF_CYLINDERS; // fill cylinderRequestsArr with random numbers from 0 to 9999

    printf("Algorithm | Head Movement\n");
    printf("-------------------------");
    printf("\nFCFS      | %d", fcfs());
    
    // sort cylinderRequestsArr since sorted array is needed for the following algorithms
    qsort(cylinderRequestsArr, sizeof(cylinderRequestsArr)/sizeof(*cylinderRequestsArr), sizeof(*cylinderRequestsArr), compare);

	printf("\nSSTF      | %d", sstf());
	printf("\nSCAN      | %d", scan());
	printf("\nC-SCAN    | %d", cscan());
	printf("\nLOOK      | %d\n", look());

    return 0;
}