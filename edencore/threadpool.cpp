#include "threadpool.h"
#include <cassert>
#include <iostream>

namespace eden {

// Initialize the thread pool with worker threads
ThreadPool::ThreadPool(std::size_t nThreads) {
    workers_.reserve(nThreads);
    for (std::size_t i = 0; i < nThreads; ++i) {
        // std::jthread will capture a stop_token for us
        workers_.emplace_back([this](std::stop_token st) {
            this->workerLoop(st);
        });
    }
}

// Gracefully shut down the thread pool
ThreadPool::~ThreadPool() {
    // Unblock any workers still waiting
    // Release semaphore to unblock threads
    std::cout << "ThreadPool shutting down...\n";
    for (std::size_t i = 0; i < workers_.size(); ++i) {
        sem_.release();
    }
    // ~std::jthread will request_stop() and then join()
}

// A lock-free stack is a data structure that allows multiple threads 
// to push and pop items without blocking each other using mutexes. 
// It relies on atomic operations like compare_exchange_weak to safely manage shared state.

/*
Enqueue a job:
A new Node is allocated to hold the job.
We read the current head_ (top of the stack).
We try to atomically replace head_ with our new node, setting its next pointer to the old head_.
This repeats until the replacement succeeds — another thread might be pushing/popping at the same time.
This is called a CAS loop (Compare-And-Swap loop).
*/
void ThreadPool::enqueue(std::function<void()> job) {
    // Create a new job node
    Node* node = new Node{std::move(job), nullptr};
    // Lock-free push to the stack
    Node* old = head_.load(std::memory_order_relaxed);
    do {
        node->next = old;
    } while (!head_.compare_exchange_weak(
        old, node,
        // std::memory_order_release — ensures we publish the node to other threads safely.
        std::memory_order_release,
        std::memory_order_relaxed));

    // Notify one thread
    sem_.release();
}

// Worker thread logic
/*
Load the top of the stack.
If it's not nullptr, try to replace it with the next node in the chain.
Once successful, we return that node and execute its job.
If another thread pops first, we retry.
*/
void ThreadPool::workerLoop(std::stop_token st) {
    while (true) {
        // Try to acquire work, with a timeout
        while (!sem_.try_acquire_for(std::chrono::milliseconds(50))) {
            if (st.stop_requested()) return;
        }

        // Try to pop a job from the stack

        // Load the top of the stack.
        // If it's not nullptr, try to replace it with the next node in the chain.
        // Once successful, we return that node and execute its job.
        // If another thread pops first, we retry.
        Node* node = nullptr;
        do {
            // std::memory_order_acquire — ensures we read data safely after loading the pointer.
            node = head_.load(std::memory_order_acquire);
            if (!node) break;
        } while (!head_.compare_exchange_weak(
            node, node->next,
            // acq_rel (acquire + release) is used on the CAS to enforce both rules.
            std::memory_order_acq_rel,
            std::memory_order_acquire));

        // Run the job
        if (node) {
            try {
                node->job();
            } catch (...) {
                // Handle or log the exception
                std::cerr << "Exception in worker thread\n";
            }
            delete node;
        } else if (st.stop_requested()) {
            return;
        }
    }
}

} // namespace eden
