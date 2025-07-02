#pragma once

#include "attributes.h"
#include "context.h"
#include <memory>
#include <string>

namespace eden {

using TaskID  = int32_t;

/**
  * * @brief Interface for all tasks in the workflow.
  * * @details This interface defines the basic structure and behavior of task  * that can be executed within a workflow. Each task must implement the `run`
  * method, which is responsible for executing the task's logic. The `status`
  * method provides information about the current state of the task.
  */
class ITask {
private:
    TaskID taskID_;
    std::string taskName_;

    int inputID_;
    int outputID_;

public:
    enum class Status { Pending, Running, Completed, Failed };
    Status status {ITask::Status::Pending};

    ITask() = delete;
    explicit ITask(const TaskID& taskID, const std::string& taskName, int inputID, int outputID) 
      : taskID_(taskID), taskName_(taskName), inputID_(inputID), outputID_(outputID) {}

    virtual ~ITask() = default;

    // Prepare the task for execution. This may involve loading
    virtual void prepare(const AttributeSPtr& attrs, const ContextSPtr& ctx) = 0;

    // Every task now gets both the scenario-wide Attributes **and**
    // the global Context loaded from JSON.
    virtual void run(const AttributeSPtr& attrs, const ContextSPtr& ctx) = 0;

    const TaskID& ID() const noexcept { return taskID_; }
    const int& inputID() const noexcept { return inputID_; }
    const int& outputID() const noexcept { return outputID_; }
    const std::string& name() const noexcept { return taskName_; }

    std::string statusString() const noexcept{
        switch (status) {
            case Status::Pending: return "Pending";
            case Status::Running: return "Running";
            case Status::Completed: return "Completed";
            case Status::Failed: return "Failed";
            default: return "Pending";
        }
    }

    void setStatusFromString(const std::string& statusStr) {
        if (statusStr == "Pending") {
            status = Status::Pending;
        } else if (statusStr == "Running") {
            status = Status::Running;
        } else if (statusStr == "Completed") {
            status = Status::Completed;
        } else if (statusStr == "Failed") {
            status = Status::Failed;
        }
        else {
            throw std::invalid_argument("Invalid status string: " + statusStr);
        }
    }
};

using TaskSPtr = std::shared_ptr<ITask>;

} // namespace eden
