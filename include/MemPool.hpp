#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <cstddef>

class MemPool
{
public:
    MemPool(int num_blocks, int block_size);
    ~MemPool();
    void* Alloc();
    bool Free(void* ptr);

private:
    int num_blocks_{};
    int block_size_{};
    std::deque<int> free_list_{};
    std::vector<std::byte> mem_pool_;
};

inline MemPool::MemPool(int num_blocks, int block_size): num_blocks_{num_blocks},
                                                  block_size_{block_size},
                                             mem_pool_(num_blocks*block_size)
{
    for(int i{}; i < num_blocks_; ++i){
        free_list_.push_back(i);
    }
}

inline MemPool::~MemPool()
{
}

inline void* MemPool::Alloc(){
    if(free_list_.empty()){
        return nullptr;
    }
    
    int block_id{free_list_.back()};
    free_list_.pop_back();

    return &mem_pool_[block_id*block_size_];
}

inline bool MemPool::Free(void* ptr){
    if(ptr == nullptr){
        return false;
    }
    
    uintptr_t offset{static_cast<std::byte*>(ptr) - mem_pool_.data()};
    
    if(offset % block_size_ != 0){
        return false;
    }
    
    int block_id{offset/block_size_};

    if(block_id >=num_blocks_){
        return false;
    }
    
    free_list_.push_back(block_id);
    return true;
}

