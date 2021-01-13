#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>

#define NUM_THREADS_NORTH 1 // number of farmer threads from North Tunbridge
#define NUM_THREADS_SOUTH 3 // number of farmer threads from South Tunbridge
#define MIN(x, y) ((x<y) ? x : y)

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // initialize a mutex

int northFarmerNumber = 1; // curr thread number (North)
int southFarmerNumber = 1; // curr thread number (Sorth)


void *northToSouth(void *param) { // all northFarmers threads call this function
    pthread_mutex_lock(&mutex1); // lock the mutex so only 1 farmer can be on the bridge at a time
    
    printf("North Tunbridge %d farmer can cross the bridge\n", northFarmerNumber);
    printf("North Tunbridge %d is traveling on the bridge…\n", northFarmerNumber);
    sleep(rand() % 4); // sleep randomly either 0 sec, 1 sec, 2 sec, or 3 sec
    printf("North Tunbridge %d farmer has left the bridge\n", northFarmerNumber);
    northFarmerNumber++;

    pthread_mutex_unlock(&mutex1); // unlock the mutex

    pthread_exit(0);
}

void *southToNorth(void *param) { // all southFarmers threads call this function
    pthread_mutex_lock(&mutex1); // lock the mutex so only 1 farmer can be on the bridge at a time
    
    printf("South Tunbridge %d farmer can cross the bridge\n", southFarmerNumber);
    printf("South Tunbridge %d is traveling on the bridge…\n", southFarmerNumber);
    sleep(rand() % 4); // sleep randomly either 0 sec, 1 sec, 2 sec, or 3 sec
    printf("South Tunbridge %d farmer has left the bridge\n", southFarmerNumber);
    southFarmerNumber++;

    pthread_mutex_unlock(&mutex1); // unlock the mutex

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t northFarmers[NUM_THREADS_NORTH];
    pthread_t southFarmers[NUM_THREADS_SOUTH];

    // --------------- CREATE THREADS ---------------
    for(int i=0; i<MIN(NUM_THREADS_NORTH, NUM_THREADS_SOUTH); i++) {
        pthread_create(&northFarmers[i], NULL, northToSouth, (void*)NULL);
        pthread_create(&southFarmers[i], NULL, southToNorth, (void*)NULL);
    }

    if (NUM_THREADS_SOUTH < NUM_THREADS_NORTH) { // if excess North threads left over
        for(int i=NUM_THREADS_SOUTH; i<NUM_THREADS_NORTH; i++) {
            pthread_create(&northFarmers[i], NULL, northToSouth, (void*)NULL);
        }
    }
    else if (NUM_THREADS_NORTH < NUM_THREADS_SOUTH) { // if excess South threads left over
        for(int i=NUM_THREADS_NORTH; i<NUM_THREADS_SOUTH; i++) {
            pthread_create(&southFarmers[i], NULL, southToNorth, (void*)NULL);
        }
    }

    // --------------- JOIN THREADS ---------------
    for(int i=0; i<MIN(NUM_THREADS_NORTH, NUM_THREADS_SOUTH); i++) {
        pthread_join(northFarmers[i], NULL);
        pthread_join(southFarmers[i], NULL);
    }

    if (NUM_THREADS_SOUTH < NUM_THREADS_NORTH) { // if excess North threads left over
        for(int i=NUM_THREADS_SOUTH; i<NUM_THREADS_NORTH; i++) {
            pthread_join(northFarmers[i], NULL);
        }
    }
    else if (NUM_THREADS_NORTH < NUM_THREADS_SOUTH) { // if excess South threads left over
        for(int i=NUM_THREADS_NORTH; i<NUM_THREADS_SOUTH; i++) {
            pthread_join(southFarmers[i], NULL);
        }
    }

    return 0;
}