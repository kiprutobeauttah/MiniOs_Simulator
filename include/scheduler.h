#pragma once
#include "process.h"
#include <vector>
#include <memory>

class Scheduler {
public:
    Scheduler();
    int create_process(const std::string &name, int priority, int memory, int burst);
    bool terminate_process(int pid);
    std::vector<std::shared_ptr<Process>> list_processes() const;
    void run_fcfs(); // run one simulation of FCFS
    void run_rr(int quantum); // run round-robin with given quantum
    MemoryManager& memory_manager();
private:
    int next_pid;
    std::vector<std::shared_ptr<Process>> procs;
    MemoryManager memmgr;
};
