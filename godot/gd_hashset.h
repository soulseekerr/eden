#ifndef GD_HASHSET_H
#define GD_HASHSET_H

#include "gd_classes.h"
#include "gd_errors.h"
#include "gd_memory.h"

#include <stdint.h>

constexpr uint32_t HASH_TABLE_SIZE_MAX = 29;

const uint32_t hash_table_size_primes[HASH_TABLE_SIZE_MAX] = {
	5,
	13,
	23,
	47,
	97,
	193,
	389,
	769,
	1543,
	3079,
	6151,
	12289,
	24593,
	49157,
	98317,
	196613,
	393241,
	786433,
	1572869,
	3145739,
	6291469,
	12582917,
	25165843,
	50331653,
	100663319,
	201326611,
	402653189,
	805306457,
	1610612741,
};

// Computed with elem_i = UINT64_C (0 x FFFFFFFF FFFFFFFF ) / d_i + 1, where d_i is the i-th element of the above array.
const uint64_t hash_table_size_primes_inv[HASH_TABLE_SIZE_MAX] = {
	3689348814741910324,
	1418980313362273202,
	802032351030850071,
	392483916461905354,
	190172619316593316,
	95578984837873325,
	47420935922132524,
	23987963684927896,
	11955116055547344,
	5991147799191151,
	2998982941588287,
	1501077717772769,
	750081082979285,
	375261795343686,
	187625172388393,
	93822606204624,
	46909513691883,
	23456218233098,
	11728086747027,
	5864041509391,
	2932024948977,
	1466014921160,
	733007198436,
	366503839517,
	183251896093,
	91625960335,
	45812983922,
	22906489714,
	11453246088
};

/**
 * Fastmod computes ( n mod d ) given the precomputed c much faster than n % d.
 * The implementation of fastmod is based on the following paper by Daniel Lemire et al.
 * Faster Remainder by Direct Computation: Applications to Compilers and Software Libraries
 * https://arxiv.org/abs/1902.01961
 */
static _FORCE_INLINE_ uint32_t fastmod(const uint32_t n, const uint64_t c, const uint32_t d) {
#if defined(_MSC_VER)
	// Returns the upper 64 bits of the product of two 64-bit unsigned integers.
	// This intrinsic function is required since MSVC does not support unsigned 128-bit integers.
#if defined(_M_X64) || defined(_M_ARM64)
	return __umulh(c * n, d);
#else
	// Fallback to the slower method for 32-bit platforms.
	return n % d;
#endif // _M_X64 || _M_ARM64
#else
#ifdef __SIZEOF_INT128__
	// Prevent compiler warning, because we know what we are doing.
	uint64_t lowbits = c * n;
	__extension__ typedef unsigned __int128 uint128;
	return static_cast<uint64_t>(((uint128)lowbits * d) >> 64);
#else
	// Fallback to the slower method if no 128-bit unsigned integer type is available.
	return n % d;
#endif // __SIZEOF_INT128__
#endif // _MSC_VER
}

/**
 * Thomas Wang's 64-bit to 32-bit Hash function:
 * https://web.archive.org/web/20071223173210/https:/www.concentric.net/~Ttwang/tech/inthash.htm
 * 
 * This hash function provides a quick and efficient way to reduce 
 * a 64-bit integer to a 32-bit hash value, 
 * suitable for use in hash tables and other data structures 
 * where a smaller hash value is needed. 
 * The bitwise operations help in spreading out the bits of the input key 
 * to achieve a good distribution of hash values.
 * 
 * @param p_int - 64-bit unsigned integer key to be hashed
 * @return unsigned 32-bit value representing hashcode
 */
static inline uint32_t hash_one_uint64(const uint64_t p_int) {
	uint64_t v = p_int;
	v = (~v) + (v << 18); // v = (v << 18) - v - 1;
	v = v ^ (v >> 31);
	v = v * 21; // v = (v + (v << 2)) + (v << 4);
	v = v ^ (v >> 11);
	v = v + (v << 6);
	v = v ^ (v >> 22);
	return uint32_t(v);
}

