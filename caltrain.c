#include <pthread.h>
#include "caltrain.h"

void station_init(struct station *station)
{
	// FILL ME IN
    station->freeSeats = 0;
    station->waitingList = 0;
    pthread_mutex_init(&station->mutexTrain, NULL);
    pthread_cond_init(&station->trainArrival, NULL);
    pthread_cond_init(&station->trainDepature, NULL);
}

void station_load_train(struct station *station, int count) {
    pthread_mutex_lock(&station->mutexTrain);
        station->freeSeats = count;
        if(station->freeSeats != 0 && station->waitingList != 0) { // no free seats or waiting list is empty, return  
            pthread_cond_broadcast(&station->trainArrival);
            pthread_cond_wait(&station->trainDepature, &station->mutexTrain);
        }
        station->freeSeats = 0; 
    pthread_mutex_unlock(&station->mutexTrain);
}

void station_wait_for_train(struct station *station) {
    pthread_mutex_lock(&station->mutexTrain);
    station->waitingList++; // increment waiting list count
    while(station->freeSeats == 0) {// there is no free seats
        pthread_cond_wait(&station->trainArrival, &station->mutexTrain); // wait till a train is in the station
    }
}

void station_on_board(struct station *station) {
    station->waitingList--;
    station->freeSeats--;
    if(station->freeSeats == 0 || station->waitingList == 0) {
        pthread_cond_signal(&station->trainDepature);
    }
    pthread_mutex_unlock(&station->mutexTrain);
}