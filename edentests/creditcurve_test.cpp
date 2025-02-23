#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "core/creditcurve.h"  // Assuming your implementation is in this header file

// Helper function to create a sample JSON for testing
nlohmann::json createSampleJson() {
    nlohmann::json jsonData;
    jsonData["curves"]["USD"]["T1Y"] = 1.0;
    jsonData["curves"]["USD"]["T2Y"] = 2.0;
    jsonData["curves"]["USD"]["T5Y"] = 5.0;
    jsonData["curves"]["USD"]["T10Y"] = 7.0;
    jsonData["curves"]["USD"]["T30Y"] = 8.5;
    jsonData["curves"]["USD"]["T5Y"] = 5.0;
    jsonData["curves"]["EUR"]["T1Y"] = 0.8;
    jsonData["curves"]["EUR"]["T3Y"] = 2.5;
    jsonData["curves"]["EUR"]["T10Y"] = 3.0;
    jsonData["curves"]["EUR"]["T20Y"] = 4.3;
    jsonData["curves"]["EUR"]["T30Y"] = 6.0;
    return jsonData;
}

// Test valid spread lookup
TEST(CreditCurvesTest, ValidSpreadLookup) {
    nlohmann::json jsonData = createSampleJson();
    eden::V1::CreditCurveContainer creditCurves(jsonData);

    // auto& curveUSD = creditCurves.getCreditCurve("USD");
    // auto& curveEUR = creditCurves.getCreditCurve("EUR");

    // EXPECT_DOUBLE_EQ(curveEUR->getSpread(eden::Tenor::T1Y), 1.0);
    // EXPECT_DOUBLE_EQ(curveUSD->getSpread(eden::Tenor::T3Y), 2.5);
}

// Test interpolation for missing tenors
TEST(CreditCurvesTest, InterpolationForMissingTenor) {
    nlohmann::json jsonData = createSampleJson();
    eden::V1::CreditCurveContainer creditCurves(jsonData);

    auto& curveUSD = creditCurves.getCreditCurve("USD");
    auto& curveEUR = creditCurves.getCreditCurve("EUR");

    // 2Y is missing in the USD curve, but it should interpolate between 1Y (1.0) and 5Y (5.0)
    EXPECT_NEAR(curveEUR->getSpread(eden::Tenor::T2Y, true), 2.0, 0.01);

    // 7Y is missing in the EUR curve, interpolation should happen between 5Y and 10Y
    EXPECT_NEAR(curveUSD->getSpread(eden::Tenor::T7Y, true), 2.75, 0.01);
}

// Test interpolation disabled
TEST(CreditCurvesTest, NoInterpolationForMissingTenor) {
    nlohmann::json jsonData = createSampleJson();
    eden::V1::CreditCurveContainer creditCurves(jsonData);

    auto& curveUSD = creditCurves.getCreditCurve("USD");

    // 7Y is missing in the EUR curve, with interpolation disabled, this should throw an exception
    EXPECT_THROW(curveUSD->getSpread(eden::Tenor::T7Y, false), std::invalid_argument);
}

// Test handling of missing curve for currency
TEST(CreditCurvesTest, MissingCurrencyCurve) {
    nlohmann::json jsonData = createSampleJson();
    eden::V1::CreditCurveContainer creditCurves(jsonData);

    auto& curveUSD = creditCurves.getCreditCurve("USD");

    // USD curve exists, but ABC curve doesn't
    EXPECT_THROW(curveUSD->getSpread(eden::Tenor::T1Y), std::invalid_argument);
}

// Test handling of invalid tenor
TEST(CreditCurvesTest, InvalidTenor) {
    nlohmann::json jsonData = createSampleJson();
    eden::V1::CreditCurveContainer creditCurves(jsonData);

    auto& curveUSD = creditCurves.getCreditCurve("USD");

    // Invalid tenor, should throw exception
    EXPECT_THROW(curveUSD->getSpread(static_cast<eden::Tenor>(99), true), std::invalid_argument);
}

// Test if tenor exists in curve
TEST(CreditCurvesTest, HasTenorCheck) {
    nlohmann::json jsonData = createSampleJson();
    eden::V1::CreditCurveContainer creditCurves(jsonData);

    auto& curveEUR = creditCurves.getCreditCurve("USD");

    EXPECT_TRUE(curveEUR->hasTenor(eden::Tenor::T1Y));
    EXPECT_FALSE(curveEUR->hasTenor(eden::Tenor::T7Y));
}

// Test maturity lookup
TEST(CreditCurvesTest, MaturityLookup) {
    nlohmann::json jsonData = createSampleJson();
    eden::V1::CreditCurveContainer creditCurves(jsonData);

    auto& curveUSD = creditCurves.getCreditCurve("USD");

    EXPECT_DOUBLE_EQ(curveUSD->getMaturity(eden::Tenor::T1Y), 1.0);
    EXPECT_DOUBLE_EQ(curveUSD->getMaturity(eden::Tenor::T3Y), 3.0);
    EXPECT_DOUBLE_EQ(curveUSD->getMaturity(eden::Tenor::T10Y), 10.0);
}

