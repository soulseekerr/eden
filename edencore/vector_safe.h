#include <iostream>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <algorithm> // For std::copy

int example_vector_safe();

template <class T>
class VectorSafe_t {
private:
    std::unique_ptr<T[]> ptr_;      // Smart pointer for managing memory
    uint32_t capacity_;             // Total capacity of the vector
    uint32_t num_elements_;         // Number of elements currently in the vector
    uint8_t capacityMethod_;        // Capacity growth method

public:
    static constexpr uint32_t defaultCapacity = 1741;        // Default initial capacity
    static constexpr uint8_t defaultCapacityMethod = 1;      // Double the capacity
    static constexpr uint8_t logCapacityMethod = 2;          // Logarithmic capacity growth

    // Constructor
    constexpr explicit VectorSafe_t(uint32_t capacity = defaultCapacity, uint8_t capacityMethod = defaultCapacityMethod)
        : ptr_(std::make_unique<T[]>(capacity)), capacity_(capacity), num_elements_(0), capacityMethod_(capacityMethod) {
    }

    // Destructor
    ~VectorSafe_t() = default;

    // Copy Constructor
    VectorSafe_t(const VectorSafe_t<T>& other) 
        : ptr_(std::make_unique<T[]>(other.capacity_)), capacity_(other.capacity_), num_elements_(other.num_elements_), capacityMethod_(other.capacityMethod_) {
        std::copy(other.begin(), other.end(), begin());
    }
    
    // Move Constructor
    VectorSafe_t(VectorSafe_t<T>&& other) noexcept 
        : ptr_(std::move(other.ptr_)), capacity_(other.capacity_), num_elements_(other.num_elements_), capacityMethod_(other.capacityMethod_) {
        other.capacity_ = 0;
        other.num_elements_ = 0;
    }

    // Copy Assignment Operator
    VectorSafe_t<T>& operator=(const VectorSafe_t<T>& other) {
        if (this != &other) {
            ptr_ = std::make_unique<T[]>(other.capacity_);
            capacity_ = other.capacity_;
            num_elements_ = other.num_elements_;
            capacityMethod_ = other.capacityMethod_;
            std::copy(other.begin(), other.end(), begin());
        }
        return *this;
    }

    // Move Assignment Operator
    VectorSafe_t<T>& operator=(VectorSafe_t<T>&& other) noexcept {
        if (this != &other) {
            ptr_ = std::move(other.ptr_);
            capacity_ = other.capacity_;
            num_elements_ = other.num_elements_;
            capacityMethod_ = other.capacityMethod_;
            other.capacity_ = 0;
            other.num_elements_ = 0;
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

    // Size and capacity functions
    uint32_t size() const {
        return num_elements_;
    }

    uint32_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return num_elements_ == 0;
    }

    // Push back with capacity management
    void push_back(const T& key) {
        if (num_elements_ >= capacity_) {
            resize();
        }
        ptr_[num_elements_++] = key;
    }

    void push_back(T&& value) {
        if (num_elements_ >= capacity_) {
            resize();
        }
        ptr_[num_elements_++] = std::move(value);
    }

    // Pop back function
    void pop_back() {
        if (num_elements_ > 0) {
            --num_elements_;
        }
    }

    // Access with bounds checking
    T& at(uint32_t index) {
        if (index >= num_elements_) {
            throw std::out_of_range("Index out of range");
        }
        return ptr_[index];
    }

    // Erase function
    void erase(uint32_t index) {
        if (index >= num_elements_) {
            throw std::out_of_range("Index out of range");
        }
        for (uint32_t i = index; i < num_elements_ - 1; ++i) {
            ptr_[i] = std::move(ptr_[i + 1]);
        }
        --num_elements_;
    }

    // Clear function
    void clear() {
        num_elements_ = 0;
    }

    // Reserve function
    void reserve(uint32_t newCapacity) {
        if (newCapacity <= capacity_) return;
        reallocate(newCapacity);
    }

    // Reallocate memory with a new capacity
    void reallocate(uint32_t newCapacity) {
        std::unique_ptr<T[]> newData = std::make_unique<T[]>(newCapacity);
        for (size_t i = 0; i < num_elements_; ++i) {
            newData[i] = std::move(ptr_[i]);
        }
        ptr_ = std::move(newData);
        capacity_ = newCapacity;
    }

    // Shrink to fit function
    void shrink_to_fit() {
        if (num_elements_ >= capacity_) return;
        reallocate(num_elements_);
    }

    // Iterators
    T* begin() { return ptr_.get(); }
    T* end() { return ptr_.get() + num_elements_; }

    const T* begin() const { return ptr_.get(); }  // Const version
    const T* end() const { return ptr_.get() + num_elements_; }  // Const version


private:
    // Resize based on capacity method
    void resize() {
        uint32_t newCapacity;
        if (capacityMethod_ == logCapacityMethod) {
            newCapacity = capacity_ + std::log2(capacity_);
        } else {
            newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
        }
        reserve(newCapacity);
    }
};
