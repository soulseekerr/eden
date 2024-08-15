#ifndef GODOT_MUTEX_H
#define GODOT_MUTEX_H

#include "gd_classes.h"

#include <mutex>
#define THREADING_NAMESPACE std

template <class MutexT>
class MutexLock;

template <class StdMutexT>
class MutexImpl {
	friend class MutexLock<MutexImpl<StdMutexT>>;

	using StdMutexType = StdMutexT;

	mutable StdMutexT mutex;

public:
	_ALWAYS_INLINE_ void lock() const {
		mutex.lock();
	}

	_ALWAYS_INLINE_ void unlock() const {
		mutex.unlock();
	}

	_ALWAYS_INLINE_ bool try_lock() const {
		return mutex.try_lock();
	}
};

using Mutex = MutexImpl<THREADING_NAMESPACE::recursive_mutex>; // Recursive, for general use

#endif // GODOT_MUTEX_H