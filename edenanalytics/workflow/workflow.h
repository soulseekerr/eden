#pragma once

#include "attributes.h"
#include "context.h"
#include "executor.h"
#include "itask.h"
#include <atomic>
#include <latch>
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

namespace eden {

class IWorkflow {
  virtual void run(IExecutor& executor) = 0;
};

/**
 * @brief Workflow class
 * @details This class is used to store the workflow composed of tasks and its dependencies
 */
class Workflow : public IWorkflow{
private:
    // Name of Workflow
    std::string name_;
    // Attributes and context
    AttributeSPtr attributes_;
    ContextSPtr context_;
    // Map of task IDs to task pointers
    std::unordered_map<TaskID, TaskSPtr> tasks_;
    // depoendencies at task ID level
    std::unordered_map<TaskID, std::vector<TaskID>> deps_;
    // node links (In - Out)
    std::unordered_map<TaskID, std::vector<std::pair<int, int>>> links_;

    std::unordered_map<TaskID, std::vector<TaskID>> children_;
    std::unordered_map<TaskID, std::atomic<std::size_t>> remaining_;

public:
  Workflow() = delete;
  Workflow(const Workflow&) = delete;
  Workflow(Workflow&&) = delete;
  Workflow& operator=(const Workflow&) = delete;
  Workflow& operator=(Workflow&&) = delete;
  ~Workflow() = default;

  // Constructor only valid with fullfillment of all parameters
  explicit Workflow(const std::string& name, const AttributeSPtr& attrs, const ContextSPtr& ctx)
    : name_(name), attributes_(std::move(attrs)), context_(std::move(ctx))
  {}

  // Run with any executor; each task sees the same attrs_ and ctx_.
  void run(IExecutor& executor) override;

  void exportGraphviz(std::ostream& os) const;
  void loadJsonFile(const std::string& jsonFile);

  [[nodiscard]] ITask::Status statusOf(const TaskID& id) const { return tasks_.at(id)->status; }
  [[nodiscard]] const std::string& name() const noexcept { return name_; }
  [[nodiscard]] const AttributeSPtr& attributes() const noexcept { return attributes_; }
  [[nodiscard]] const ContextSPtr& context() const noexcept { return context_; }
  [[nodiscard]] const std::unordered_map<TaskID, TaskSPtr>& tasks() const noexcept { return tasks_; }
  [[nodiscard]] const std::unordered_map<TaskID, std::vector<std::pair<int, int>>>& links() const noexcept { return links_; }
  [[nodiscard]] const std::unordered_map<TaskID, std::vector<TaskID>>& deps() const noexcept { return deps_; }
  
  Workflow& addTask(TaskID id, TaskSPtr task) {
    tasks_.emplace(id, task);
    return *this;
  }

  Workflow& dependsOn(const TaskID& after, const TaskID& before) {
    deps_[after].push_back(before);
    return *this;
  }
};

using WorkflowUPtr = std::unique_ptr<Workflow>;

} // namespace eden
