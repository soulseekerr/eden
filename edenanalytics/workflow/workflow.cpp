#include "workflow.h"
#include "task/fetchdatatask.h"
#include <stdexcept>
#include <algorithm>

namespace eden {

void Workflow::exportGraphviz(std::ostream& os) const {
    os << "digraph Workflow {\n";
    os << "  rankdir=LR;\n"; // left to right layout
  
    for (const auto& [id, task] : tasks_) {
        std::string color = "lightgray"; // default for waiting
  
        switch (task->status) {
            case ITask::Status::Pending:
                color = "blue"; break;
            case ITask::Status::Completed:
                color = "green"; break;
            case ITask::Status::Running:
                color = "orange"; break;
            case ITask::Status::Failed:
                color = "red"; break;
            default:
                break;
        }
  
        os << "  \"" << id << "\" [style=filled, fillcolor=" << color << "];\n";
    }
  
    for (const auto& [after, befores] : deps_) {
        for (const auto& before : befores) {
            os << "  \"" << before << "\" -> \"" << after << "\";\n";
        }
    }
  
    os << "}\n";
}

void Workflow::loadJsonFile(const std::string& jsonFile) {
    // Load the JSON file and parse it to set up tasks and dependencies
    std::ifstream file(jsonFile);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open JSON file: " + jsonFile);
    }
    nlohmann::json data;
    file >> data;

    const auto& tasks = data["tasks"];
    for (const auto& jsonTask : tasks) {

      auto taskType = jsonTask["type"].get<std::string>();
      auto taskID = jsonTask["id"].get<int>();
      auto taskName = jsonTask["name"].get<std::string>();
      auto status = jsonTask["status"].get<std::string>();
      auto inputID = jsonTask["input_id"].get<int>();
      auto outputID = jsonTask["output_id"].get<int>();
      
      TaskSPtr task;
      if (taskType == "CalibrationTask") {
        
        task = std::make_shared<FetchDataTask>(taskID, taskName, inputID, outputID);
   
      } else if (taskType == "FetchDataTask") {
        
        task = std::make_shared<FetchDataTask>(taskID, taskName, inputID, outputID);
   
      } else if (taskType == "ComputePVTask") {
        
        task = std::make_shared<FetchDataTask>(taskID, taskName, inputID, outputID);
   
      } else if (taskType == "SavePVTask") {
        
        task = std::make_shared<FetchDataTask>(taskID, taskName, inputID, outputID);
   
      } else {
        
        throw std::runtime_error("Unknown task type: " + taskType);
      }

      if (status == "Pending") {
        task->status = ITask::Status::Pending;
      } else if (status == "Running") {
        task->status = ITask::Status::Running;
      } else if (status == "Completed") {
        task->status = ITask::Status::Completed;
      } else if (status == "Failed") {
        task->status = ITask::Status::Failed;
      } else {
        throw std::runtime_error("Unknown status: " + status);
      }

      addTask(taskID, task);
    }

    const auto& deps = data["deps"];
    for (const auto& jsonDep : deps) {

        auto after = jsonDep["after"].get<int>();
        auto before = jsonDep["before"].get<int>();

        dependsOn(after, before);
    }

    const auto& links = data["node_links"];
    for (const auto& jsonLinks : links) {

        auto ID = jsonLinks["id"].get<TaskID>();
        auto from = jsonLinks["from"].get<int>();
        auto to = jsonLinks["to"].get<int>();

        links_[ID].emplace_back(from, to);
    }

    // for (const auto& [taskId, pairs] : links_) {
    //     std::cout << "Links for task: " << taskId << "\n";
    //     for (const auto& [from, to] : pairs) {
    //         std::cout << "  from: " << from << ", to: " << to << "\n";
    //     }
    // }

}

void Workflow::run(IExecutor& executor) {
    std::cout << ">>> Workflow::run() started\n";

    // 1) Build the dependency graph: children_ and remaining_
    children_.clear();
    remaining_.clear();

    // Initialize all task entries
    for (const auto& [id, _] : tasks_) {
        children_[id] = {};  // no children yet
        remaining_[id].store(0, std::memory_order_relaxed);  // no unresolved dependencies
    }

    // Process dependency links: populate children and count dependencies
    for (const auto& [after, befores] : deps_) {
        std::cout << after << " depends on: ";
        for (const auto& b : befores)
            std::cout << b << " ";
        std::cout << "\n";

        if (!tasks_.count(after)) throw std::runtime_error(std::format("Unknown task: {}", after));
        for (const auto& before : befores) {
            if (!tasks_.count(before)) throw std::runtime_error(std::format("Unknown task: {}", before));
            children_[before].push_back(after);  // 'before' must run before 'after'
            remaining_[after].fetch_add(1, std::memory_order_relaxed);  // increment dependency count
        }
    }

    // 2) Initialize a one-shot latch to track task completion
    std::latch done(tasks_.size());

    std::cout << "Workflow::run() - latch created -  tasks_ size: " << tasks_.size() << "\n";

    for (const auto& [id, rem] : remaining_) {
        std::cout << "Task " << id << " remaining deps: " << rem.load() << "\n";
    }

    
    // 3) Define a helper to schedule a task and its downstream dependents
    // tells the compiler exactly what the type of scheduleTask is, 
    // so it can compile a recursive reference to itself.
    std::function<void(const TaskID&)> scheduleTask = [&](const TaskID& id) {
        executor.enqueue([&, id] {
            tasks_.at(id)->run(attributes_, context_);
    
            for (const auto& child : children_[id]) {
                if (remaining_[child].fetch_sub(1, std::memory_order_acq_rel) == 1) {
                    std::cout << "Workflow::run() - scheduleTask: " << child << "\n";
                    scheduleTask(child);
                }
            }
    
            std::cout << "Workflow::run() - count_down\n";
            done.count_down();
        });
    };
    

    // 4) Kick off tasks that have no dependencies
    for (const auto& [id, _] : tasks_) {
        if (remaining_[id].load(std::memory_order_acquire) == 0) {
            std::cout << "Workflow::run() - scheduleTask no deps: " << id << "\n";
            scheduleTask(id);  // schedule immediately
        }
    }

    // 5) Wait for all tasks to finish
    done.wait();
}

} // namespace eden
