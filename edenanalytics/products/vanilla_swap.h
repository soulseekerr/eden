#pragma once

#include "models/hullwhite_1factor.h"
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <stdexcept>

struct CashFlow {
    double amount;
    std::chrono::system_clock::time_point payment_date;
};


using namespace edenanalytics;

namespace edenanalytics {

enum class LegType { Fixed, Floating };

struct SwapLeg {
    std::vector<double> paymentDates;
    double rate; // For fixed leg, this is the fixed rate; for floating leg, this can be an assumed average rate
    double notional;
    LegType type;
};

struct SwapData {
    SwapLeg fixedLeg;
    SwapLeg floatingLeg;
    double maturity;
};

class InterestRateSwap {
public:
    InterestRateSwap(double notionalA, double fixedRateA,
        double notionalB, double fixedRateB,
        double discountRateA, double discountRateB,
        double T, double dt)
    : notionalA_(notionalA), fixedRateA_(fixedRateA),
      notionalB_(notionalB), fixedRateB_(fixedRateB),
      T_(T), dt_(dt) {}

    virtual ~InterestRateSwap() {}

    double getNotionalA() const { return notionalA_; }

    double getFixedRateA() const { return fixedRateA_; }

    double getNotionalB() const { return notionalB_; }

    double getFixedRateB() const { return fixedRateB_; }

    double getMaturity() const { return T_; }

    double getTimeStep() const { return dt_; }

private:
    double notionalA_, fixedRateA_, notionalB_, fixedRateB_;
    double T_, dt_;
};

} // namespace edenanalytics