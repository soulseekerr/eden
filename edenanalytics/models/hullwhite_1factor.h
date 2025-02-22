#pragma once

#include <cmath>
// #include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>

#include <iostream>
#include <stdexcept>

#include "vector.h"
#include "core/yieldcurve.h"

namespace edenanalytics {

/**
 * 
 * Hull-White 1 factor model: stochastic model for interest rates.
 * dr(t) = θ(t) − αr(t)dt + σdW(t)
 * 
 * r(t) is the short rate at time t.
 * α (mean reversion rate) controls how quickly the short rate reverts to the long-term mean.
 * σ (volatility) is the standard deviation of the short rate changes.
 * 𝜃(𝑡) is a time-dependent function that ensures the model fits the initial yield curve.
 * W(t) is a Wiener process (Brownian motion).
 * 
 * The short rate (the instantaneous interest rate) follows a mean-reverting process.
 * It is an extension of the Vasicek model.
 * 
 * Mean reversion: How quickly the interest rates revert to the mean level.
 * 
 * Volatility: Volatility of the short rate, affecting the dispersion of the intesrest rate paths.
 * 
 * Time-dependent Mean Level: theta function allows the model to fit the current term structure of interest rates, 
 * making it more flexible than the Vasicek model.
 * We calibrate the theta parameter so that the model fits the observed market prices of interest rate derivatives
 * such as bonds, caps, floors, or swaptions. 
 * This is done by adjusting theta so that the model's output matches the market prices.
 */

class HullWhiteModel1F {
private:
    // Speed of mean reversion, positive constant
    double alpha_;
    // Volatility of the short rate, positive constant
    double sigma_;
    // Time-dependent function chosen to fit the initial term structure of interest rates
    std::vector<double> theta_function_;
    // Initial Yield curve and corresponding maturities
    YieldCurve initialTermStructure_;

    std::default_random_engine generator_; // Random number generator

public:
    explicit HullWhiteModel1F(double alpha, double sigma, const YieldCurve& initialTermStructure);
    virtual ~HullWhiteModel1F() = default;
    
    _ALWAYS_INLINE_ const double speedMeanReversion() const { return alpha_; }
    _ALWAYS_INLINE_ const double volatilityShortRate() const { return sigma_; }
    _ALWAYS_INLINE_ const YieldCurve& initialTermStructure() const { return initialTermStructure_; }

    // Simulate short rate using the Euler-Maruyama method
    std::vector<double> simulateShortRate(double r0, double dt, int numSteps);

    // Calculate bond price analytically in the Hull-White model
    double bondPrice(double r0, double t, double T);

    // Method to calculate theta(t)
    double calculateTheta(double t) const;

    // Method to calculate forward rate f(0,t)
    double forwardRate(double t) const;
};

struct ICalibration {
    virtual void calibrate(double firstGuess) = 0;
};

} // namespace edenanalytics