struct HashMapHasherDefault {
    // Generic hash function for any type
    template <class T>
    static inline uint32_t hash(const T* p_pointer) {
        return hash_one_uint64((uint64_t)p_pointer);
    }
};

template <typename T>
struct HashMapComparatorDefault {
	static bool compare(const T& p_lhs, const T& p_rhs) {
		return p_lhs == p_rhs;
	}
};

/**
 * Implementation of Set using a bidi indexed hash map.
 * Use RBSet instead of this only if the following conditions are met:
 *
 * - You need to keep an iterator or const pointer to Key and you intend to add/remove elements in the meantime.
 * - Iteration order does matter (via operator<)
 *
 */

template <class TKey,
        class Hasher = HashMapHasherDefault,
        class Comparator = HashMapComparatorDefault<TKey>>
class HashSet {
private:
    uint32_t capacity_index__ = 0;
    uint32_t num_elements__ = 0;

	TKey* keys__ = nullptr;
	uint32_t* hash_to_key__ = nullptr;
	uint32_t* key_to_hash__ = nullptr;
	uint32_t* hashes__ = nullptr;

public:
    static constexpr uint32_t MIN_CAPACITY_INDEX = 2; // Use a prime.
	static constexpr float MAX_OCCUPANCY = 0.75;
	static constexpr uint32_t EMPTY_HASH = 0;

    HashSet() {
        capacity_index__ = MIN_CAPACITY_INDEX;
    }

    HashSet(uint32_t p_initial_capacity) {
        // Capacity cannot be 0
        capacity_index__ = 0;
        reserve(p_initial_capacity);
    }

    ~HashSet() {
        clear();

		if (keys__ != nullptr) {
			Memory::free_static(keys__);
			Memory::free_static(key_to_hash__);
			Memory::free_static(hash_to_key__);
			Memory::free_static(hashes__);
		}
    }

	HashSet(const HashSet &p_other) {
		_init_from(p_other);
	}

	void operator=(const HashSet &p_other) {
		if (this == &p_other) {
			return; // Ignore self assignment.
		}

		clear();

		if (keys__ != nullptr) {
			Memory::free_static(keys__);
			Memory::free_static(key_to_hash__);
			Memory::free_static(hash_to_key__);
			Memory::free_static(hashes__);
			keys__ = nullptr;
			hashes__ = nullptr;
			hash_to_key__ = nullptr;
			key_to_hash__ = nullptr;
		}

		_init_from(p_other);
	}

private:
	_FORCE_INLINE_ uint32_t _hash(const TKey &p_key) const {
		uint32_t hash = Hasher::hash(p_key);

		if (unlikely(hash == EMPTY_HASH)) {
			hash = EMPTY_HASH + 1;
		}

		return hash;
	}

	static _FORCE_INLINE_ uint32_t _get_probe_length(const uint32_t p_pos, const uint32_t p_hash, const uint32_t p_capacity, const uint64_t p_capacity_inv) {
		const uint32_t original_pos = fastmod(p_hash, p_capacity_inv, p_capacity);
		return fastmod(p_pos - original_pos + p_capacity, p_capacity_inv, p_capacity);
	}

	bool _lookup_pos(const TKey &p_key, uint32_t &r_pos) const {
		if (keys__ == nullptr || num_elements__ == 0) {
			return false; // Failed lookups, no elements
		}

		const uint32_t capacity = hash_table_size_primes[capacity_index__];
		const uint64_t capacity_inv = hash_table_size_primes_inv[capacity_index__];
		uint32_t hash = _hash(p_key);
		uint32_t pos = fastmod(hash, capacity_inv, capacity);
		uint32_t distance = 0;

		while (true) {
			if (hashes__[pos] == EMPTY_HASH) {
				return false;
			}

			if (distance > _get_probe_length(pos, hashes__[pos], capacity, capacity_inv)) {
				return false;
			}

			if (hashes__[pos] == hash && Comparator::compare(keys__[hash_to_key__[pos]], p_key)) {
				r_pos = hash_to_key__[pos];
				return true;
			}

			pos = fastmod(pos + 1, capacity_inv, capacity);
			distance++;
		}
	}

