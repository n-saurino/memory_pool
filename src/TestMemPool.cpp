#include <gtest/gtest.h>
#include "MemPool.hpp" // Adjust the include path as necessary

// Test Fixture
class MemoryPoolTest : public ::testing::Test {
protected:
    size_t numBlocks = 5;
    size_t blockSize = 16;
    MemPool pool;

    MemoryPoolTest() : pool(numBlocks, blockSize) {}
};

// Test case for successful allocation
TEST_F(MemoryPoolTest, AllocateSuccess) {
    for (size_t i = 0; i < numBlocks; ++i) {
        void* ptr = pool.Alloc();
        EXPECT_NE(ptr, nullptr); // Pointer should not be null
    }
}

// Test case for allocation failure when no blocks are free
TEST_F(MemoryPoolTest, AllocateFailureWhenFull) {
    for (size_t i = 0; i < numBlocks; ++i) {
        pool.Alloc();
    }
    void* ptr = pool.Alloc();
    EXPECT_EQ(ptr, nullptr); // Should return null when no blocks are free
}

// Test case for successful deallocation
TEST_F(MemoryPoolTest, DeAllocateSuccess) {
    void* ptr = pool.Alloc();
    ASSERT_NE(ptr, nullptr); // Ensure allocation was successful

    bool deAllocated = pool.Free(ptr);
    EXPECT_EQ(deAllocated, true); // Deallocation should succeed
}

// Test case for reallocation after deallocation
TEST_F(MemoryPoolTest, ReAllocateAfterDeAllocate) {
    void* ptr1 = pool.Alloc();
    ASSERT_NE(ptr1, nullptr); // Ensure allocation was successful

    pool.Free(ptr1);

    void* ptr2 = pool.Alloc();
    EXPECT_EQ(ptr1, ptr2); // Pointer should be reused
}

// Test case for deallocating null pointer
TEST_F(MemoryPoolTest, DeAllocateNullPointer) {
    bool result = pool.Free(nullptr);
    EXPECT_EQ(result, false); // Should return false for null pointer
}

// Test case for deallocating misaligned pointer
TEST_F(MemoryPoolTest, DeAllocateMisalignedPointer) {
    void* ptr = pool.Alloc();
    ASSERT_NE(ptr, nullptr); // Ensure allocation was successful

    void* misalignedPtr = static_cast<std::byte*>(ptr) + 1; // Create misaligned pointer
    bool result = pool.Free(misalignedPtr);
    EXPECT_EQ(result, false); // Should return false for misaligned pointer
}

// Test case for deallocating pointer outside the pool
TEST_F(MemoryPoolTest, DeAllocatePointerOutsidePool) {
    std::byte externalMemory[16];
    bool result = pool.Free(externalMemory);
    EXPECT_EQ(result, false); // Should return false for pointer not in the pool
}

// Test case for multiple allocations and deallocations
TEST_F(MemoryPoolTest, MultipleAllocationsAndDeallocations) {
    void* ptr1 = pool.Alloc();
    void* ptr2 = pool.Alloc();
    ASSERT_NE(ptr1, nullptr);
    ASSERT_NE(ptr2, nullptr);

    pool.Free(ptr1);
    pool.Free(ptr2);

    void* ptr3 = pool.Alloc();
    void* ptr4 = pool.Alloc();

    EXPECT_NE(ptr3, nullptr);
    EXPECT_NE(ptr4, nullptr);
}

// Test case for pointer reuse order
TEST_F(MemoryPoolTest, PointerReuseOrder) {
    void* ptr1 = pool.Alloc();
    void* ptr2 = pool.Alloc();
    ASSERT_NE(ptr1, nullptr);
    ASSERT_NE(ptr2, nullptr);

    pool.Free(ptr2);
    pool.Free(ptr1);

    void* ptr3 = pool.Alloc();
    EXPECT_EQ(ptr3, ptr1); // Last deallocated block reused first
}

// Test case for memory alignment
TEST_F(MemoryPoolTest, AllocationAlignment) {
    for (size_t i = 0; i < numBlocks; ++i) {
        void* ptr = pool.Alloc();
        ASSERT_NE(ptr, nullptr);

        uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
        EXPECT_EQ(address % blockSize, 0); // Address should be aligned to blockSize
    }
}