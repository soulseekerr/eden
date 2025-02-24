
#include "attributes.h"

using namespace eden;

Attributes::Attributes(const DateTime& cob) noexcept : cob_(cob) {}
    
 // Destructor
 Attributes::~Attributes() {}

 // Copy constructor
 Attributes::Attributes(const Attributes& other) : cob_(other.cob_) {}

 // Move constructor
Attributes::Attributes(Attributes&& other) noexcept : cob_(std::move(other.cob_)) {}

// Assignment operator
Attributes& Attributes::operator=(const Attributes& other) {
    if (this == &other) {
        return *this;
    }
    cob_ = other.cob_;
    return *this;
}

// Move assignment operator
Attributes& Attributes::operator=(Attributes&& other) {
    if (this == &other) {
        return *this;
    }
    cob_ = std::move(other.cob_);
    return *this;
}

// Equality operator
bool Attributes::operator==(const Attributes& other) const {
    return (cob_ == other.cob_);
}
    
// Inequality operator
bool Attributes::operator!=(const Attributes& other) const {
    return !(*this == other);
}