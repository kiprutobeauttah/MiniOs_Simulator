#pragma once
#include <string>

enum class ProcState { Ready, Running, Waiting, Terminated };

struct Process {
    int pid;
    std::string name;
    int priority;
    int memory; // MB
    int burst;  // simulated execution time (units)
    int remaining;
    ProcState state;
    Process(int pid_, const std::string &name_, int priority_, int memory_, int burst_);
};
