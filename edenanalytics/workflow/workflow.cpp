#include "workflow.h"
#include "task/fetchdatatask.h"
#include <stdexcept>
#include <algorithm>

#include <nlohmann/json.hpp>
// #include <imgui.h>
// #include <imnodes.h>
#include <fstream>


namespace eden {

// dot -Tpng workflow.dot -o workflow.png
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
  
    // TOFIX: wrong output in the dot file
    for (const auto& [after, befores] : deps_) {
        for (const auto& before : befores) {
            os << "  \"" << before << "\" -> \"" << after << "\";\n";
        }
    }
  
    os << "}\n";
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
