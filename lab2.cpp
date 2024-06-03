#include "lab2.h"


unsigned int lab2_thread_graph_id() {
    return 9;
}

const char *lab2_unsynchronized_threads() {
    return "bcd";
}

const char *lab2_sequential_threads() {
    return "fhi";
}

void tryToCreateThread(int thread_id, void *(*thread_func)(void *), void *args = NULL) {
    lab2::err = pthread_create(&lab2::tid[thread_id], NULL, thread_func, args);
    if (lab2::err != 0)
        std::cerr << "Can't create thread. Error: " << strerror(lab2::err) << std::endl;
}

void tryToMutexInit(pthread_mutex_t *mutex) {
    if (pthread_mutex_init(mutex, NULL) != 0) {
        std::cerr << "Mutex init failed" << std::endl;
        exit(1);
    }
}

void tryToSemInit(sem_t *semaphore, int initialValue) {
    if (sem_init(semaphore, 0, initialValue) != 0) {
        std::cerr << "Semaphore init failed" << std::endl;
        exit(1);
    }
}

pthread_t lab2::tid[NUMBER_OF_THREADS];
pthread_mutex_t lab2::lock;
sem_t lab2::semF, lab2::semH, lab2::semI, lab2::semG, lab2::semK;;
int lab2::err;

pthread_cond_t condA = PTHREAD_COND_INITIALIZER;
pthread_cond_t condB = PTHREAD_COND_INITIALIZER;
pthread_cond_t condC = PTHREAD_COND_INITIALIZER;
pthread_cond_t condD = PTHREAD_COND_INITIALIZER;


pthread_mutex_t mutexA;
pthread_mutex_t mutexB;
pthread_mutex_t mutexC;
pthread_mutex_t mutexD;


int current_thread = B_THREAD;

void *lab2::unsynchronized::thread_a(void *ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        pthread_mutex_lock(&mutexA);
        std::cout << "a" << std::flush;
        pthread_mutex_unlock(&mutexB);
        computation();
    }
    return ptr;

}

void *lab2::unsynchronized::thread_d(void *ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        pthread_mutex_lock(&mutexD);
        std::cout << "d" << std::flush;
        pthread_mutex_unlock(&mutexB);
        computation();
    }
    return ptr;
}

void *lab2::unsynchronized::thread_e(void *ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        pthread_mutex_lock(&lab2::lock);
        std::cout << "e" << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        computation();
    }
    return ptr;
}

void *lab2::unsynchronized::thread_b(void *ptr) {
    for (int i = 0; i < THREAD_DURATION * 2; ++i) {
        pthread_mutex_lock(&mutexB);
        std::cout << "b" << std::flush;
        pthread_mutex_unlock(&mutexC);
        computation();
    }
    return ptr;
}

void *lab2::unsynchronized::thread_c(void *ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        pthread_mutex_lock(&mutexC);
        std::cout << "c" << std::flush;
        pthread_mutex_unlock(&mutexA);
        computation();
    }

    pthread_join(lab2::tid[A_THREAD], NULL);
    tryToCreateThread(D_THREAD, lab2::unsynchronized::thread_d);

    for (int i = 0; i < THREAD_DURATION; ++i) {
        pthread_mutex_lock(&mutexC);
        std::cout << "c" << std::flush;
        pthread_mutex_unlock(&mutexD);
        computation();
    }

    pthread_join(lab2::tid[B_THREAD], NULL);
    pthread_join(lab2::tid[D_THREAD], NULL);
    tryToCreateThread(E_THREAD, lab2::unsynchronized::thread_e);

    for (int i = 0; i < THREAD_DURATION; ++i) {
        pthread_mutex_lock(&lab2::lock);
        std::cout << "c" << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        computation();
    }

    pthread_join(lab2::tid[E_THREAD], NULL);
    return ptr;
}


void *lab2::unsynchronized::thread(void *ptr) {
    auto attr = static_cast<ThreadArgs *>(ptr);
    int duration = attr->thread_duration;
    char msg = attr->thread_msg;
    for (int i = 0; i < duration; ++i) {
        pthread_mutex_lock(&lab2::lock);
        std::cout << msg << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        computation();
    }
    return ptr;
}

