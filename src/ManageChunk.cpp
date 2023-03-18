#pragma once
#include <stddef.h>
#include "ManageChunk.h"
#include <malloc.h>
#include <new>
#include <stdexcept>
#ifdef DEBUG 
#if DEBUG == 1
#include <iostream>
#include "MemUse.h"
#endif
#endif

namespace test
{
//
//  The implement of class MemChunk
//
    MemChunk::MemChunk(size_t blockSize, size_t size)
    :freeNum(blockSize), chunkSize(size), freeChunkPtr(nullptr), 
    startChunkPtr(nullptr), prev(nullptr), next(nullptr)
    {
        while(true){
            startChunkPtr = reinterpret_cast<Chunk*>(malloc(freeNum * chunkSize));
            if(startChunkPtr == nullptr){
                throw std::bad_alloc();
            }else{
#ifdef DEBUG
#if DEBUG == 1
                std::cout << "Create a MemChunk -> size: " << chunkSize << " , num: " << freeNum << std::endl;
                MemUse::Allocate(freeNum * chunkSize);
                std::cout << "Used: " << MemUse::GetUsed() << std::endl;
#endif
#endif
                break;
            }
        }
        freeChunkPtr = startChunkPtr;
        Chunk* cur = startChunkPtr;
        for(size_t i = 0;i < freeNum - 1;++i, cur = cur -> next){
            cur -> next = reinterpret_cast<Chunk*>(
                reinterpret_cast<char*>(cur) + chunkSize
            );
        }
        cur -> next = nullptr;
    }


    void* MemChunk::Allocate()
    {
        if(freeNum <= 0){
            return nullptr;
        }
        --freeNum;
        void* tmp = freeChunkPtr;
        freeChunkPtr = freeChunkPtr -> next;
        return tmp;
    }

    void MemChunk::DeAllocate(void* ptr)
    {
        Chunk* tmp = reinterpret_cast<Chunk*>(ptr);
        tmp -> next = freeChunkPtr;
        freeChunkPtr = tmp;
        ++freeNum;
    }

    MemChunk::~MemChunk()
    {
#ifdef DEBUG
#if DEBUG == 1
        std::cout << "Destory a MemChunk -> size: " << chunkSize << " , num: " << freeNum << std::endl;
        MemUse::DeAllocate(freeNum * chunkSize);
        std::cout << "Used: " << MemUse::GetUsed() << std::endl;
#endif
#endif
        free((void*)startChunkPtr);

    }

//
//  The implement of class ManageChunk
//

    ManageChunk::ManageChunk(size_t size)
    :manageChunkSize(size), head(nullptr), free(nullptr){}

    void* ManageChunk::Allocate(size_t chunkNum)
    {
        if(head == nullptr){
            head = new MemChunk(chunkNum, manageChunkSize);
            return head -> Allocate();
        }
        MemChunk *cur = head;
        for(;cur != nullptr;cur = cur -> next){
            if(cur -> freeNum > 0){
                if(cur == free){
                    free = nullptr;
                }
                if(cur == head){
                    break;
                }else{
                    cur -> prev ->next = cur -> next;
                    if(cur -> next){
                        cur -> next -> prev = cur -> prev;
                    } 
                    cur -> next = head;
                    head -> prev = cur;
                    head = cur;
                    break;
                }
            }
        }
        if(cur == nullptr){
            cur = new MemChunk(chunkNum, manageChunkSize);
            cur -> next = head;
            head -> prev = cur;
            head = cur;
        }
        return head -> Allocate();
    }

    void ManageChunk::DeAllocate(void* ptr, size_t chunkNum)
    {
        if(ptr == nullptr){
            return;
        }
        MemChunk* cur = head;
        for(;cur != nullptr; cur = cur -> next){
            if(ptr >= reinterpret_cast<void*>(cur -> startChunkPtr) 
            && 
            ptr < reinterpret_cast<void*>(
                reinterpret_cast<char*>(cur -> startChunkPtr) + chunkNum * manageChunkSize))
            {
                cur -> DeAllocate(ptr);
                if(cur -> freeNum == chunkNum){
                    if(cur != head){
                        if(cur -> prev != nullptr){
                            cur -> prev -> next = cur -> next;
                        }
                        if(cur -> next != nullptr){
                            cur -> next -> prev = cur -> prev;
                        }
                        cur -> next = head;
                        head -> prev = cur;
                        head = cur;
                    }
                    if(free != nullptr){
                        if(free -> prev != nullptr){
                            free -> prev -> next = free -> next;
                        }
                        if(free -> next != nullptr){
                            free -> next -> prev = free -> prev;
                        }
                        delete free;
                    }
                    free = cur;
                }
                return;
            }
        }
        throw std::logic_error("memory release error!");
    }

    ManageChunk::~ManageChunk()
    {
        MemChunk* tmp = nullptr;
        while(head != nullptr){
            tmp = head -> next;
            delete head;
            head = tmp;
        }
    }

}