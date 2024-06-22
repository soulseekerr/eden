#pragma once

#include "datetime.h"

namespace eden {

class DataAttributes {
private:
    DateTime    cob_;

public:
    DataAttributes() = delete;
    explicit DataAttributes(const DateTime& cob) : cob_(cob) {}

    virtual ~DataAttributes() {}

    const DateTime& cob() const { return cob_; }
};

} // namespace eden