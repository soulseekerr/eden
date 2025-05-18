
#include "threadtask.h"
#include "threadsignal.h"
#include <iostream>

using namespace eden;

ThreadTask::ThreadTask(const std::string& name, ThreadSignal* sig) 
    : name_(name), thread_(nullptr), sig_(sig) {
    std::cout << "ThreadTask Ctor" << std::endl;
}

void ThreadTask::execute(void (*func)(const std::string&, ThreadSignal*)) {
    std::cout << "[" << name_ << "] jthread instance" << std::endl;
    thread_ = std::make_unique<std::jthread>(func, name_, sig_);   
}