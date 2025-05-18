#pragma once

#include <cstddef>
#include <thread>

namespace eden {

/// Abstract source of “how many threads should we use?”
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

} // namespace eden
