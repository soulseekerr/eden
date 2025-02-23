#pragma once

#include <map>
#include <vector>
#include <string>

#include "utils.h"
#include "tasknode.h"
#include "datetime.h"

#include "taskparameters.h"
#include "core/creditcurve.h"

#include "models/hullwhite_1factor.h"
#include "products/vanilla_swap.h"
#include "pricers/swap_pricer.h"


namespace eden {

class ComputeCVA : public ATaskNode {
public:
    ComputeCVA(const std::shared_ptr<TaskParameters>& p) : ATaskNode(p) {}
    virtual ~ComputeCVA() {}

    Response execute() override {
        const eden::DateTime& cob = this->params()->cob();
            
        auto msg = std::format(
            "Compute CVA COB {}:", 
            cob.year_month_day_h());
            
        std::cout << msg << std::endl;

        // std::vector<std::string> tenors = {"0M", "3M", "6M", "1Y", "2Y", "5Y", "7Y", "10Y", "20Y", "30Y"};
        // std::vector<double> spreads = {108, 109, 109, 142, 215, 290, 412, 472, 503, 531, 542};
        // CreditCurve curve(tenors, spreads);
        
        // std::cout << "Tenors,Spreads: " << curve.toString() << std::endl;

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

        return Response(Status::OK, "Computation is successful");
    }
};

} // namespace eden