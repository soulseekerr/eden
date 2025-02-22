#pragma once

#include "core.h"
#include "datetime.h"

namespace eden {

class DataAttributes {
private:
    DateTime cob_;

public:
    // Default constructor 
    DataAttributes() = delete;
    // Constructor accepting cob
    explicit DataAttributes(const DateTime& cob) : cob_(cob) {}
    // Destructor
    virtual ~DataAttributes() {}
    // Copy constructor
    DataAttributes(const DataAttributes& other) : cob_(other.cob_) {}
    // Move constructor
    DataAttributes(DataAttributes&& other) noexcept : cob_(std::move(other.cob_)) {}
    // Assignment operator
    DataAttributes& operator=(const DataAttributes& other) {
        if (this == &other) {
            return *this;  // self-assignment
        }
        cob_ = other.cob_;
        return *this;
    }
    // Move assignment operator
    DataAttributes& operator=(DataAttributes&& other) {
        if (this == &other) {
            return *this;  // self-assignment
        }
        cob_ = std::move(other.cob_);
        return *this;
    }
    // Equality operator
    bool operator==(const DataAttributes& other) const {
        return (cob_ == other.cob_);
    }
    // Inequality operator
    bool operator!=(const DataAttributes& other) const {
        return !(*this == other);
    }
    // Getter for cob
    _ALWAYS_INLINE_ const DateTime& cob() const { return cob_; }
};

} // namespace eden