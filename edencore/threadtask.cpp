
#include "threadtask.h"
#include "threadsignal.h"
#include <iostream>

using namespace eden;

ThreadTask::ThreadTask(const std::string& name) 
    : name_(name), thread_(nullptr), sig_(nullptr) {

    std::cout << "Thread ctor" << std::endl;
}

ThreadTask::ThreadTask(const std::string& name, ThreadSignal* sig=nullptr) 
    : name_(name), thread_(nullptr), sig_(sig) {

    std::cout << "Thread ctor2" << std::endl;
}

ThreadTask::~ThreadTask() {

    std::cout << "Thread dtor" << std::endl;
    if (thread_ != nullptr) delete thread_;
}

void ThreadTask::join() {

    if (thread_ != nullptr) thread_->join();
}

void ThreadTask::execute(void (*func)(const std::string&, ThreadSignal*)) {

    std::cout << "[" << name_ << "] Thread execute" << std::endl;
    thread_ = new std::thread(func, name_, sig_);   
}