#pragma once

#include "data_attributes.h"

namespace eden {

class TaskParameters : public DataAttributes {
public:
    TaskParameters() = delete;

    explicit TaskParameters(const DateTime& dt, const std::string& scenario) : DataAttributes(dt), scenario_(scenario) {}

    virtual ~TaskParameters() {}

    const std::string& getScenario() const { return scenario_; }

private:
    std::string scenario_;
};

} // namespace eden 