void *lab2::sequential::thread_f(void *ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        sem_wait(&lab2::semF);
        pthread_mutex_lock(&lab2::lock);
        std::cout << "f" << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        sem_post(&lab2::semH);
        computation();
    }

    return ptr;
}

void *lab2::sequential::thread_g(void *ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        sem_wait(&lab2::semG);
        pthread_mutex_lock(&lab2::lock);
        std::cout << "g" << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        computation();
        sem_post(&lab2::semH);
    }
    return ptr;
}

void *lab2::sequential::thread_h(void *ptr) {
    for (int i = 0; i < THREAD_DURATION * 2; ++i) { // run 'h' twice as long
        sem_wait(&lab2::semH);
        pthread_mutex_lock(&lab2::lock);
        std::cout << "h" << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        computation();
        sem_post(&lab2::semI);
    }
    return ptr;
}

void *lab2::sequential::thread_k(void *ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        sem_wait(&lab2::semK);
        pthread_mutex_lock(&lab2::lock);
        std::cout << "k" << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        computation();
        sem_post(&lab2::semI);
    }
    return ptr;
}

void *lab2::sequential::thread_i(void *ptr) {
    for (int i = 0; i < THREAD_DURATION * 3; ++i) { // run 'i' three times as long
        sem_wait(&lab2::semI);
        pthread_mutex_lock(&lab2::lock);
        std::cout << "i" << std::flush;
        pthread_mutex_unlock(&lab2::lock);
        computation();
        if (i < THREAD_DURATION - 1)
            sem_post(&lab2::semF);
        else if (i < (THREAD_DURATION * 2) - 1)
            sem_post(&lab2::semG);
        else
            sem_post(&lab2::semK);
    }
    return ptr;
}

void *lab2::thread_main(void *ptr) {

    tryToMutexInit(&mutexA);
    tryToMutexInit(&mutexB);
    tryToMutexInit(&mutexC);
    tryToMutexInit(&mutexD);

    pthread_mutex_lock(&mutexA);
    pthread_mutex_lock(&mutexC);
    pthread_mutex_lock(&mutexD);

    tryToCreateThread(B_THREAD, lab2::unsynchronized::thread_b);
    tryToCreateThread(C_THREAD, lab2::unsynchronized::thread_c);
    tryToCreateThread(A_THREAD, lab2::unsynchronized::thread_a);

    pthread_join(lab2::tid[C_THREAD], NULL);

    //std::cout << "bcabcabcabcdbcdbcdececec" << std::flush;

    tryToCreateThread(F_THREAD, lab2::sequential::thread_f);
    tryToCreateThread(H_THREAD, lab2::sequential::thread_h);
    tryToCreateThread(I_THREAD, lab2::sequential::thread_i);
    tryToCreateThread(G_THREAD, lab2::sequential::thread_g);
    tryToCreateThread(K_THREAD, lab2::sequential::thread_k);

    pthread_join(lab2::tid[F_THREAD], NULL);
    pthread_join(lab2::tid[H_THREAD], NULL);
    pthread_join(lab2::tid[I_THREAD], NULL);
    pthread_join(lab2::tid[G_THREAD], NULL);
    pthread_join(lab2::tid[K_THREAD], NULL);

    return ptr;
}


int lab2_init() {
    // initilize mutex
    tryToMutexInit(&lab2::lock);

    // initialize semaphores
    // THIS CODE WILL NOT RUN ON MacOS!
    // MacOS doesn't support unnamed semaphores, so named semaphores should be used instead
    tryToSemInit(&lab2::semF, 1);
    tryToSemInit(&lab2::semH, 0);
    tryToSemInit(&lab2::semI, 0);
    tryToSemInit(&lab2::semG, 0);
    tryToSemInit(&lab2::semK, 0);

    // start the main thread
    tryToCreateThread(MAIN_THREAD, lab2::thread_main);

    // ... and wait for it to finish
    pthread_join(lab2::tid[MAIN_THREAD], NULL);

    // free resources
    pthread_mutex_destroy(&lab2::lock);
    sem_destroy(&lab2::semF);
    sem_destroy(&lab2::semH);
    sem_destroy(&lab2::semI);
    sem_destroy(&lab2::semG);
    sem_destroy(&lab2::semK);

    std::cout << std::endl;
    // success
    return 0;
}
