#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>

#define NUM_THREADS 4 // maximum number of threads
#define UPPER 10000 // upper bound  

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // initialize a mutex

int currThread = 0; // curr thread number
int numWithMostDivisors = 1; // number that contains the maximum # of divisors
int maxDivisors = 1; // maximum # of divisors

void *numOfDivisors(void *param) { // all threads call this function
    pthread_mutex_lock(&mutex1); // lock the mutex so only 1 thread can modify value of currThread at a time
    int currThreadCopy = currThread++; // store a copy of currThread value in this thread in case it gets modified in another thread later
    pthread_mutex_unlock(&mutex1); // unlock the mutex

    int valuesPerThread = UPPER/NUM_THREADS; // if 1 thread -> 10k, if 2 threads -> 5k, if 4 threads -> 2.5k
    int startingValue = (currThreadCopy*valuesPerThread)+1;
    int endingValue = (currThreadCopy+1)*valuesPerThread;

    for (int num = startingValue; num <= endingValue; num++) {
        int divisors = 2; // 1 and itself

        for(int i=2; i<=sqrt(num); i++) { // iterate up to square root of the number
            if (num%i == 0 && i != sqrt(num)) { // if i != sqrt(num), there must be a complementary divisor, so increment by 2
                divisors+=2;
            }
            else if (num%i == 0) { // if i == sqrt(num), the complementary divisor is the same as the current divisor, so only increment by 1
                divisors++;
            }
        }

        if (maxDivisors < divisors) { // if maxDivisors < # of divisors for the current number, update maxDivisors and numWithMostDivisors
            maxDivisors = divisors;
            numWithMostDivisors = num;
        }
    } 

    printf("Thread range: %d - %d\n", startingValue, endingValue);

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    clock_t startTime = clock(); // start timer

    pthread_t threads[NUM_THREADS];


    for (int i = 0; i < NUM_THREADS; i++) // creating threads 
        pthread_create(&threads[i], NULL, numOfDivisors, (void*)NULL); 
  
    for (int i = 0; i < NUM_THREADS; i++) // joining threads i.e. waiting for all threads to complete 
        pthread_join(threads[i], NULL); 

    clock_t endTime = clock(); // end timer

    double totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
  
    printf("number of threads: %d\n", NUM_THREADS);
    printf("number= %d has %d divisors\n", numWithMostDivisors, maxDivisors);
    printf("Program took %f seconds\n", totalTime);
  
    return 0;
}