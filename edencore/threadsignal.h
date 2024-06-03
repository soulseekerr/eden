#pragma once

#include <thread>
#include <chrono>
#include <semaphore>

namespace eden {

class ThreadSignal {

public:
    ThreadSignal(const int token) : token_(token) {
        sem_ = new std::counting_semaphore<10>(token_);
    }

    ~ThreadSignal() {
        delete sem_;
    }

    void release() { sem_->release(); }
    void acquire() { sem_->acquire(); }

private:
    int token_;
    std::counting_semaphore<10>* sem_;
};

} // namespace eden