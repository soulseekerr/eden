#pragma once

#include "attributes.h"

namespace eden {

class TaskParameters : public Attributes {
public:
    TaskParameters() = delete;

    explicit TaskParameters(const DateTime& dt, const std::string& scenario) : Attributes(dt), scenario_(scenario) {}

    virtual ~TaskParameters() {}

    const std::string& getScenario() const { return scenario_; }

private:
    std::string scenario_;
};

} // namespace eden 