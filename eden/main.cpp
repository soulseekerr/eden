
#include "ver.h"
#include "core/edenanalytics.h"
#include "logger.h"
#include "exceptions.h"
#include "time_elapsed.h"
#include "datetime.h"
#include "attributes.h"
#include "task/scenariotask.h"
#include "models/hullwhite_1factor.h"
#include "task/computepresentvalue.h"
#include "task/computetest.h"

#include "concurrency.h"
#include "threadpool.h"
#include "workflow/workflow.h"
#include "task/fetchdatatask.h" 
#include "pathmanager.h"
#
#include <sstream>
#include <format>
#include <string>
#include <fmt/core.h>
#include <iostream>
#include <format>
#include <memory>

using namespace eden;


class Factory {
public:
    static WorkflowUPtr createWorkflow(const std::string& name, const AttributeSPtr& attributes, ContextSPtr& context, const std::string& jsonFile) {
        WorkflowUPtr wf = std::make_unique<Workflow>(name, attributes, context);
        wf->loadJsonFile(jsonFile);
        return wf;
    }

    static TaskSPtr createTask(int taskID, const std::string& taskType, const AttributeSPtr& attributes, ContextSPtr& context) {
        if (taskType == "FetchDataTask") {
             TaskSPtr task = std::make_shared<FetchDataTask>(taskID, "FetchDataTask", taskID, taskID + 1);
             task->prepare(attributes, context);
             return task;
        } else if (taskType == "CalibrationTask") {
            TaskSPtr task = std::make_shared<FetchDataTask>(taskID, "FetchDataTask", taskID, taskID + 1);
             task->prepare(attributes, context);
             return task;
        } else if (taskType == "ComputePVTask") {
            TaskSPtr task = std::make_shared<FetchDataTask>(taskID, "FetchDataTask", taskID, taskID + 1);
             task->prepare(attributes, context);
             return task;
        } else if (taskType == "SavePVTask") {
            TaskSPtr task = std::make_shared<FetchDataTask>(taskID, "FetchDataTask", taskID, taskID + 1);
            task->prepare(attributes, context);
            return task;
        }
        throw std::runtime_error("Unknown task type: " + taskType);
    }
};

std::string getJsonFile(PathManager& pm, const EdenFileType& fileType, const std::string& filename) {
    auto path = pm.getFilePath(fileType, filename);
    if (!path) {
        std::string label;
        switch (fileType) {
            case EdenFileType::Workflow: label = "Workflow"; break;
            case EdenFileType::Config: label = "Config"; break;
            default: label = "Unknown"; break;
        }
        throw MissingFileException(label, filename);
    }
    return path->string();
}

int main() {
    std::string logName = "EdenLog";
    fmt::print("Version: {}", getVersion());  // test fmt library
    fmt::print("Version: {}", eden::analytics::getVersion());

    // Logs
    auto& log = LoggerManager::getInstance();
    log.addLogger(std::make_unique<LoggerConsole>(LOG_LEVEL::LOG_DEBUG));
    std::string logFile = std::format("{}_{}.log", logName, sTimeElapsed::yearMonthDayHyphen());
    log.addLogger(std::make_unique<LoggerFile>(LOG_LEVEL::LOG_DEBUG, logFile));

    log.logInfo("Starting log - {}", DateTime().toString());
    log.logInfo("Version EdenCore: {}", getVersion());
    log.logInfo("Version EdenAnalytics: {}", eden::analytics::getVersion());

    auto& pathManager = PathManager::getInstance();

    // Inputs of the binary - harcoded here
    auto cob = DateTime(2024, 6, 3);
    // Attributes cannot be modified further but copied among other task instances
    const AttributeSPtr& attr = std::make_shared<Attributes>(cob);
    log.log("COB: {}", attr->cob().yearMonthDayHyphen());

    ContextSPtr wfContext;
    WorkflowUPtr wf;

    try {
        const auto workflowFile = getJsonFile(pathManager, EdenFileType::Workflow, "workflow_test.json");
        log.logInfo("Workflow file: {}", workflowFile);
    
        const auto contextFile = getJsonFile(pathManager, EdenFileType::Workflow, "context_test.json");
        log.logInfo("Context file: {}", contextFile);

        wfContext = std::make_shared<JsonContext>(contextFile);

        log.logInfo("Context  successfully created");

        wf = Factory::createWorkflow("MyFlow", attr, wfContext, workflowFile);

    } catch (const MissingFileException& e) {
        log.logError("Missing file: {}", e.what());
        return -1;
    }

    log.logInfo("Workflow successfully created");
    log.logInfo("Workflow name: {} - COB: {}", wf->name(), wf->attributes()->cob().yearMonthDayHyphen());
    
    ContextSPtr taskContext = std::make_shared<TaskContext>();

    wf->addTask(1, Factory::createTask(1, "CalibrationTask", attr, taskContext))
      .addTask(2, Factory::createTask(2, "ComputePVTask", attr, taskContext))
      .addTask(3, Factory::createTask(3, "ComputePVTask", attr, taskContext))
      .addTask(4, Factory::createTask(4, "ComputePVTask", attr, taskContext))
      .addTask(5, Factory::createTask(5, "SavePVTask", attr, taskContext))
      .addTask(6, Factory::createTask(6, "SavePVTask", attr, taskContext))
      .addTask(7, Factory::createTask(7, "SavePVTask", attr, taskContext))
      .addTask(8, Factory::createTask(8, "FetchDataTask", attr, taskContext))
      .dependsOn(2, 1)
      .dependsOn(3, 1)
      .dependsOn(4, 1)
      .dependsOn(5, 2)
      .dependsOn(6, 3)
      .dependsOn(7, 4)
      .dependsOn(8, 5)
      .dependsOn(8, 6)
      .dependsOn(8, 7);

    std::ofstream out("workflow.dot");
    wf->exportGraphviz(out);
    out.close();

    // 4) Decide thread count
    HardwareConcurrencyProvider provider;

    // 5) Construct pool directly from provider
    ThreadPool pool{provider};

    // 6) Run your workflow
    wf->run(pool);

    if (wf->statusOf(2) == ITask::Status::Completed) {
        std::cout << "Workflow succeeded\n";
    }

    return 0;
}