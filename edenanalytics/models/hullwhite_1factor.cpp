
#include "hullwhite_1factor.h"

using namespace edenanalytics;

HullWhiteModel1F::HullWhiteModel1F(
    double alpha, 
    double sigma, 
    const YieldCurve& initialTermStructure)
    :   alpha_(alpha), sigma_(sigma), initialTermStructure_(initialTermStructure),
        generator_(std::random_device{}()) {

}

// Simulate short rate using the Euler-Maruyama method
std::vector<double> HullWhiteModel1F::simulateShortRate(double r0, double dt, int numSteps) {
    std::vector<double> rates(numSteps);
    rates[0] = r0;

    std::normal_distribution<double> distribution(0.0, 1.0);

    for (int i = 1; i < numSteps; ++i) {
        double t = i * dt;
        double theta = calculateTheta(t);
        double dw = distribution(generator_) * std::sqrt(dt);
        double dr = (theta - alpha_ * rates[i - 1]) * dt + sigma_ * dw;
        rates[i] = rates[i - 1] + dr;
    }

    return rates;
}

// Calculate bond price analytically in the Hull-White model
double HullWhiteModel1F::bondPrice(double r0, double t, double T) {
    double B0T = std::exp(-initialTermStructure_.value(T) * T);
    double B0T_exp = B0T * std::exp(0.5 * sigma_ * sigma_ / (alpha_ * alpha_) * (1 - std::exp(-2 * alpha_ * (T - t))));
    double A = B0T_exp * std::exp(-alpha_ * (T - t) * r0);
    return A;
}

// Method to calculate theta(t)
double HullWhiteModel1F::calculateTheta(double t) const {
    // Approximate the derivative of the forward rate f(0,t)
    double dt = 0.0001; // small increment for numerical differentiation
    double fwdRate = forwardRate(t);
    double fwdRatePlusDt = forwardRate(t + dt);
    double dfdt = (fwdRatePlusDt - fwdRate) / dt;

    // Calculate theta(t) using the Hull-White formula
    double theta = dfdt + alpha_ * fwdRate + (sigma_ * sigma_ / (2 * alpha_)) * (1 - std::exp(-2 * alpha_ * t));
    return theta;
}

// Method to calculate forward rate f(0,t)
double HullWhiteModel1F::forwardRate(double t) const {
    return initialTermStructure_.value(t) + t * initialTermStructure_.value(t);
}