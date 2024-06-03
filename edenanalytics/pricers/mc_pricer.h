#pragma once

#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>

#include "models/hullwhite_1factor.h"
#include "models/hullwhite_3factor.h"
#include "products/vanilla_swap.h"
#include "swap_pricer.h"

namespace edenanalytics {

class MonteCarloPricer {
public:
    MonteCarloPricer(const IHullWhiteModel& model, const SwapData& swapData, int numPaths)
        : model_(model), swapData_(swapData), numPaths_(numPaths) {}

    double priceInstrunment(double t) const {
        double sumPayoffs = 0.0;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(0.0, 1.0);

        for (int i = 0; i < numPaths_; ++i) {
            double r = simulateShortRate(t, gen, dist);
            double payoff = calculatePayoff(t, r);
            sumPayoffs += payoff;
        }

        return sumPayoffs / numPaths_;
    }

private:
    const IHullWhiteModel& model_;
    const SwapData& swapData_;
    int numPaths_;

    double simulateShortRate(double t, std::mt19937& gen, std::normal_distribution<>& dist) const {
        double dt = 0.01; // Time step for simulation
        double r = 0.03; // Initial short rate
        double T = swapData_.maturity;
        while (t < T) {
            double dW = std::sqrt(dt) * dist(gen);
            // double theta = model_.discountFactor(t, T, r);
            // double drift = model_.discountFactor(t, t + dt, r) / theta - 1;
            // double a = model_.speed_mean_reversion();
            // double df = model_.discountFactor(t, t + dt, r);
            // double vol = std::sqrt((df * df - 1) / (2 * a));
            // r += a * (theta - r) * dt + vol * dW;

            double theta_t = theta(t);
            double drift = a_ * (theta_t - r);
            r += drift * dt + sigma_ * dW;

            rates.push_back(r);

            t += dt;
        }
        return r;
    }

    double calculatePayoff(double t, double r) const {
        SwapPricer swapPricer(model_, swapData_);
        return swapPricer.priceSwap(t, r);
    }
};

static int price_hullwhite1factor_vanilla_swap() {
    // Hull-White model parameters
    double a = 0.1;
    double sigma = 0.01;

    // Create the Hull-White model
    HullWhiteModel1F hwModel(a, sigma);

    // Market data (hypothetical)
    std::vector<double> marketPrices = {0.98, 0.95, 0.92, 0.89, 0.85}; // Example prices
    std::vector<double> times = {1.0, 2.0, 3.0, 4.0, 5.0}; // Corresponding times in years

    // Calibrate the model
    Calibration1F calibration(hwModel, marketPrices, times);
    calibration.calibrate();

    // Check the calibrated theta values
    for (double t : times) {
        std::cout << "Theta at time " << t << ": " << hwModel.discountFactor(0, t, 0.03) << std::endl;
    }

    // Swap leg data
    SwapLeg fixedLeg;
    fixedLeg.paymentDates = {1.0, 2.0, 3.0, 4.0, 5.0}; // Payment dates in years
    fixedLeg.rate = 0.05; // 5% fixed rate
    fixedLeg.notional = 1000000; // Fixed leg notional amount
    fixedLeg.type = LegType::Fixed;

    SwapLeg floatingLeg;
    floatingLeg.paymentDates = {1.0, 2.0, 3.0, 4.0, 5.0}; // Payment dates in years
    floatingLeg.rate = 0.03; // Placeholder for floating rate
    floatingLeg.notional = 1000000; // Floating leg notional amount
    floatingLeg.type = LegType::Floating;

    // Swap data
    SwapData swapData;
    swapData.fixedLeg = fixedLeg;
    swapData.floatingLeg = floatingLeg;
    swapData.maturity = 5.0; // Swap maturity in years

    // Create the swap pricer
    // SwapPricer swapPricer(hwModel, swapData);

    // Pricing the swap at time t = 0 with initial short rate r = 0.03
    // double t = 0.0;
    // double r = 0.03;
    // double swapPrice = swapPricer.priceSwap(t, r);

    // Monte Carlo pricing
    MonteCarloPricer mcPricer(hwModel, swapData, 10000); // 10,000 paths
    double swapPrice = mcPricer.priceInstrunment(0.0);

    std::cout << "Swap price (1F): " << swapPrice << std::endl;

    return 0;
}

static int price_hullwhite3factor_vanilla_swap() {
    // Hull-White model parameters
    double a = 0.1;
    double sigma1 = 0.01;
    double sigma2 = 0.01;
    double sigma3 = 0.01;

    // Create the Hull-White three-factor model
    HullWhiteModel3F hwModel(a, sigma1, sigma2, sigma3);

    // Define theta values (hypothetical)
    std::vector<double> theta = {0.03, 0.035, 0.04, 0.045, 0.05};
    hwModel.setTheta(theta);

    // Swap leg data
    SwapLeg fixedLeg;
    fixedLeg.paymentDates = {1.0, 2.0, 3.0, 4.0, 5.0}; // Payment dates in years
    fixedLeg.rate = 0.05; // 5% fixed rate
    fixedLeg.notional = 1000000; // Fixed leg notional amount
    fixedLeg.type = LegType::Fixed;

    SwapLeg floatingLeg;
    floatingLeg.paymentDates = {1.0, 2.0, 3.0, 4.0, 5.0}; // Payment dates in years
    floatingLeg.rate = 0.03; // Placeholder for floating rate
    floatingLeg.notional = 1000000; // Floating leg notional amount
    floatingLeg.type = LegType::Floating;

    // Swap data
    SwapData swapData;
    swapData.fixedLeg = fixedLeg;
    swapData.floatingLeg = floatingLeg;
    swapData.maturity = 5.0; // Swap maturity date in years

    // Monte Carlo pricing
    MonteCarloPricer mcPricer(hwModel, swapData, 10000); // 10,000 paths
    double swapPrice = mcPricer.priceInstrunment(0.0);

    std::cout << "Monte Carlo Swap price (3F): " << swapPrice << std::endl;

    return 0;
}

} // namespace edenanalytics
