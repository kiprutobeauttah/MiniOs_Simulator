#include "memory.h"
#include <sstream>

MemoryManager::MemoryManager(int totalMb): total(totalMb) {
    mem.push_back({0, totalMb, 0});
}

int MemoryManager::allocate(int pid, int size) {
    for (size_t i=0;i<mem.size();++i) {
        if (mem[i].pid==0 && mem[i].size >= size) {
            int start = mem[i].start;
            if (mem[i].size == size) {
                mem[i].pid = pid;
            } else {
                // split block
                MemBlock used = {mem[i].start, size, pid};
                mem[i].start += size;
                mem[i].size -= size;
                mem.insert(mem.begin()+i, used);
            }
            return start;
        }
    }
    return -1;
}

void MemoryManager::deallocate(int pid) {
    for (size_t i=0;i<mem.size();++i) {
        if (mem[i].pid == pid) {
            mem[i].pid = 0;
        }
    }
    // merge adjacent free blocks
    for (size_t i=0;i+1<mem.size();) {
        if (mem[i].pid==0 && mem[i+1].pid==0) {
            mem[i].size += mem[i+1].size;
            mem.erase(mem.begin()+i+1);
        } else ++i;
    }
}

std::vector<MemBlock> MemoryManager::blocks() const { return mem; }

std::string MemoryManager::dump() const {
    std::ostringstream oss;
    oss << "Memory map (start,size,pid):\n";
    for (auto &b: mem) oss << "[" << b.start << "," << b.size << "," << b.pid << "]\n";
    return oss.str();
}
