#pragma once

#include <string>
#include <thread>
#include <chrono>
#include <semaphore>
#include <memory>

namespace eden {

class ThreadSignal;

/**
 * @brief ThreadTask class
 * @details This class is used to create a thread task that can be executed
 *          in a separate thread. It uses std::jthread to manage the thread
 *          and std::semaphore to synchronize the execution of the task.
 */
class ThreadTask {
    private:
    std::string name_;
    // Thread of execution
    std::unique_ptr<std::jthread> thread_;
    // Signal used for synchronization (if used)
    ThreadSignal* sig_;

public:
    ThreadTask() = delete;
    explicit ThreadTask(const std::string& name, ThreadSignal* sig=nullptr);
    virtual ~ThreadTask() = default;

    const std::string& name() const { return name_; }

    // Function to execute the task
    void execute(void (*func)(const std::string&, ThreadSignal*));
};

} // namespace eden