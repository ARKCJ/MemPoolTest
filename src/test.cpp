#define DEBUG 1
#include "ManageChunk.h"
#include "MemPool.h"
#include "Mutex.h"

int main(){
    auto& memPool = test::AllocatorFactory<1000>::GetMemPool();
    std::vector<std::vector<void*>> vec;
    for(size_t i = 0; i < 200;++i){
        vec.emplace_back();
        for(size_t j = 0; j < 10000; ++j){
            vec[i].push_back(memPool.allocate(i * 4));
        }
    }
    size_t m = 0;
    for(size_t i = 0; i < 200;++i){
        m = std::max(m, i);
        for(size_t j = 0; j < 10000; ++j){
            memPool.deallocate(vec[i][j], i * 4);
        }
    }
}