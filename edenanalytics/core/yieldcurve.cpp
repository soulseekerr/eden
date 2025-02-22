
#include "yieldcurve.h"

namespace edenanalytics {

YieldCurve::YieldCurve(const eden::Vector_t<double>& maturities, const eden::Vector_t<double>& values) 
    : maturities_(maturities), values_(values) {
    
    if (maturities_.size() != values_.size()) 
        throw std::out_of_range("Vectors are of different size");
}
    
double YieldCurve::value(double t) const {
    if (values_.empty() || values_.empty()) { 
        throw std::runtime_error("Market rate curve is not set."); 
    }
    
    auto it = std::lower_bound(maturities_.begin(), maturities_.end(), t);
    
    if (it == maturities_.end()) { return values_.back(); }
    if (it == maturities_.begin()) { return values_.front(); }
    
    size_t idx = std::distance(maturities_.begin(), it);
    
    double t1 = maturities_[idx - 1];
    double t2 = maturities_[idx];
    double r1 = values_[idx - 1];
    double r2 = values_[idx];
    
    return r1 + (r2 - r1) * (t - t1) / (t2 - t1);
}

} // namespace edenanalytics