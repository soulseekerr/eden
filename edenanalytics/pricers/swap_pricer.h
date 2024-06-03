#pragma once

#include "models/hullwhite_1factor.h"
#include "products/vanilla_swap.h"

using namespace edenanalytics;

namespace edenanalytics {

class SwapPricer {
private:
    const IHullWhiteModel& model_;
    const SwapData& swapData_;

public:
    SwapPricer(const IHullWhiteModel& model, const SwapData& swapData)
        : model_(model), swapData_(swapData) {}

    double priceSwap(double t, double r) const {
        auto legPvFixed = presentValueLeg(t, r, swapData_.fixedLeg);
        auto legPvFloating = presentValueLeg(t, r, swapData_.floatingLeg);

        double pvFixed = std::accumulate(legPvFixed.begin(), legPvFixed.end(), 0);
        double pvFloating = std::accumulate(legPvFloating.begin(), legPvFloating.end(), 0);

        std::cout << "Fixed Leg Value: " << std::setprecision(3) << pvFixed << std::endl;
        std::cout << "Floating Leg Value: " << std::setprecision(3) << pvFloating << std::endl;
        std::cout << "Swap NPV: " << std::setprecision(3) << (pvFixed - pvFloating) << std::endl;

        return pvFixed - pvFloating;
    }

    std::vector<double> presentValueLeg(double t, double r, const SwapLeg& leg) const {
        std::vector<double> legPVs;
        auto legPv = 0.0;
        for (double paymentDate : leg.paymentDates) {
            double df = model_.discountFactor(t, paymentDate, r);

            if (leg.type == LegType::Fixed) {
                legPv = leg.rate * leg.notional * df;
            } else if (leg.type == LegType::Floating) {     
                // For simplicity, we assume the floating rate is the short rate r.
                legPv += leg.notional * (df - 1);
            }
            legPVs.push_back(legPv);
        }
        return legPVs;
    }
};

} // namespace edenanalytics