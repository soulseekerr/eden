#pragma once

#include "task/tasknode.h"
#include "datetime.h"
#include "attributes.h"
#include "models/hullwhite_1factor.h"
#include "products/vanilla_swap.h"
#include "pricers/swap_pricer.h"

namespace eden {

// TODO add composition with Strategies = computation task 
class ComputePresentValue : public TaskNode {
public:
    ComputePresentValue(const std::shared_ptr<Attributes>& p) : TaskNode(p) {}
    virtual ~ComputePresentValue() {}

    Response execute() override;
};

class SavePresentValue : public TaskNode {
public:
    SavePresentValue(const std::shared_ptr<Attributes>& p) : TaskNode(p) {}
    virtual ~SavePresentValue() {}

    Response execute() override;
};

} // namespace eden