#include <gtest/gtest.h>
#include "threadsignal.h"
#include "threadtask.h"

void function_computation_heavy(const std::string& name, eden::ThreadSignal* s) {

    std::cout << "[" << name << "] Waiting for the signal." << std::endl; 
    s->acquire();

    std::cout << "[started] [" << name << "] function_computation_heavy()." << std::endl;
    
    using namespace std::literals;
    std::this_thread::sleep_for(2s);

    std::cout << "[ended] [" << name << "] function_computation_heavy(). Releasing." << std::endl;
    s->release();
}

void invoke(
    const std::string& n, 
    eden::ThreadSignal* s, 
    std::function<void(const std::string&, eden::ThreadSignal*)> func) {

    return func(n, s);
}

// struct TheTask {
//     std::unique_ptr<std::jthread> the_thread;

//     explicit TheTask() {
//         the_thread = std::make_unique<std::jthread>([this]() {
//             std::cout << "[started] Thread 1" << std::endl;
//             std::this_thread::sleep_for(std::chrono::seconds(2));
//             std::cout << "[ended] Thread 1" << std::endl;
//         });
//     }
// };

// Demonstrate semaphore implementation.
// TEST(SemaphoreTest2, BasicAssertions) {

//     constexpr auto totalTaskCount = 3;
//     std::counting_semaphore<totalTaskCount> semaphore(1);

//     TheTask task1(semaphore);
//     TheTask task2(semaphore);
// }

// Demonstrate semaphore implementation.
TEST(SemaphoreTest, BasicAssertions) {

    constexpr auto totalTaskCount = 3;
    std::counting_semaphore<totalTaskCount> semaphore(1);

    std::jthread th1([&semaphore]() {
        std::cout << "[started] Thread 1" << std::endl;
        semaphore.acquire();
        
        auto start = std::chrono::high_resolution_clock::now();

        using namespace std::literals;
        std::this_thread::sleep_for(2s);
        
        auto end = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Elapsed Time Thread 1: " << dt << " ms\n";
        
        std::cout << "[ended] Thread 1" << std::endl;
        semaphore.release();
    });

    std::jthread th2([&semaphore]() {
        std::cout << "[started] Thread 2" << std::endl;
        semaphore.acquire();
        
        auto start = std::chrono::high_resolution_clock::now();

        using namespace std::literals;
        std::this_thread::sleep_for(4s);
        
        auto end = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Elapsed Time Thread 2: " << dt << " ms\n";
        
        std::cout << "[ended] Thread 2" << std::endl;
        semaphore.release();
    });

    std::jthread th3([&semaphore]() {
        std::cout << "[started] Thread 3" << std::endl;
        semaphore.acquire();

        auto start = std::chrono::high_resolution_clock::now();

        using namespace std::literals;
        std::this_thread::sleep_for(1s);
        
        auto end = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "Elapsed Time Thread 3: " << dt << " ms\n";

        std::cout << "[ended] Thread 3" << std::endl;
        semaphore.release();
    });

    // for (auto i = 0; i < totalTaskCount; i++) {
    //     pool.push_back(std::make_shared<eden::ThreadTask>(std::format("TASK_{}", i), &taskSync));
    // }

    // for (auto& t : pool) {
    //     t->execute(function_computation_heavy);
    // }

    // Testing invoke function.
    // std::cout << "Invoking TaskTest" << std::endl;
    // invoke("TaskTest", &taskSync, function_computation_heavy);
    // std::cout << "Invoking TaskTest done" << std::endl;

}