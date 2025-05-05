#pragma once

#include <functional>

namespace eden {

/// Abstract interface for any task executor (thread pool, event loop, etc.)
struct IExecutor {
  virtual ~IExecutor() = default;
  /// Schedule a void() job for execution
  virtual void enqueue(std::function<void()> job) = 0;
};

} // namespace eden
