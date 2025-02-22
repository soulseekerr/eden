#pragma once

#include "core.h"
#include "datetime.h"

namespace eden {

class Attributes {
private:
    DateTime cob_;

public:
    // Default constructor 
    Attributes() = delete;
    // Constructor accepting cob
    explicit Attributes(const DateTime& cob);
    // Destructor
    virtual ~Attributes();
    // Copy constructor
    Attributes(const Attributes& other);
    // Move constructor
    Attributes(Attributes&& other) noexcept;
    // Assignment operator
    Attributes& operator=(const Attributes& other);
    // Move assignment operator
    Attributes& operator=(Attributes&& other);
    // Equality operator
    bool operator==(const Attributes& other) const;
    // Inequality operator
    bool operator!=(const Attributes& other) const;
    // Getter for cob
    _ALWAYS_INLINE_ const DateTime& cob() const { return cob_; }
};

} // namespace eden