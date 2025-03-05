
#include "computetest.h"

#include <map>
#include <vector>
#include <string>

#include "utils.h"
#include "tasknode.h"
#include "datetime.h"

#include "scenariotask.h"
#include "core/creditcurve.h"

#include "models/hullwhite_1factor.h"
#include "products/vanilla_swap.h"
#include "pricers/swap_pricer.h"

using namespace eden;

Response ComputeTest::execute() {
    const eden::DateTime& cob = attributes()->cob();

    auto msg = std::format("Compute CVA COB {}:", cob.year_month_day_h());
            
    std::cout << msg << std::endl;

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
        std::cout << "EUR 1Y Spread: " << curve->getSpread(Tenor::T1Y) << std::endl;
    }

    return resp::success;
}