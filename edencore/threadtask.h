#pragma once

#include <string>
#include <thread>
#include <chrono>
#include <semaphore>

namespace eden {

class ThreadSignal;

class ThreadTask {
public:
    ThreadTask(const std::string& name);
    ThreadTask(const std::string& name, ThreadSignal* sig);
    virtual ~ThreadTask();

    const std::string& name() const { return name_; }

    void join();

    void execute(void (*func)(const std::string&, ThreadSignal*));

private:
    std::string     name_;
    // Thread of execution
    std::thread*    thread_;
    // Signal used for synchronization (if used)
    ThreadSignal*   sig_;
};

} // namespace eden