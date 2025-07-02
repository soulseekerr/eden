#pragma once

#include <stop_token>
#include <semaphore>
#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <cstddef>

namespace eden {

// Abstract interface for any task executor (thread pool, event loop, etc.)
struct IThreadExecutor {
    virtual ~IThreadExecutor() = default;
    /// Schedule a void() job for execution
    virtual void enqueue(std::function<void()> job) = 0;
};

// Abstract source of “how many threads should we use?”
struct IThreadCountProvider {
    virtual ~IThreadCountProvider() = default;
    virtual std::size_t getThreadCount() const noexcept = 0;
  };
  
  /// Default: use hardware_concurrency (fall back to 1)
  struct HardwareConcurrencyProvider : IThreadCountProvider {
    std::size_t getThreadCount() const noexcept override {
      auto n = std::thread::hardware_concurrency();
      return n == 0 ? 1 : n;
    }
};

// Thread pool implementation
class ThreadPool : public IThreadExecutor {
public:
    // Build with explicit thread-count
    explicit ThreadPool(std::size_t nThreads);

    // Build by querying a provider
    explicit ThreadPool(const IThreadCountProvider& prov)
      : ThreadPool(prov.getThreadCount()) {}

    virtual ~ThreadPool() override;  // wakes & joins all threads

    // Schedule a job for execution
    void enqueue(std::function<void()> job) override;

private:
    // Main loop for each thread
    void workerLoop(std::stop_token st);

    // Node structure for lock-free stack
    struct Node {
        std::function<void()> job;
        Node* next;
    };

    // Top of the lock-free stack
    std::atomic<Node*> head_{nullptr};
    // Semaphore to notify waiting threads
    std::counting_semaphore<INT_MAX> sem_{0};
    // Vector of threads
    std::vector<std::jthread> workers_;
};

} // namespace eden

