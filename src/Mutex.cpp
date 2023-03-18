#pragma once
#include "Mutex.h"
#include <stdexcept>

namespace test{

    Mutex::Mutex()
    {
        if(pthread_mutex_init(&m_Mutex, nullptr)){
            throw std::runtime_error("mutex create error!");
        }
    }

    void Mutex::Lock()
    {
        if(pthread_mutex_lock(&m_Mutex)){
            throw std::runtime_error("mutex lock error!");
        }
    }
    void Mutex::UnLock()
    {
        if(pthread_mutex_unlock(&m_Mutex)){
            throw std::runtime_error("mutex unlock error!");
        }
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy(&m_Mutex);
    }
}