	uint32_t _insert_with_hash(uint32_t p_hash, uint32_t p_index) {
		const uint32_t capacity = hash_table_size_primes[capacity_index__];
		const uint64_t capacity_inv = hash_table_size_primes_inv[capacity_index__];
		uint32_t hash = p_hash;
		uint32_t index = p_index;
		uint32_t distance = 0;
		uint32_t pos = fastmod(hash, capacity_inv, capacity);

		while (true) {
			if (hashes__[pos] == EMPTY_HASH) {
				hashes__[pos] = hash;
				key_to_hash__[index] = pos;
				hash_to_key__[pos] = index;
				return pos;
			}

			// Not an empty slot, let's check the probing length of the existing one.
			uint32_t existing_probe_len = _get_probe_length(pos, hashes__[pos], capacity, capacity_inv);
			if (existing_probe_len < distance) {
				key_to_hash__[index] = pos;
				SWAP(hash, hashes__[pos]);
				SWAP(index, hash_to_key__[pos]);
				distance = existing_probe_len;
			}

			pos = fastmod(pos + 1, capacity_inv, capacity);
			distance++;
		}
	}

	void _init_from(const HashSet &p_other) {
		capacity_index__ = p_other.capacity_index__;
		num_elements__ = p_other.num_elements__;

		if (p_other.num_elements__ == 0) {
			return;
		}

		uint32_t capacity = hash_table_size_primes[capacity_index__];

		hashes__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));
		keys__ = reinterpret_cast<TKey *>(Memory::alloc_static(sizeof(TKey) * capacity));
		key_to_hash__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));
		hash_to_key__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));

		for (uint32_t i = 0; i < num_elements__; i++) {
			memnew_placement(&keys__[i], TKey(p_other.keys__[i]));
			key_to_hash__[i] = p_other.key_to_hash__[i];
		}

		for (uint32_t i = 0; i < capacity; i++) {
			hashes__[i] = p_other.hashes__[i];
			hash_to_key__[i] = p_other.hash_to_key__[i];
		}
	}

	void _resize_and_rehash(uint32_t p_new_capacity_index) {
		// Capacity can't be 0.
		capacity_index__ = MAX((uint32_t)MIN_CAPACITY_INDEX, p_new_capacity_index);

		uint32_t capacity = hash_table_size_primes[capacity_index__];

		uint32_t *old_hashes = hashes__;
		uint32_t *old_key_to_hash = key_to_hash__;

		hashes__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));
		keys__ = reinterpret_cast<TKey *>(Memory::realloc_static(keys__, sizeof(TKey) * capacity));
		key_to_hash__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));
		hash_to_key__ = reinterpret_cast<uint32_t *>(Memory::realloc_static(hash_to_key__, sizeof(uint32_t) * capacity));

		for (uint32_t i = 0; i < capacity; i++) {
			hashes__[i] = EMPTY_HASH;
		}

		for (uint32_t i = 0; i < num_elements__; i++) {
			uint32_t h = old_hashes[old_key_to_hash[i]];
			_insert_with_hash(h, i);
		}

		Memory::free_static(old_hashes);
		Memory::free_static(old_key_to_hash);
	}

	_FORCE_INLINE_ int32_t _insert(const TKey &p_key) {
		uint32_t capacity = hash_table_size_primes[capacity_index__];
		if (unlikely(keys__ == nullptr)) {
			// Allocate on demand to save memory.

			hashes__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));
			keys__ = reinterpret_cast<TKey *>(Memory::alloc_static(sizeof(TKey) * capacity));
			key_to_hash__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));
			hash_to_key__ = reinterpret_cast<uint32_t *>(Memory::alloc_static(sizeof(uint32_t) * capacity));

			for (uint32_t i = 0; i < capacity; i++) {
				hashes__[i] = EMPTY_HASH;
			}
		}

		uint32_t pos = 0;
		bool exists = _lookup_pos(p_key, pos);

		if (exists) {
			return pos;
		} else {
			if (num_elements__ + 1 > MAX_OCCUPANCY * capacity) {
				ERR_FAIL_COND_V_MSG(capacity_index__ + 1 == HASH_TABLE_SIZE_MAX, -1, "Hash table maximum capacity reached, aborting insertion.");
				_resize_and_rehash(capacity_index__ + 1);
			}

			uint32_t hash = _hash(p_key);
			memnew_placement(&keys__[num_elements__], TKey(p_key));
			_insert_with_hash(hash, num_elements__);
			num_elements__++;
			return num_elements__ - 1;
		}
	}

