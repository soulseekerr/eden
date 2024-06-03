#pragma once

#include <vector>
#include <cmath>
#include <random>
#include <numeric>
#include <iostream>

/** Hull-White Model Class
 * double a;     // Mean reversion rate
 * double sigma; // Volatility
 * double r0;    // Initial interest rate
 */ 

class HullWhiteModelV1 {
public:
    HullWhiteModelV1(double a, double sigma, double r0)
        : a_(a), sigma_(sigma), r0_(r0),
        gen_(rd_()), __NO_MATH_ERRNO__(0.0, 1.0) {}

    ~HullWhiteModelV1() {}

    std::vector<double> simulateInterestRatePath(double T, int steps) const {
        std::vector<double> rates(steps + 1);
        rates[0] = r0_;
        double dt = T / steps;
        for (int i = 1; i <= steps; ++i) {
            double dw = std::sqrt(dt) * norm_dist_(gen_);
            rates[i] = rates[i - 1] + a_ * (0 - rates[i - 1]) * dt + sigma_ * dw;
        }
        return rates;
    }

    double simulateShortRate(double dt, double theta, double& currentTime) {
        double dr = (theta - a_ * r0_) * dt + sigma_ * sqrt(dt) * norm_dist_(gen_);
        r0_ += dr;
        currentTime += dt;
        return r0_;
    }

    void reset(double r0) { r0_ = r0; }

    double getInitialRate() const { return r0_; }

private:
    // Hull White parameters
    double a_, sigma_, r0_;
    /**
     * Random number generators and distributions inherently involve state changes 
     * (e.g., updating the state of the generator to produce the next random number). 
     * However, from the perspective of the class user, generating a random number is 
     * conceptually a read-only operation. Thus, marking these members as mutable allows 
     * you to maintain the logical constness of the simulateInterestRatePath method, 
     * while still allowing it to generate random numbers.
     */
    mutable std::mt19937 gen_;
    mutable std::random_device rd_;
    mutable std::normal_distribution<> norm_dist_;
};

class HullWhiteModelV2 {
private:
    double a;  // Mean reversion speed
    double sigma;  // Volatility
    double r0;  // Initial short rate

public:
    HullWhiteModelV2(double meanReversion, double sigma, double initialRate)
        : a(meanReversion), sigma(sigma), r0(initialRate) {}

    double shortRate(double t);
    double bondPrice(double T);
    double zeroCouponBondPrice(double t, double T);

private:
    double A(double t, double T);
    double B(double t, double T);
    double forwardRate(double t, double T);
};
    
inline double HullWhiteModelV2::shortRate(double t) {
    return r0 * exp(-a * t);  // Simplified short rate
}

inline double HullWhiteModelV2::A(double t, double T) {
    double B_t_T = B(t, T);
    double term1 = (sigma * sigma / (2 * a * a)) * (1 - exp(-a * (T - t))) * (1 - exp(-a * (T - t)));
    double term2 = (sigma * sigma / (4 * a)) * (1 - exp(-2 * a * (T - t)));
    return exp(term1 - term2);
}

inline double HullWhiteModelV2::B(double t, double T) {
    return (1 - exp(-a * (T - t))) / a;
}

inline double HullWhiteModelV2::forwardRate(double t, double T) {
    return r0 * exp(-a * t) + (sigma * sigma / (2 * a * a)) * (1 - exp(-2 * a * t));
}

inline double HullWhiteModelV2::zeroCouponBondPrice(double t, double T) {
    double B_t_T = B(t, T);
    double A_t_T = A(t, T);
    return A_t_T * exp(-B_t_T * shortRate(t));
}

double HullWhiteModelV2::bondPrice(double T) {
    return zeroCouponBondPrice(0, T);
}


static int get_hullwhite_v2_values() {
    double meanReversion = 0.1;
    double sigma = 0.01;
    double initialRate = 0.05;
    
    HullWhiteModelV2 model(meanReversion, sigma, initialRate);

    double t = 0;
    double T = 10;

    std::cout << "Short rate at time t = " << t << " is: " << model.shortRate(t) << std::endl;
    std::cout << "Zero-coupon bond price for maturity T = " << T << " is: " << model.bondPrice(T) << std::endl;

    return 0;
}