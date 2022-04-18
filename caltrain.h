#include <pthread.h>

struct station {
    int freeSeats;
    int waitingList;
    pthread_mutex_t mutexTrain;
    pthread_cond_t trainArrival;
    pthread_cond_t trainDepature;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);