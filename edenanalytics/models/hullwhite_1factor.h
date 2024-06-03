#pragma once

#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <random>
#include <cmath>

namespace edenanalytics {

/**
 * Hull-White 1 factor model: stochastic model for interest rates.
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

struct IHullWhiteModel {
    // Speed of mean reversion
    virtual const double speedMeanReversion() const = 0;
    // Volatility of the short rate
    virtual const double volatilityShortRate() const = 0;
    // Time dependent mean level allocated from the calibration
    virtual void setTheta(const std::vector<double>& theta) = 0;
    // Discount factor
    virtual double discountFactor(double t, double T, double r) const = 0;
};

class HullWhiteModel1F : public IHullWhiteModel {
private:
    // Speed of mean reversion, positive constant
    double a_;
    // Volatility of the short rate, positive constant
    double sigma_;
    // Time-dependent function chosen to fit the initial term structure of interest rates
    std::vector<double> theta_;

    std::vector<double> marketTimes_;
    std::vector<double> marketRates_;

public:
    HullWhiteModel1F(double a, double sigma)
        : a_(a), sigma_(sigma) {}

    virtual ~HullWhiteModel1F() {}
    
    const double speedMeanReversion() const override { return a_; }

    const double volatilityShortRate() const override { return sigma_; }

    void setTheta(const std::vector<double>& theta) override { theta_ = theta; }

    void setMarketRateCurve(const std::vector<double>& times, const std::vector<double>& rates) {
        marketTimes_ = times;
        marketRates_ = rates;
    }

    // Discount factor for a zero-coupon bond maturing at time T when observed at time t
    double discountFactor(double t, double T, double r) const override {
        double B = (1.0 - std::exp(-a_ * (T - t))) / a_;
        double A = std::exp((B - (T - t)) * theta(t) - (sigma_ * sigma_ / (4 * a_)) * (1 - std::exp(-2 * a_ * (T - t))) / (2 * a_));
        return A * std::exp(-B * r);
    }

    double marketRateForTime(double t) const {
        if (marketTimes_.empty() || marketRates_.empty()) {
            throw std::runtime_error("Market rate curve is not set.");
        }
        auto it = std::lower_bound(marketTimes_.begin(), marketTimes_.end(), t);
        if (it == marketTimes_.end()) {
            return marketRates_.back();
        }
        if (it == marketTimes_.begin()) {
            return marketRates_.front();
        }
        size_t idx = std::distance(marketTimes_.begin(), it);
        double t1 = marketTimes_[idx - 1];
        double t2 = marketTimes_[idx];
        double r1 = marketRates_[idx - 1];
        double r2 = marketRates_[idx];
        return r1 + (r2 - r1) * (t - t1) / (t2 - t1);
    }

private:
    // Assuming piecewise constant theta(t)
    double theta(double t) const {    
        int index = static_cast<int>(t);
        return (index < theta_.size()) ? theta_[index] : theta_.back();
    }
};

struct ICalibration {
    virtual void calibrate(double firstGuess) = 0;
};

class Calibration1F : public ICalibration {
private:
    HullWhiteModel1F& model_;
    std::vector<double> marketPrices_;
    std::vector<double> times_;
    // Gradient descent parameters
    const double learningRate = 0.01;
    const int maxIterations = 1000;
    const double tolerance = 1e-6;

public:
    Calibration1F(HullWhiteModel1F& model, 
        const std::vector<double>& marketPrices, 
        const std::vector<double>& times)
        : model_(model), marketPrices_(marketPrices), times_(times) {}

    void calibrate(double firstGuess) override {
        // Dummy calibration routine: a real one would involve an optimizer
        // We use agradient descent
        std::vector<double> initialTheta(times_.size(), firstGuess); // Initial guess

        // Lambda function
        auto objectiveFunction = [this](const std::vector<double>& theta) {
            return this->objective(theta);
        };
        
        // Use a simple optimizer, e.g., gradient descent or a library like NLopt
        std::vector<double> optimalTheta = optimize(initialTheta, objectiveFunction);

        // Set the optimal theta in the model
        model_.setTheta(optimalTheta);
    }

private:
    double objective(const std::vector<double>& theta) const {
        model_.setTheta(theta);
        double error = 0.0;
        for (size_t i = 0; i < marketPrices_.size(); ++i) {
            double modelPrice = modelPriceForTime(times_[i]);
            error += std::pow(modelPrice - marketPrices_[i], 2);
        }
        return error;
    }

    double modelPriceForTime(double time) const {
        // Placeholder: use the model to calculate price for a given time
        // double r = 0.03; // Assumed short rate
        double r = model_.marketRateForTime(time);
        return model_.discountFactor(0, time, r); // Placeholder for real price calculation
    }

    // Gradient descent
    std::vector<double> optimize(const std::vector<double>& initialTheta, std::function<double(const std::vector<double>&)> objectiveFunction) {
        std::vector<double> theta = initialTheta;
        for (int iter = 0; iter < maxIterations; ++iter) {

            std::vector<double> gradients = computeGradients(theta, objectiveFunction);

            bool converged = true;
            for (size_t i = 0; i < theta.size(); ++i) {

                theta[i] -= learningRate * gradients[i];

                if (std::abs(learningRate * gradients[i]) > tolerance) {
                    converged = false;
                }
            }

            if (converged) {
                break;
            }
        }
        return theta;
    }

    std::vector<double> computeGradients(const std::vector<double>& theta, std::function<double(const std::vector<double>&)> objectiveFunction) {

        std::vector<double> gradients(theta.size());
        const double h = 1e-5;

        for (size_t i = 0; i < theta.size(); ++i) {

            std::vector<double> thetaPlus = theta;
            std::vector<double> thetaMinus = theta;
            
            thetaPlus[i] += h;
            thetaMinus[i] -= h;

            gradients[i] = (objectiveFunction(thetaPlus) - objectiveFunction(thetaMinus)) / (2 * h);
        }
        return gradients;
    }
};

} // namespace edenanalytics