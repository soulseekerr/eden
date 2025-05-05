#include "threadpool.h"
#include <cassert>
#include <iostream>

namespace eden {

ThreadPool::ThreadPool(std::size_t nThreads) {
    workers_.reserve(nThreads);
    for (std::size_t i = 0; i < nThreads; ++i) {
        // std::jthread will capture a stop_token for us
        workers_.emplace_back([this](std::stop_token st) {
            this->workerLoop(st);
        });
    }
}

ThreadPool::~ThreadPool() {
    // Unblock any workers still waiting
    std::cout << "ThreadPool shutting down...\n";
    for (std::size_t i = 0; i < workers_.size(); ++i) {
        sem_.release();
    }
    // ~std::jthread will request_stop() and then join()
}

void ThreadPool::enqueue(std::function<void()> job) {
    // Push onto lock-free stack
    Node* node = new Node{std::move(job), nullptr};
    Node* old = head_.load(std::memory_order_relaxed);
    do {
        node->next = old;
    } while (!head_.compare_exchange_weak(
        old, node,
        std::memory_order_release,
        std::memory_order_relaxed));

    // Signal one waiting worker
    sem_.release();
}

void ThreadPool::workerLoop(std::stop_token st) {
    while (true) {
        while (!sem_.try_acquire_for(std::chrono::milliseconds(50))) {
            if (st.stop_requested()) return;
        }

        Node* node = nullptr;
        do {
            node = head_.load(std::memory_order_acquire);
            if (!node) break;
        } while (!head_.compare_exchange_weak(
            node, node->next,
            std::memory_order_acq_rel,
            std::memory_order_acquire));

        if (node) {
            try {
                node->job();
            } catch (...) {
                // swallow
            }
            delete node;
        } else if (st.stop_requested()) {
            return;
        }
    }
}


// void ThreadPool::workerLoop(std::stop_token st) {
//     while (true) {
//         sem_.acquire();

//         // Always try to pop a job
//         Node* node = nullptr;
//         do {
//             node = head_.load(std::memory_order_acquire);
//             if (!node) break;
//         } while (!head_.compare_exchange_weak(
//             node, node->next,
//             std::memory_order_acq_rel,
//             std::memory_order_acquire));

//         if (node) {
//             try {
//                 node->job();
//             } catch (...) {
//                 // handle or log
//             }
//             delete node;
//         } else if (st.stop_requested()) {
//             // Stop only when there's no job to run AND stop was requested
//             return;
//         }
//     }
// }


// void ThreadPool::workerLoop(std::stop_token st) {
//     while (true) {
//         // Wait for either a new job or cancellation
//         sem_.acquire();

//         // If cancellation requested and no more work, exit
//         if (st.stop_requested() && head_.load(std::memory_order_acquire) == nullptr) {
//             return;
//         }

//         // Pop one job off the stack
//         Node* node = nullptr;
//         do {
//             node = head_.load(std::memory_order_acquire);
//             if (!node) break;
//         } while (!head_.compare_exchange_weak(
//             node, node->next,
//             std::memory_order_acq_rel,
//             std::memory_order_acquire));

//         if (node) {
//             // Execute and free
//             try {
//                 node->job();
//             } catch (...) {
//                 // swallow or log
//             }
//             delete node;
//         }
//     }
// }

} // namespace eden
