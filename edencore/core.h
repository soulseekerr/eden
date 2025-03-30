#pragma once

#include <string_view>
#include <atomic>
#include <type_traits>
#include <semaphore>
#include <cstddef>

// Caught with -Wdangling 
// warning: temporary whose address is used as value of local variable ...
// string_view foo(const string& s LIFEIME_BOUND) ...
#if defined(__clang__)
#define LIFETIME_BOUND [[clang::lifetimebound]]
#else
#define LIFETIME_BOUND
#endif

#ifndef _ALWAYS_INLINE_
#define _ALWAYS_INLINE_ inline
#endif

namespace eden {

/**
 * @brief A simple status enum
 * 
 */
enum class Status : int {
    OK = 1,
    ERROR = -1 
};

/**
 * @brief A simple response struct that contains a status and a message
 * 
 */
struct Response final {
    Status status;
    std::string_view msg;
    constexpr Response(const Status s, std::string_view m) noexcept : status(s), msg(m) {}
    ~Response() = default;
};

namespace resp {
	constexpr Response success{Status::OK, "Operation successful"};
	constexpr Response failure{Status::ERROR, "An error occurred"};
} // namespace resp

/**
 * @brief Atomic type that is always lock-free
 * This approach ensures safe concurrent access without using locks
 * while maintaining proper memory synchronization
 * 
 */
template <class T>
class SafeNumeric {
	std::atomic<T> value;

	static_assert(std::atomic<T>::is_always_lock_free);

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		// Ensures that all previous writes 
		// before calling store are visible to other threads
		// before the value is stored
		// Used when writing to an atomic variable to establish
		// synchronization with subsequent memory_order_acquire loads
		value.store(p_value, std::memory_order_release);
	}

	_ALWAYS_INLINE_ T get() const {
		// Ensures that all operations following this load
		// are executed after reading the value
		// Used when reading from an atomic variable to synchronize
		// with previous memory_order_release stores
		return value.load(std::memory_order_acquire);
	}

	_ALWAYS_INLINE_ T increment() {
		// Combines the effects of memory_order_acquire and memory_order_release
		// Ensures that all reads/writes are visible before modifying the value,
		// and modificaitons are visible to subsequent reads
		return value.fetch_add(1, std::memory_order_acq_rel) + 1;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postincrement() {
		return value.fetch_add(1, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T decrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel) - 1;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postdecrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T add(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel) + p_value;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postadd(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T sub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel) - p_value;
	}

	_ALWAYS_INLINE_ T bit_or(T p_value) {
		return value.fetch_or(p_value, std::memory_order_acq_rel);
	}
	_ALWAYS_INLINE_ T bit_and(T p_value) {
		return value.fetch_and(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T bit_xor(T p_value) {
		return value.fetch_xor(p_value, std::memory_order_acq_rel);
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postsub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ explicit SafeNumeric<T>(T p_value = static_cast<T>(0)) {
		set(p_value);
	}
};

class SafeFlag {
	std::atomic_bool flag;

	static_assert(std::atomic_bool::is_always_lock_free, "Atomic bool must be lock-free");

public:
	explicit SafeFlag(bool p_value = false) : flag(p_value) {}

	_ALWAYS_INLINE_ bool is_set() const {
		return flag.load(std::memory_order_acquire);
	}

	_ALWAYS_INLINE_ void set() {
		flag.store(true, std::memory_order_release);
	}

	_ALWAYS_INLINE_ void clear() {
		flag.store(false, std::memory_order_release);
	}

	_ALWAYS_INLINE_ void set_to(bool p_value) {
		flag.store(p_value, std::memory_order_release);
	}
};

} // namespace eden