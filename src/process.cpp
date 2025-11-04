#include "process.h"
Process::Process(int pid_, const std::string &name_, int priority_, int memory_, int burst_)
: pid(pid_), name(name_), priority(priority_), memory(memory_), burst(burst_), remaining(burst_), state(ProcState::Ready)
{}
