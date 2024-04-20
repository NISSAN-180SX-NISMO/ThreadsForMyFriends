//
// lab3 definitions header file
//

#ifndef LAB3_LAB3_H
#define LAB3_LAB3_H

// global includes
#include <iostream>
#include <Windows.h>
#include <random>

const int sleep_time = 1;

void sleep_ms(int milliseconds); // cross-platform sleep function

void computation();

int lab3_init();

unsigned int lab3_thread_graph_id();

const char *lab3_unsynchronized_threads();

const char *lab3_sequential_threads();


namespace lab3 {
#define NUMBER_OF_THREADS 13
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
#define M_THREAD 11
#define N_THREAD 12
#define THREAD_DURATION 3


    extern HANDLE tid[];
    extern HANDLE lock, semI, semK, semM, semN, semC, semD, semF, semG;

    DWORD thread_main(LPVOID ptr);

    namespace unsynchronized {
        struct ThreadArgs {
            char thread_msg;
            int thread_duration;
        };

        DWORD thread(LPVOID ptr);

        DWORD thread_a(LPVOID ptr);

        DWORD thread_b(LPVOID ptr);

        DWORD thread_c(LPVOID ptr);

        DWORD thread_d(LPVOID ptr);

        DWORD thread_e(LPVOID ptr);

        DWORD thread_f(LPVOID ptr);

        DWORD thread_g(LPVOID ptr);

        DWORD thread_h(LPVOID ptr);
    }

    namespace sequential {
        DWORD thread_i(LPVOID ptr);

        DWORD thread_k(LPVOID ptr);

        DWORD thread_m(LPVOID ptr);

        DWORD thread_n(LPVOID ptr);
    }


}

#endif //LAB3_LAB3_H
