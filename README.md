# **Implement a Memory Pool**
### **Difficulty:** Hard  
### **Tags:** `C++`, `Memory Management`, `Custom Allocator`, `Performance Optimization`

## **Problem Statement**
In high-performance applications such as trading systems and game engines, frequent dynamic memory allocation (`new` and `delete`) can introduce performance bottlenecks. To mitigate this, we implement a **Memory Pool** that efficiently manages memory allocation and deallocation.

You need to design a class `MemoryPool` that provides:
1. **Fast allocation and deallocation** of fixed-size objects.
2. **Efficient memory reuse** to avoid frequent system calls to `malloc` and `free`.
3. **Minimal fragmentation**, ensuring memory blocks are reused optimally.

---

## **Class API**
```cpp
class MemoryPool {
public:
    /**
     * Constructor: Initializes a memory pool with a fixed object size.
     * @param objectSize: The size of each object in bytes.
     * @param poolSize: The number of objects to preallocate in the pool.
     */
    MemoryPool(size_t objectSize, size_t poolSize);

    /**
     * Allocates a memory block from the pool.
     * @return Pointer to the allocated memory, or nullptr if the pool is exhausted.
     */
    void* allocate();

    /**
     * Deallocates a previously allocated block, returning it to the pool.
     * @param ptr: Pointer to the memory block to be freed.
     */
    void deallocate(void* ptr);

    /**
     * Destructor: Cleans up the memory pool.
     */
    ~MemoryPool();
};
```

---

## **Example Usage**
```cpp
#include <iostream>
#include "MemoryPool.h"

int main() {
    // Create a MemoryPool for objects of size 32 bytes, with 10 preallocated blocks
    MemoryPool pool(32, 10);

    // Allocate memory
    void* ptr1 = pool.allocate();
    void* ptr2 = pool.allocate();

    std::cout << "Allocated Memory: " << ptr1 << ", " << ptr2 << std::endl;

    // Deallocate memory
    pool.deallocate(ptr1);
    pool.deallocate(ptr2);

    return 0;
}
```

---

## **Constraints**
- `1 <= objectSize <= 1024` (Memory blocks should support various object sizes)
- `1 <= poolSize <= 10^6` (Should handle a large number of objects efficiently)
- **Memory reuse is required** (Calling `allocate()` repeatedly should not cause excessive heap allocations)

---

## **Hints**
1. Use a **free list** to track available memory blocks efficiently.
2. Preallocate a **large contiguous block** and manage allocations within it.
3. Optimize for **cache efficiency** to improve performance.
4. Ensure **thread safety** if used in a multithreaded environment.

---

## **Follow-Up Questions**
1. How would you modify this design to **support variable-sized allocations**?
2. Can you make this **thread-safe** for concurrent memory allocation?
3. How would you extend this design to handle **memory fragmentation** over time?
