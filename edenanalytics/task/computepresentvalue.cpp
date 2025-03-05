
#include "task/computepresentvalue.h"
#include <format>
#include <iostream>

using namespace eden;

Response ComputePresentValue::execute() {
    const eden::DateTime& cob = attributes()->cob();
        
    auto msg = std::format(
        "Compute Present Value COB {}:", 
        cob.year_month_day_h());
        
    std::cout << msg << std::endl;

    double alpha = 0.1, sigma = 0.01; 

    eden::Vector_t<double> marketPrices;
    marketPrices.push_back(0.254);
    marketPrices.push_back(0.230);
    marketPrices.push_back(0.2);
    marketPrices.push_back(0.21);
    marketPrices.push_back(0.24);
    marketPrices.push_back(0.25);
    marketPrices.push_back(0.21);
    marketPrices.push_back(0.24);
    marketPrices.push_back(0.3);

    eden::Vector_t<double> times;
    times.push_back(0.5);
    times.push_back(1.0);
    times.push_back(1.5);
    times.push_back(2.0);
    times.push_back(2.5);
    times.push_back(3.0);
    times.push_back(3.5);
    times.push_back(4.0);
    times.push_back(4.5);
    times.push_back(5.0);

    auto initalTermStructure = YieldCurve(times, marketPrices);

    HullWhiteModel1F model(alpha, sigma, initalTermStructure);

    // Calibration1F calib(model, marketPrices, times);
    // model.setMarketRateCurve(times, marketPrices);
    // calib.calibrate(0.3);

    SwapData    deal;
    deal.maturity = 5.0;
    deal.fixedLeg.notional = 100;
    deal.fixedLeg.rate = 0.2;
    deal.fixedLeg.type = LegType::Fixed;
    deal.fixedLeg.paymentDates = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0};
    deal.floatingLeg.notional = 100;
    deal.floatingLeg.type = LegType::Floating;
    deal.floatingLeg.paymentDates = {0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0};

    SwapPricer1F pricer(model, deal);

    auto vLegFixedPv = pricer.presentValueLeg(deal.maturity, 0.03, deal.fixedLeg);
    auto vLegFloatingPv = pricer.presentValueLeg(deal.maturity, 0.03, deal.floatingLeg);

    double pvFixed = std::accumulate(vLegFixedPv.begin(), vLegFixedPv.end(), 0);
    double pvFloating = std::accumulate(vLegFloatingPv.begin(), vLegFloatingPv.end(), 0);
    double pv2 = pvFixed - pvFloating;

    std::cout << "Fixed Leg Value: " << pvFixed << std::endl;
    std::cout << "Floating Leg Value: " << pvFloating << std::endl;
    std::cout << "Swap NPV: " << pv2 << std::endl;

    return resp::success;
}

Response SavePresentValue::execute() {
    const eden::DateTime& cob = attributes()->cob();
    
    auto msg = std::format("Save Present Value COB {}:", cob.year_month_day_h());
    std::cout << msg << std::endl;

    return resp::success;
}