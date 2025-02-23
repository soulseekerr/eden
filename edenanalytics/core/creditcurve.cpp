#include "creditcurve.h"

// Example usage
int exampleCreditCurve() {
    try {
        // Load JSON data for multiple CreditCurves
        std::ifstream file("all_credit_curves.json");
        if (!file) {
            throw std::runtime_error("Failed to open JSON file");
        }

        json jsonData;
        file >> jsonData;

        // Create a container for credit curves from the loaded JSON data
        eden::V1::CreditCurveContainer container(jsonData);

        // Retrieve and use the CreditCurve
        auto curve = container.getCreditCurve("EUR");
        if (curve) {
            std::cout << "EUR 1Y Spread: " << curve->getSpread(eden::Tenor::T1Y) << std::endl;
        }

        // Get spread for EUR, 1Y tenor (which exists)
        std::cout << "EUR 1Y Spread: " << curve->getSpread(eden::Tenor::T1Y) << "\n";

        // Get spread for EUR, 7Y tenor (which doesn't exist, so it should interpolate)
        std::cout << "EUR 7Y Spread (Interpolated): " << curve->getSpread(eden::Tenor::T7Y) << "\n";

        // Set allowInterpolation to false to prevent interpolation, will return exception on missing tenor
        try {
            std::cout << "USD 7Y Spread (No Interpolation): " << curve->getSpread(eden::Tenor::T7Y, false) << "\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }

        // Save all CreditCurves to a JSON file
        container.saveToJson("saved_credit_curves.json");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

int exampleCreditCurve2() {
    // Define a credit curve with some tenors and spreads
    std::unordered_map<eden::Tenor, double> curveData = {
        { eden::Tenor::T1Y, 0.02 },  // 2% spread at 1Y
        { eden::Tenor::T2Y, 0.03 },  // 3% spread at 2Y
        { eden::Tenor::T5Y, 0.05 }   // 5% spread at 5Y
    };

    // Create the CreditCurve object
    eden::V2::CreditCurve curve(std::move(curveData));

    // Example of getting spread for T1Y
    try {
        std::cout << "Spread for T1Y: " << curve.getSpread(eden::Tenor::T1Y) << std::endl;
        std::cout << "Spread for T2Y: " << curve.getSpread(eden::Tenor::T2Y) << std::endl;
        std::cout << "Spread for T5Y: " << curve.getSpread(eden::Tenor::T5Y) << std::endl;

        // Check if a tenor exists before accessing
        if (curve.hasTenor(eden::Tenor::T10Y)) {
            std::cout << "Spread for T10Y: " << curve.getSpread(eden::Tenor::T10Y) << std::endl;
        } else {
            std::cout << "T10Y tenor not found." << std::endl;
        }

        // Try getting an out-of-range tenor (T10Y)
        std::cout << "Spread for T10Y (interpolated): " << curve.getSpread(eden::Tenor::T10Y, true) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}