#pragma once
#include <vector>
#include <string>

struct MemBlock {
    int start; // in MB offset
    int size;  // in MB
    int pid;   // 0 if free
};

class MemoryManager {
public:
    MemoryManager(int totalMb);
    int allocate(int pid, int size); // returns start offset or -1
    void deallocate(int pid);
    std::vector<MemBlock> blocks() const;
    std::string dump() const;
private:
    int total;
    std::vector<MemBlock> mem;
};
