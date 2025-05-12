#pragma once

#include "workflow.h"
#include <nlohmann/json.hpp>
#include <string>

namespace eden {

/**
 * @brief IWorkflowSerializer interface
 * @details This interface defines the methods for serializing and deserializing the Workflow object.
 * @note The implementation of this interface should handle the conversion of the Workflow object to and from a JSON format.    
 */
class IWorkflowSerializer {
public:
    virtual ~IWorkflowSerializer() = default;
    virtual void save(const WorkflowUPtr& workflow, const std::string& filepath) = 0;
    virtual void load(WorkflowUPtr& workflow, const std::string& filepath) = 0;
    virtual std::string version() const = 0;
};

/**
 * @brief WorkflowSerializer class
 * @details This class is used to serialize and deserialize the Workflow object to and from a JSON file.
 * @note The JSON file is used to store the workflow tasks, dependencies, and node links.
 */
class WorkflowSerializer : public IWorkflowSerializer {
public:
    virtual void save(const WorkflowUPtr& workflow, const std::string& filepath) override;
    virtual void load(WorkflowUPtr& workflow, const std::string& filepath) override;
    virtual std::string version() const override { return "v1"; }
};

} // namespace eden
