
#include "workflowserializer.h"
#include "task/fetchdatatask.h"
#include <fstream>
#include <stdexcept>
#include <iomanip>

using namespace eden;

void WorkflowSerializer::save(const WorkflowUPtr& workflow, const std::string& jsonFile) {
    nlohmann::json data;
    data["workflowName"] = workflow->name();

    // Attributes (optional)
    // if (workflow.attributes()) {
    //     data["attributes"]["cob"] = workflow.attributes()->cob().yearMonthDayHyphen();
    // }

    // Tasks
    for (const auto& [id, task] : workflow->tasks()) {
        nlohmann::json t;
        t["id"] = id;
        t["name"] = task->name();
        t["type"] = "FetchDataTask"; // or task->type() if virtual
        t["status"] = task->statusString(); // "Pending", etc.
        t["input_id"] = task->inputID();
        t["output_id"] = task->outputID();
        data["tasks"].push_back(std::move(t));
    }

    // Dependencies
    for (const auto& [after, befores] : workflow->deps()) {
        for (int before : befores) {
            data["deps"].push_back({{"after", after}, {"before", before}});
        }
    }

    // Links
    for (const auto& [taskID, pairs] : workflow->links()) {
        for (const auto& [from, to] : pairs) {
            data["node_links"].push_back({{"id", taskID}, {"from", from}, {"to", to}});
        }
    }

    std::ofstream out(jsonFile);
    if (!out) throw std::runtime_error("Cannot open file: " + jsonFile);
    out << std::setw(4) << data;
}

void WorkflowSerializer::load(WorkflowUPtr& workflow, const std::string& jsonFile) {
    // Load the JSON file and parse it to set up tasks and dependencies
    std::ifstream in(jsonFile);
    if (!in) throw std::runtime_error("Cannot open file: " + jsonFile);
    
    nlohmann::json data;
    in >> data;

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

      task->setStatusFromString(status);

      workflow->addTask(taskID, task);
    }

    const auto& deps = data["deps"];
    for (const auto& jsonDep : deps) {

        auto after = jsonDep["after"].get<int>();
        auto before = jsonDep["before"].get<int>();

        workflow->dependsOn(after, before);
    }

    const auto& links = data["node_links"];
    for (const auto& jsonLinks : links) {

        auto ID = jsonLinks["id"].get<TaskID>();
        auto from = jsonLinks["from"].get<int>();
        auto to = jsonLinks["to"].get<int>();

        workflow->addLink(ID, from, to);
    }

    // for (const auto& [taskId, pairs] : workflow->links()) {
    //     std::cout << "Links for task: " << taskId << "\n";
    //     for (const auto& [from, to] : pairs) {
    //         std::cout << "  from: " << from << ", to: " << to << "\n";
    //     }
    // }
}
