#pragma once

#include <cmath>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "core.h"

namespace edenanalytics {

class YieldCurve {
private:
    std::vector<double> maturities_;
    std::vector<double> values_;
    
public:
    YieldCurve(const std::vector<double>& maturities, const std::vector<double>& values);    
    virtual ~YieldCurve() = default;
    
    _ALWAYS_INLINE_ double operator[](size_t index) const { 
        if (index >= values_.size()) { 
            throw std::out_of_range("Index is out of range"); 
        }
        return values_[index]; 
    }
    
    _ALWAYS_INLINE_ size_t size() const { return values_.size(); }
    
    double value(double t) const;
};
    

} // namespace edenanalytics
