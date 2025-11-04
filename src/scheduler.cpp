#include "scheduler.h"
#include <algorithm>
#include <iostream>

Scheduler::Scheduler(): next_pid(1), memmgr(1024) {}

int Scheduler::create_process(const std::string &name, int priority, int memory, int burst) {
    int pid = next_pid++;
    int start = memmgr.allocate(pid, memory);
    if (start < 0) {
        std::cout << "Memory allocation failed for process " << name << " (" << memory << " MB)\n";
        return -1;
    }
    auto p = std::make_shared<Process>(pid, name, priority, memory, burst);
    procs.push_back(p);
    std::cout << "Created PID=" << pid << " name=" << name << " mem=" << memory << "MB burst=" << burst << "\n";
    return pid;
}

bool Scheduler::terminate_process(int pid) {
    auto it = std::find_if(procs.begin(), procs.end(), [&](auto &p){ return p->pid==pid; });
    if (it==procs.end()) return false;
    (*it)->state = ProcState::Terminated;
    memmgr.deallocate(pid);
    std::cout << "Terminated PID=" << pid << "\n";
    return true;
}

std::vector<std::shared_ptr<Process>> Scheduler::list_processes() const {
    return procs;
}

void Scheduler::run_fcfs() {
    for (auto &p: procs) {
        if (p->state==ProcState::Ready) {
            p->state = ProcState::Running;
            // simulate running to completion instantly for demo
            p->remaining = 0;
            p->state = ProcState::Terminated;
            memmgr.deallocate(p->pid);
        }
    }
}

void Scheduler::run_rr(int quantum) {
    bool any = true;
    while (any) {
        any = false;
        for (auto &p : procs) {
            if (p->state == ProcState::Ready) {
                any = true;
                p->state = ProcState::Running;
                int run = std::min(quantum, p->remaining);
                p->remaining -= run;
                if (p->remaining == 0) {
                    p->state = ProcState::Terminated;
                    memmgr.deallocate(p->pid);
                } else {
                    p->state = ProcState::Ready;
                }
            }
        }
    }
}

MemoryManager& Scheduler::memory_manager() { return memmgr; }
