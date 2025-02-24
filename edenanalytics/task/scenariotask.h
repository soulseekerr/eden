#pragma once

#include "attributes.h"

namespace eden {

class ScenarioTask : public Attributes {
public:
    ScenarioTask() = delete;

    explicit ScenarioTask(const DateTime& dt, const std::string& scenario) 
        : Attributes(dt), scenario_(scenario) {}

    virtual ~ScenarioTask() {}

    const std::string& getScenario() const { return scenario_; }

private:
    std::string scenario_;
};

} // namespace eden 