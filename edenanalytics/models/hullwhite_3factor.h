#pragma once

#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>

#include "hullwhite_1factor.h"

namespace edenanalytics {

class HullWhiteModel3F : public IHullWhiteModel {
public:
    HullWhiteModel3F(double a, double sigma1, double sigma2, double sigma3)
        : a_(a), sigma1_(sigma1), sigma2_(sigma2), sigma3_(sigma3) {}

    double discountFactor(double t, double T, double r) const override {
        double B = (1.0 - std::exp(-a_ * (T - t))) / a_;
        double A = std::exp((B - (T - t)) * theta(t) - (sigma1_ * sigma1_ + sigma2_ * sigma2_ + sigma3_ * sigma3_) * B * B / (2 * a_));
        return A * std::exp(-B * r);
    }

    const double speedMeanReversion() const override { return a_; }

    const double volatilityShortRate() const override { return sigma1_; }

    void setTheta(const std::vector<double>& theta) override { theta_ = theta; }

    std::vector<double> simulateShortRatePath(double t, double T, double r0, double dt, int numPaths) const {
        std::vector<double> rates;
        rates.push_back(r0);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dist(0.0, 1.0);

        double r = r0;
        while (t < T) {
            double dW1 = std::sqrt(dt) * dist(gen);
            double dW2 = std::sqrt(dt) * dist(gen);
            double dW3 = std::sqrt(dt) * dist(gen);

            double theta_t = theta(t);
            double drift = a_ * (theta_t - r);
            r += drift * dt + sigma1_ * dW1 + sigma2_ * dW2 + sigma3_ * dW3;

            rates.push_back(r);
            t += dt;
        }
        return rates;
    }

private:
    double a_, sigma1_, sigma2_, sigma3_;
    std::vector<double> theta_;

    double theta(double t) const {
        int index = static_cast<int>(t);
        return (index < theta_.size()) ? theta_[index] : theta_.back();
    }
};

} // namespace edenanalytics