#ifndef GD_CLASSES_H
#define GD_CLASSES_H

#include <stddef.h>
#include <cstdint>
#include <stdarg.h>

// Turn argument to string constant:
// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing
#ifndef _STR
#define _STR(m_x) #m_x
#define _MKSTR(m_x) _STR(m_x)
#endif

// Should always inline no matter what.
#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline
#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

// Should always inline, except in dev builds because it makes debugging harder.
#ifndef _FORCE_INLINE_
#ifdef DEV_ENABLED
#define _FORCE_INLINE_ inline
#else
#define _FORCE_INLINE_ _ALWAYS_INLINE_
#endif
#endif

// No discard allows the compiler to flag warnings if we don't use the return value of functions / classes
#ifndef _NO_DISCARD_
#define _NO_DISCARD_ [[nodiscard]]
#endif

#undef ABS
#undef MIN
#undef MAX

template <typename T>
constexpr T ABS(T p_v) {
    return p_v < 0 ? -p_v : p_v;
}

template <typename T, typename T2>
constexpr auto MIN(const T p_a, const T2 p_b) {
    return p_a < p_b ? p_a : p_b;
}

template <typename T, typename T2>
constexpr auto MAX(const T p_a, const T2 p_b) {
    return p_a > p_b ? p_a : p_b;
}

// Generic Swap template
#ifndef SWAP
#define SWAP(p_x, p_y) __swap_tmpl((p_x), (p_y))
template <class T>
inline void __swap_tmpl(T& x, T& y) {
    T aux = x;
    x = y;
    y = aux;
}
#endif // SWAP

// Generic comparator used in Map, List, etc.
template <class T>
struct Comparator {
	_ALWAYS_INLINE_ bool operator()(const T &p_a, const T &p_b) const { return (p_a < p_b); }
};

// Global lock macro, relies on the static Mutex::_global_mutex.
void _global_lock();
void _global_unlock();

struct _GlobalLock {
	_GlobalLock() { _global_lock(); }
	~_GlobalLock() { _global_unlock(); }
};

#define GLOBAL_LOCK_FUNCTION _GlobalLock _global_lock_;

#if defined(__GNUC__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) x
#define unlikely(x) x
#endif

// Home for state needed from global functions
// that cannot be stored in Engine or OS due to e.g. circular includes

class CoreGlobals {
public:
	static bool leak_reporting_enabled;
	static bool print_line_enabled;
	static bool print_error_enabled;
};

#endif // GD_CLASSES_H