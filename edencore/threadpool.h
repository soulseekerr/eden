#pragma once

#include "executor.h"
#include "concurrency.h"
#include <stop_token>
#include <semaphore>
#include <atomic>
#include <functional>
#include <thread>
#include <vector>

namespace eden {

class ThreadPool : public IExecutor {
public:
    /// Build with explicit thread-count
    explicit ThreadPool(std::size_t nThreads);

    /// Build by querying a provider
    explicit ThreadPool(const IThreadCountProvider& prov)
      : ThreadPool(prov.getThreadCount()) {}

    ~ThreadPool() override;  // wakes & joins all threads

    /// Schedule a job for execution
    void enqueue(std::function<void()> job) override;

private:
    void workerLoop(std::stop_token st);

    struct Node {
        std::function<void()> job;
        Node*                 next;
    };

    std::atomic<Node*>                head_{nullptr};
    std::counting_semaphore<INT_MAX>  sem_{0};
    std::vector<std::jthread>         workers_;
};

} // namespace eden

