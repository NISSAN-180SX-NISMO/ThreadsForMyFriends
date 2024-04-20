#include "lab3.h"

//
// lab3 code should be located here!
//


unsigned int lab3_thread_graph_id() {
    return 5;
}

const char *lab3_unsynchronized_threads() {
    return "cdfg";
}

const char *lab3_sequential_threads() {
    return "ikm";
}

HANDLE tryToCreateThread(DWORD (*routine)(LPVOID), LPVOID attrs) {
    HANDLE hThread;
    DWORD IDThread;
    hThread = CreateThread(nullptr, 0, routine, attrs, 0, &IDThread);
    if (hThread == nullptr) {
        std::cout << "CreateThread error: " << GetLastError() << std::endl;
        exit(static_cast<uint32_t>(GetLastError()));
    }
    return hThread;
}

HANDLE tryToMutexInit(bool initiallyOwned = FALSE) {
    HANDLE mutex = CreateMutex(
            nullptr, // default security attributes // аттрибуты безопасности по умолчанию
            initiallyOwned,   // initially not owned          // начальное значение владения мьютексом
            nullptr); // unnamed mutex               // безымянный мьютекс

    if (mutex == nullptr) {
        printf("CreateMutex error: %lu\n", GetLastError());
        exit(1);
    }
    return mutex;
}

HANDLE tryToSemInit(int initialCount, int maxCount) {
    HANDLE sem = CreateSemaphore(
            nullptr,       // default security attributes // аттрибуты безопасности по умолчанию
            initialCount,  // initial count               // начальное значение счетчика
            maxCount,      // maximum count               // максимаьлное значение счетчика
            nullptr);      // unnamed semaphore           // безымянный семафор

    if (sem == nullptr) {
        printf("CreateSemaphore error: %lu\n", GetLastError());
        exit(1);
    }
    return sem;
}

void tryToMutexLock(HANDLE mutex) {
    DWORD wait_result = WaitForSingleObject(mutex, INFINITE);
    if (wait_result != WAIT_OBJECT_0) {
        // Обработка ошибки
        printf("WaitForSingleObject error: %lu\n", GetLastError());
        exit(1);
    }
}

void tryToMutexUnlock(HANDLE mutex) {
    if (!ReleaseMutex(mutex)) {
        // Обработка ошибки
        printf("ReleaseMutex error: %lu\n", GetLastError());
        exit(1);
    }
}

void threadJoin(HANDLE threadHandle) {
    WaitForSingleObject(threadHandle, INFINITE);
    CloseHandle(threadHandle);
}

HANDLE lab3::tid[NUMBER_OF_THREADS];
HANDLE lab3::lock;
HANDLE lab3::semI;
HANDLE lab3::semK;
HANDLE lab3::semM;
HANDLE lab3::semN;
HANDLE lab3::semC;
HANDLE lab3::semD;
HANDLE lab3::semF;
HANDLE lab3::semG;


DWORD lab3::unsynchronized::thread_a(LPVOID) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "a" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }
    return 0;
}

DWORD lab3::unsynchronized::thread_b(LPVOID) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "b" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }
    return 0;
}

DWORD lab3::unsynchronized::thread_d(LPVOID) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        WaitForSingleObject(semD, INFINITE);
        tryToMutexLock(lab3::lock);
        std::cout << "d" << std::flush;
        tryToMutexUnlock(lab3::lock);
        ReleaseSemaphore(semF, 1, NULL);
        computation();
    }
    return 0;
}

DWORD lab3::unsynchronized::thread_e(LPVOID) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "e" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }
    return 0;
}

DWORD lab3::unsynchronized::thread_f(LPVOID) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        WaitForSingleObject(semF, INFINITE);
        tryToMutexLock(lab3::lock);
        std::cout << "f" << std::flush;
        tryToMutexUnlock(lab3::lock);
        ReleaseSemaphore(semG, 1, NULL);
        computation();
    }
    return 0;
}

DWORD lab3::unsynchronized::thread_h(LPVOID) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "h" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }
    return 0;
}

DWORD lab3::unsynchronized::thread_g(LPVOID) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "g" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }

    ReleaseSemaphore(semC, 1, NULL);

    for (int i = 0; i < THREAD_DURATION; ++i) {
        WaitForSingleObject(semG, INFINITE);
        tryToMutexLock(lab3::lock);
        std::cout << "g" << std::flush;
        tryToMutexUnlock(lab3::lock);
        ReleaseSemaphore(semC, 1, NULL);
        computation();
    }

    WaitForSingleObject(lab3::tid[C_THREAD], INFINITE);
    lab3::tid[H_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_h, nullptr);
    lab3::tid[K_THREAD] = tryToCreateThread(lab3::sequential::thread_k, nullptr);

    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "g" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }

    WaitForSingleObject(lab3::tid[H_THREAD], INFINITE);

    return 0;
}

DWORD lab3::unsynchronized::thread_c(LPVOID) {

    lab3::tid[A_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_a, nullptr);

    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "c" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }

    WaitForSingleObject(lab3::tid[A_THREAD], INFINITE);
    lab3::tid[B_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_b, nullptr);
    lab3::tid[E_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_e, nullptr);
    lab3::tid[G_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_g, nullptr);


    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "c" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }

    WaitForSingleObject(lab3::tid[B_THREAD], INFINITE);
    WaitForSingleObject(lab3::tid[E_THREAD], INFINITE);
    lab3::tid[F_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_f, nullptr);
    lab3::tid[D_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_d, nullptr);


    for (int i = 0; i < THREAD_DURATION; ++i) {
        WaitForSingleObject(semC, INFINITE);
        tryToMutexLock(lab3::lock);
        std::cout << "c" << std::flush;
        tryToMutexUnlock(lab3::lock);
        ReleaseSemaphore(semD, 1, NULL);
        computation();
    }

    WaitForSingleObject(lab3::tid[D_THREAD], INFINITE);
    WaitForSingleObject(lab3::tid[F_THREAD], INFINITE);

    return 0;
}


