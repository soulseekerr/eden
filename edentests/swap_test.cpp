#include <gtest/gtest.h>
#include "models/hullwhite_1factor.h"
#include "products/vanilla_swap.h"
#include "pricers/swap_pricer.h"

// Demonstrate some basic assertions.
TEST(SwapTest, BasicAssertions) {
    try {

    double a = 0.1, sigma = 0.01; 

    HullWhiteModel1F model(a, sigma);

    std::vector<double> marketPrices = {0.254, 0.230, 0.210, 0.2, 0.21, 0.24, 0.25, 0.21, 0.24, 0.3};
    std::vector<double> times = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0};
    Calibration1F calib(model, marketPrices, times);
    model.setMarketRateCurve(times, marketPrices);
    calib.calibrate(0.3);

    SwapData    deal;
    deal.maturity = 5.0;
    deal.fixedLeg.notional = 100;
    deal.fixedLeg.rate = 0.2;
    deal.fixedLeg.type = LegType::Fixed;
    deal.fixedLeg.paymentDates = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0};
    deal.floatingLeg.notional = 100;
    deal.floatingLeg.type = LegType::Floating;
    deal.floatingLeg.paymentDates = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0};

    SwapPricer pricer(model, deal);

    double pv = pricer.priceSwap(deal.maturity, 0.03);

    auto vLegFixedPv = pricer.presentValueLeg(deal.maturity, 0.03, deal.fixedLeg);
    auto vLegFloatingPv = pricer.presentValueLeg(deal.maturity, 0.03, deal.floatingLeg);

    double pvFixed = std::accumulate(vLegFixedPv.begin(), vLegFixedPv.end(), 0);
    double pvFloating = std::accumulate(vLegFloatingPv.begin(), vLegFloatingPv.end(), 0);
    double pv2 = pvFixed - pvFloating;

    std::cout << "Fixed Leg Value: " << pvFixed << std::endl;
    std::cout << "Floating Leg Value: " << pvFloating << std::endl;
    std::cout << "Swap NPV: " << pv2 << std::endl;

    EXPECT_EQ(pv, pv2);

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}