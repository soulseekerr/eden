#pragma once

#include <cmath>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "vector.h"
#include "core.h"

namespace edenanalytics {

/**
 * When implementing this in C++, you would need to:
 * Input or generate the initial term structure (e.g., using market data).
 * Calibrate the θ(t) function to fit the term structure.
 * Use this calibrated model to simulate the short rate and price derivatives.
 * 
 */

// Will contain the initial term structure in the model
class YieldCurve {
private:
    eden::Vector_t<double> maturities_;
    eden::Vector_t<double> values_;
    
public:
    YieldCurve(const eden::Vector_t<double>& maturities, const eden::Vector_t<double>& values);    
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
