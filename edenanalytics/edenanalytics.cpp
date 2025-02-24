
#include "core/edenanalytics.h"

#include "models/hullwhite_1factor.h"
#include "models/hullwhite_3factor.h"
#include "pricers/swap_pricer.h"
// #include "pricers/mc_pricer.h"
#include "products/vanilla_swap.h"
#include "task/tasknode.h"
#include "core/creditcurve.h"
#include "task/scenariotask.h"
#include "task/computetest.h"

void print_version() {
    std::cout << eden::analytics::getVersion() << std::endl;
}