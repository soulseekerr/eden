#pragma once

#include "core.h"
#include "datetime.h"

namespace eden {

/**  
 * @brief Attributes class
 * @details This class is used to store the attributes of a task
*/
class Attributes {
private:
    DateTime cob_;

public:
    // Default constructor is deleted
    Attributes() = delete;
    // Constructor accepting cob as reference
    explicit Attributes(const DateTime& cob) noexcept;
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

using AttributeSPtr = std::shared_ptr<Attributes>;

} // namespace eden