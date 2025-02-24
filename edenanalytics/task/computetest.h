#pragma once

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

namespace eden {

class ComputeTest : public TaskNode {
public:
    ComputeTest(std::shared_ptr<ScenarioTask> p) : TaskNode(p) {}
    virtual ~ComputeTest() {}

    virtual Response execute() override;
};

} // namespace eden