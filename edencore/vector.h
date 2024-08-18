#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <iterator>

namespace eden {

void example_vector();

template <class T>
class Vector_t {
public:
    constexpr explicit Vector_t(const uint32_t capacity = defaultCapacity, const uint8_t capacityMethod = defaultCapacityMethod) 
        : ptr_(new T[capacity]), capacity_(capacity), num_elements_(0), capacityMethod_(capacityMethod) {
    }

    ~Vector_t() {
        if (ptr_ != nullptr) {
            delete [] ptr_;
            ptr_ = nullptr;
        }
    }

    Vector_t(const Vector_t<T>& other) 
        : ptr_(new T[other.capacity_]), capacity_(other.capacity_), num_elements_(other.num_elements_), capacityMethod_(other.capacityMethod_) {
        
        std::copy(other.begin(), other.end(), begin());
    }
    
    Vector_t(Vector_t<T>&& other) noexcept 
        : ptr_(other.ptr_), num_elements_(other.num_elements_), capacity_(other.capacity_), capacityMethod_(other.capacityMethod_) {
    
        other.ptr_ = nullptr;
        other.num_elements_ = 0;
        other.capacity_ = 0;
    }

    Vector_t<T>& operator=(const Vector_t<T>& other) {
        if (this != &other) {
            delete[] ptr_;
            ptr_ = new T[other.capacity_];
            num_elements_ = other.num_elements_;
            capacity_ = other.capacity_;
            capacityMethod_ = other.capacityMethod_;
            std::copy(other.begin(), other.end(), begin());
        }
        return *this;
    }

    Vector_t<T>& operator=(Vector_t<T>&& other) noexcept {
        if (this != &other) {
            delete [] ptr_;
            ptr_ = other.ptr_;
            num_elements_ = other.num_elements_;
            capacity_ = other.capacity_;
            capacityMethod_ = other.capacityMethod_;
            other.ptr_ = nullptr;
            other.num_elements_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Access without bounds checking
    T& operator[](size_t index) {
        return ptr_[index];
    }

    const T& operator[](size_t index) const {
        return ptr_[index];
    }

    uint32_t size() const {
        return  num_elements_;
    }

    uint32_t capacity() const {
        return  capacity_;
    }

    bool empty() const {
        return num_elements_ == 0;
    }

    void push_back(const T& key) {
        if (capacityMethod_ == logCapacityMethod) {

        }

        if (num_elements_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ptr_[num_elements_++] == key;
    }

    void push_back(T&& value) {
        if (num_elements_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        ptr_[num_elements_++] = std::move(value);
    }

    void pop_back() {
        if (num_elements_ > 0) {
            --num_elements_;
        }
    }

    T& at(const uint32_t index) {
        if (index >= num_elements_) {
            throw std::out_of_range("Index out of range");
        }
        return ptr_[index];
    }

    void erase(uint32_t index) {
        if (index >= num_elements_) {
            throw std::out_of_range("Index out of range");
        }
        for (uint32_t i = index; i < num_elements_ - 1; ++i) {
            ptr_[i] = ptr_[i + 1];
        }
        --num_elements_;
    }

    void clear() {
        num_elements_ = 0;
    }

    void reserve(uint32_t newCapacity) {
        if (newCapacity <= capacity_) return;
        
        reallocate(newCapacity);
    }

    void reallocate(uint32_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < num_elements_; ++i) {
            newData[i] = std::move(ptr_[i]);
        }
        delete [] ptr_;
        ptr_ = newData;
        num_elements_ = newCapacity;
    }

    void shrink_to_fit() {
        if (num_elements_ >= capacity_) return;

        reallocate(num_elements_);
    }

    // Iterators
    T* begin() { return ptr_; }
    T* end() { return ptr_ + num_elements_; }

    static constexpr uint32_t defaultCapacity = 1741;

    static constexpr uint8_t defaultCapacityMethod = 1; // Double
    static constexpr uint8_t logCapacityMethod = 2; // Log

private:
    mutable T* ptr_;
    uint32_t capacity_;
    uint32_t num_elements_;

    uint8_t capacityMethod_;
};

} // namespace eden