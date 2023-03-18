#pragma once

#include<vector>
#include<stddef.h>
#include"Mutex.h"
#include"ManageChunk.h"

namespace test{
    class MemPool{
    template<size_t N>
    friend class AllocatorFactory;
    private:
        std::vector<Mutex> m_MutexList;
        std::vector<ManageChunk> m_MangeChunkList;
        size_t requiredChunkNum;
        explicit MemPool(size_t num);
    public:
        void* allocate(size_t size);
        void deallocate(void* ptr, size_t size);
        MemPool(const MemPool&) = delete;
        void operator=(const MemPool&) = delete;
    };

    template<size_t N>
    class AllocatorFactory{
    public:
        static MemPool& GetMemPool()
        {
            static MemPool memPool(N);
            return memPool;
        }
    };
}