public:
	_ALWAYS_INLINE_ uint32_t size() const { return num_elements__; }

    bool is_empty() const { return num_elements__ == 0; }

    _ALWAYS_INLINE_ uint32_t get_capacity() const { return hash_table_size_primes[capacity_index__]; }

    // Reserves space for a number of elements, useful to avoid many resizes and rehashes.
	// If adding a known (possibly large) number of elements at once, must be larger than old capacity.
	void reserve(uint32_t p_new_capacity) {
        uint32_t new_index = capacity_index__;

        while (hash_table_size_primes[new_index] < p_new_capacity) {
            ERR_FAIL_COND_MSG(new_index + 1 == (uint32_t)HASH_TABLE_SIZE_MAX, nullptr);
            new_index++;
        }

		if (new_index == capacity_index__) {
			return;
		}

		if (keys__ == nullptr) {
			capacity_index__ = new_index;
			return; // Unallocated yet.
		}
		_resize_and_rehash(new_index);
    }

	_FORCE_INLINE_ bool has(const TKey &p_key) const {
		uint32_t _pos = 0;
		return _lookup_pos(p_key, _pos);
	}

	bool erase(const TKey &p_key) {
		uint32_t pos = 0;
		bool exists = _lookup_pos(p_key, pos);

		if (!exists) {
			return false;
		}

		uint32_t key_pos = pos;
		pos = key_to_hash__[pos]; //make hash pos

		const uint32_t capacity = hash_table_size_primes[capacity_index__];
		const uint64_t capacity_inv = hash_table_size_primes_inv[capacity_index__];
		uint32_t next_pos = fastmod(pos + 1, capacity_inv, capacity);
		while (hashes__[next_pos] != EMPTY_HASH && _get_probe_length(next_pos, hashes__[next_pos], capacity, capacity_inv) != 0) {
			uint32_t kpos = hash_to_key__[pos];
			uint32_t kpos_next = hash_to_key__[next_pos];
			SWAP(key_to_hash__[kpos], key_to_hash__[kpos_next]);
			SWAP(hashes__[next_pos], hashes__[pos]);
			SWAP(hash_to_key__[next_pos], hash_to_key__[pos]);

			pos = next_pos;
			next_pos = fastmod(pos + 1, capacity_inv, capacity);
		}

		hashes__[pos] = EMPTY_HASH;
		keys__[key_pos].~TKey();
		num_elements__--;
		if (key_pos < num_elements__) {
			// Not the last key, move the last one here to keep keys lineal
			memnew_placement(&keys__[key_pos], TKey(keys__[num_elements__]));
			keys__[num_elements__].~TKey();
			key_to_hash__[key_pos] = key_to_hash__[num_elements__];
			hash_to_key__[key_to_hash__[num_elements__]] = key_pos;
		}

		return true;
	}

    void clear() {
		if (keys__ == nullptr || num_elements__ == 0) {
			return;
		}
		uint32_t capacity = hash_table_size_primes[capacity_index__];
		for (uint32_t i = 0; i < capacity; i++) {
			hashes__[i] = EMPTY_HASH;
		}
		for (uint32_t i = 0; i < num_elements__; i++) {
			keys__[i].~TKey();
		}

		num_elements__ = 0;
    }

    void reset() {
        clear();

        if (keys__ != nullptr) {
			Memory::free_static(keys__);
			Memory::free_static(key_to_hash__);
			Memory::free_static(hash_to_key__);
			Memory::free_static(hashes__);
			keys__ = nullptr;
			hashes__ = nullptr;
			hash_to_key__ = nullptr;
			key_to_hash__ = nullptr;
		}
		capacity_index__ = MIN_CAPACITY_INDEX;
    }

	// Iterator API

	struct Iterator {
		_FORCE_INLINE_ const TKey &operator*() const {
			return keys__[index];
		}
		_FORCE_INLINE_ const TKey *operator->() const {
			return &keys__[index];
		}
		_FORCE_INLINE_ Iterator &operator++() {
			index__++;
			if (index__ >= (int32_t)num_keys__) {
				index__ = -1;
				keys__ = nullptr;
				num_keys__ = 0;
			}
			return *this;
		}
		_FORCE_INLINE_ Iterator &operator--() {
			index__--;
			if (index__ < 0) {
				index__ = -1;
				keys__ = nullptr;
				num_keys__ = 0;
			}
			return *this;
		}

		_FORCE_INLINE_ bool operator==(const Iterator &b) const { return keys__ == b.keys__ && index__ == b.index__; }
		_FORCE_INLINE_ bool operator!=(const Iterator &b) const { return keys__ != b.keys__ || index__ != b.index__; }

		_FORCE_INLINE_ explicit operator bool() const {
			return keys__ != nullptr;
		}

		_FORCE_INLINE_ Iterator(const TKey *p_keys, uint32_t p_num_keys, int32_t p_index = -1) {
			keys__ = p_keys;
			num_keys__ = p_num_keys;
			index__ = p_index;
		}
		_FORCE_INLINE_ Iterator() {}
		_FORCE_INLINE_ Iterator(const Iterator &p_it) {
			keys__ = p_it.keys__;
			num_keys__ = p_it.num_keys__;
			index__ = p_it.index__;
		}
		_FORCE_INLINE_ void operator=(const Iterator &p_it) {
			keys__ = p_it.keys__;
			num_keys__ = p_it.num_keys__;
			index__ = p_it.index__;
		}

	private:
		const TKey* keys__ = nullptr;
		uint32_t num_keys__ = 0;
		int32_t index__ = -1;
	};

	_FORCE_INLINE_ Iterator begin() const {
		return num_elements__ ? Iterator(keys__, num_elements__, 0) : Iterator();
	}
	_FORCE_INLINE_ Iterator end() const {
		return Iterator();
	}
	_FORCE_INLINE_ Iterator last() const {
		if (num_elements__ == 0) {
			return Iterator();
		}
		return Iterator(keys__, num_elements__, num_elements__ - 1);
	}

	_FORCE_INLINE_ Iterator find(const TKey &p_key) const {
		uint32_t pos = 0;
		bool exists = _lookup_pos(p_key, pos);
		if (!exists) {
			return end();
		}
		return Iterator(keys__, num_elements__, pos);
	}

	_FORCE_INLINE_ void remove(const Iterator &p_iter) {
		if (p_iter) {
			erase(*p_iter);
		}
	}

	/* Insert */

	Iterator insert(const TKey &p_key) {
		uint32_t pos = _insert(p_key);
		return Iterator(keys__, num_elements__, pos);
	}

};

#endif // GD_HASHSET_H