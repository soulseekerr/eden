
#include "gd_mutex.h"

static Mutex _global_mutex;

void _global_lock() {
	_global_mutex.lock();
}

void _global_unlock() {
	_global_mutex.unlock();
}

template class MutexImpl<THREADING_NAMESPACE::recursive_mutex>;
template class MutexImpl<THREADING_NAMESPACE::mutex>;