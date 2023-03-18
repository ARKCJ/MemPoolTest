#pragma once
#include<stddef.h>

namespace test
{
    class MemChunk{
    friend class ManageChunk;
    private:
        struct Chunk
        {
            Chunk *next;
        };
        Chunk *freeChunkPtr;
        Chunk *startChunkPtr;
        MemChunk *prev;
        MemChunk *next;
        size_t freeNum;
        size_t chunkSize;
        MemChunk(size_t blockSize, size_t size);
    public:
        void* Allocate();
        void DeAllocate(void* ptr);
        ~MemChunk();
    };


    class ManageChunk{
        friend class MemPool;
    private:
        size_t manageChunkSize;
        MemChunk* head;
        MemChunk* free;
    public: 
        explicit ManageChunk(size_t size);
        void* Allocate(size_t chunkNum);
        void DeAllocate(void* ptr, size_t chunkNum);
        ~ManageChunk();
    };

}
