#pragma once
#include <pthread.h>

namespace test{
    class Mutex{
    private:
        pthread_mutex_t m_Mutex;
    public:
        Mutex();
        void Lock();
        void UnLock();
        ~Mutex();
        Mutex(const Mutex&) = delete;
        Mutex& opearator(const Mutex&) = delete;
};
}