#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef struct {
    unsigned int k;
    int* res;
    pthread_mutex_t* mutex;

    const int score1;
    const int score2;
}Data;



void *simul_game(void* arg){
    printf("Thread ID: %lu\n", pthread_self());
    Data* data = (Data*)arg;

    int score1 = data->score1, score2 = data->score2;

    for(unsigned int i=0; i < data->k; ++i){
        score1 = rand() % 12 + 1;
        score2 = rand() % 12 + 1;
    }
    
    if (score1 > score2){
        pthread_mutex_lock(data->mutex);
        data->res[0]++;
        pthread_mutex_unlock(data->mutex);
    }
    else if (score2 > score1){
        pthread_mutex_lock(data->mutex);
        data->res[1]++;
        pthread_mutex_unlock(data->mutex);
    } else {
        pthread_mutex_lock(data->mutex);
        data->res[2]++;
        pthread_mutex_unlock(data->mutex);
    }
    



}



int main(int argc, char *argv[]){
    if (argc < 6){
        fprintf(stderr,"Usage %s <max threads> <number of thows> <score1> <score2> <number of experiments>", argv[0]);
        return 1;
    }



    unsigned int k = atoi(argv[2]);
    int score1 = atoi(argv[3]), score2 = atoi(argv[4]);
    unsigned int max_threads = atoi(argv[1]);

    unsigned int expr = atoi(argv[5]);

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int res[3] = {0, 0, 0};
    srand(time(NULL));

    pthread_t* threads = malloc(max_threads * sizeof(pthread_t));

    if (!threads){
        fprintf(stderr, "Failed to allocated memory");
        return 1;
    }

    Data data = {k, res, &mutex, score1, score2};

    int active_thrads = 0;
    
    clock_t start = clock();
    for (int i = 0; i < expr; ++i){

        if (active_thrads == max_threads) {
            pthread_join(threads[--active_thrads], NULL);
        }


        if(pthread_create(&threads[active_thrads++], NULL, simul_game, &data) != 0){
            fprintf(stderr, "Failed to create thread");
            free(threads);
            return 1;
        }
    }

    while (active_thrads > 0){
        pthread_join(threads[--active_thrads], NULL);
    }
    clock_t end = clock();
    double del_time = (double) end-start;
    printf("Player 1 wins: %d\n", res[0]);
    printf("Player 2 wins: %d\n", res[1]);
    printf("Draw: %d\n", res[2]);
    printf("Del time %f\n", del_time);

    free(threads);
    pthread_mutex_destroy(&mutex);


    return 0;
}