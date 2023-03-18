#pragma once

#include <stdexcept>
#include "MemPool.h"


namespace test{
//
//  The implement of MemPool
//
    MemPool::MemPool(size_t num)
    :requiredChunkNum(num), m_MutexList(127)
    {
        for (int i = 0; i < 127; i++){
            m_MangeChunkList.emplace_back(8 + i * 4);
        }
    }

    void* MemPool::allocate(size_t size)
    {
        if(size == 0){
            return nullptr;
        }
        if(size < 8 || size > 512){
            return ::operator new(size);
        }
        m_MutexList[(size - 8) >> 2].Lock();
        void *tmp = m_MangeChunkList[(size - 8) >> 2].Allocate(requiredChunkNum);
        m_MutexList[(size - 8) >> 2].UnLock();
        return tmp;
    }

    void MemPool::deallocate(void* ptr, size_t size)
    {
        if(size < 8 || size > 512){
            ::operator delete(ptr);
            return;
        }
        m_MutexList[(size - 8) >> 2].Lock();
        m_MangeChunkList[(size - 8) >> 2].DeAllocate(ptr, requiredChunkNum);
        m_MutexList[(size - 8) >> 2].UnLock();
    }
}
