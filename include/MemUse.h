#pragma once
#include<stddef.h>

namespace test{
    struct MemUse
    {
        static void Allocate(size_t size){used += size;}

        static void DeAllocate(size_t size){used -= size;}

        static size_t GetUsed(){return used;}

        static size_t used;
    };
    size_t MemUse::used = 0;
    
}