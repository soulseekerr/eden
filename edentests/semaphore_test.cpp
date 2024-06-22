#include <gtest/gtest.h>
#include "threadsignal.h"
#include "threadtask.h"

void function_computation_heavy(const std::string& name, eden::ThreadSignal* s) {

    std::cout << "[" << name << "] Waiting for the signal." << std::endl; 
    s->acquire();

    std::cout << "[started] function_computation_heavy()." << std::endl;
    
    using namespace std::literals;
    std::this_thread::sleep_for(5s);

    std::cout << "[ended] function_computation_heavy()." << std::endl;

    std::cout << "[" << name << "] Releasing the signal." << std::endl; 
    s->release();
}

void invoke(
    const std::string& n, 
    eden::ThreadSignal* s, 
    std::function<void(const std::string&, eden::ThreadSignal*)> func) {

    return func(n, s);
}

// Demonstrate semaphore implementation.
TEST(SemaphoreTest, BasicAssertions) {

    auto totalTaskCount = 10;
    auto maxTaskCount = 3;

    eden::ThreadSignal 
        taskSync(maxTaskCount);

    std::vector<eden::ThreadTask> pool;

    for (auto i = 0; i < totalTaskCount; i++) {
        pool.push_back(eden::ThreadTask(std::format("Task {}", i), &taskSync));
    }

    for (auto i = 0; i < totalTaskCount; i++) {
        pool[i].execute(function_computation_heavy);
    }

    for (auto i = 0; i < totalTaskCount; i++) {
        pool[i].join();
    }

    // Testing invoke function.
    std::cout << "Invoking TaskTest" << std::endl;
    invoke("TaskTest", &taskSync, function_computation_heavy);
    std::cout << "Invoking TaskTest done" << std::endl;

}