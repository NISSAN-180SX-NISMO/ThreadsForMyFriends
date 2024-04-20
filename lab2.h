//
// lab2 definitions header file
//

#ifndef LAB2_LAB2_H
#define LAB2_LAB2_H

// global includes
#include <iostream>
#include <cstring>
#include <semaphore.h>
#include <random>


const int sleep_time = 1;

void sleep_ms(int milliseconds); // cross-platform sleep function

void computation();

int lab2_init();

unsigned int lab2_thread_graph_id();

const char* lab2_unsynchronized_threads();

const char* lab2_sequential_threads();

namespace lab2 {
#define NUMBER_OF_THREADS 11
#define MAIN_THREAD 0
#define A_THREAD 1
#define B_THREAD 2
#define C_THREAD 3
#define D_THREAD 4
#define E_THREAD 5
#define F_THREAD 6
#define G_THREAD 7
#define H_THREAD 8
#define I_THREAD 9
#define K_THREAD 10
#define THREAD_DURATION 4


    extern pthread_t tid[NUMBER_OF_THREADS];
    extern pthread_mutex_t lock;
    extern sem_t semF, semH, semI, semG, semK;;
    extern int err;

    void *thread_main(void *ptr);

    namespace unsynchronized {
        struct ThreadArgs {
            char thread_msg;
            int thread_duration;
        };
        void *thread(void *ptr);
        void *thread_a(void *ptr);
        void *thread_b(void *ptr);
        void *thread_c(void *ptr);
        void *thread_d(void *ptr);
        void *thread_e(void *ptr);

    }

    namespace sequential {
        void *thread_f(void *ptr);
        void *thread_h(void *ptr);
        void *thread_i(void *ptr);
        void *thread_g(void *ptr);
        void *thread_k(void *ptr);
    }


}


#endif //LAB2_LAB2_H