DWORD lab3::sequential::thread_i(LPVOID ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        WaitForSingleObject(semI, INFINITE);
        std::cout << "i" << std::flush;
        ReleaseSemaphore(semK, 1, NULL);
        computation();
    }
    return 0;
}

DWORD lab3::sequential::thread_k(LPVOID ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "k" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }

    WaitForSingleObject(lab3::tid[G_THREAD], INFINITE);
    lab3::tid[I_THREAD] = tryToCreateThread(lab3::sequential::thread_i, nullptr);
    lab3::tid[M_THREAD] = tryToCreateThread(lab3::sequential::thread_m, nullptr);

    for (int i = 0; i < THREAD_DURATION; ++i) {
        WaitForSingleObject(semK, INFINITE);
        std::cout << "k" << std::flush;
        ReleaseSemaphore(semM, 1, NULL);
        computation();
    }

    WaitForSingleObject(lab3::tid[I_THREAD], INFINITE);

    return 0;
}

DWORD lab3::sequential::thread_m(LPVOID ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        WaitForSingleObject(semM, INFINITE);
        std::cout << "m" << std::flush;
        ReleaseSemaphore(semI, 1, NULL);
        computation();
    }

    WaitForSingleObject(lab3::tid[K_THREAD], INFINITE);
    lab3::tid[N_THREAD] = tryToCreateThread(lab3::sequential::thread_n, nullptr);

    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "m" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }

    WaitForSingleObject(lab3::tid[N_THREAD], INFINITE);

    return 0;
}

DWORD lab3::sequential::thread_n(LPVOID ptr) {
    for (int i = 0; i < THREAD_DURATION; ++i) {
        tryToMutexLock(lab3::lock);
        std::cout << "n" << std::flush;
        tryToMutexUnlock(lab3::lock);
        computation();
    }
    return 0;
}


DWORD lab3::thread_main(LPVOID) {


    //std::cout << "acacaccbegccbbeeggcdfgccddffgghgkhgkhgkikmikmikmnmnmnm" << std::flush;

    lab3::tid[C_THREAD] = tryToCreateThread(lab3::unsynchronized::thread_c, nullptr);

    WaitForSingleObject(lab3::tid[A_THREAD], INFINITE);
    CloseHandle(lab3::tid[A_THREAD]);
    WaitForSingleObject(lab3::tid[B_THREAD], INFINITE);
    CloseHandle(lab3::tid[B_THREAD]);
    WaitForSingleObject(lab3::tid[E_THREAD], INFINITE);
    CloseHandle(lab3::tid[E_THREAD]);
    WaitForSingleObject(lab3::tid[C_THREAD], INFINITE);
    CloseHandle(lab3::tid[C_THREAD]);
    WaitForSingleObject(lab3::tid[D_THREAD], INFINITE);
    CloseHandle(lab3::tid[D_THREAD]);
    WaitForSingleObject(lab3::tid[F_THREAD], INFINITE);
    CloseHandle(lab3::tid[F_THREAD]);
    WaitForSingleObject(lab3::tid[G_THREAD], INFINITE);
    CloseHandle(lab3::tid[G_THREAD]);
    WaitForSingleObject(lab3::tid[H_THREAD], INFINITE);
    CloseHandle(lab3::tid[H_THREAD]);
    WaitForSingleObject(lab3::tid[I_THREAD], INFINITE);
    CloseHandle(lab3::tid[I_THREAD]);
    WaitForSingleObject(lab3::tid[K_THREAD], INFINITE);
    CloseHandle(lab3::tid[K_THREAD]);
    WaitForSingleObject(lab3::tid[M_THREAD], INFINITE);
    CloseHandle(lab3::tid[M_THREAD]);
    WaitForSingleObject(lab3::tid[N_THREAD], INFINITE);
    CloseHandle(lab3::tid[N_THREAD]);


    return 0;
}

int lab3_init() {

        // init mutex and semafores
        lab3::lock = tryToMutexInit();

        lab3::semI = tryToSemInit(1, 1);
        lab3::semK = tryToSemInit(0, 1);
        lab3::semM = tryToSemInit(0, 1);
        lab3::semN = tryToSemInit(0, 1);

        lab3::semC = tryToSemInit(0, 1);
        lab3::semD = tryToSemInit(0, 1);
        lab3::semF = tryToSemInit(0, 1);
        lab3::semG = tryToSemInit(0, 1);

        lab3::tid[MAIN_THREAD] = tryToCreateThread(lab3::thread_main, nullptr);

        // wait for main thread
        threadJoin(lab3::tid[MAIN_THREAD]);


        // close mutex and semafores
        CloseHandle(lab3::lock);

        CloseHandle(lab3::semI);
        CloseHandle(lab3::semK);
        CloseHandle(lab3::semM);
        CloseHandle(lab3::semN);

        CloseHandle(lab3::semC);
        CloseHandle(lab3::semD);
        CloseHandle(lab3::semF);
        CloseHandle(lab3::semG);

        // success
        std::cout << std::endl;

    return 0;
}
