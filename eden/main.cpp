
#include "ver.h"
#include "core/edenanalytics.h"
#include "logger.h"
#include "time_elapsed.h"
#include "datetime.h"
#include "attributes.h"
#include "task/scenariotask.h"
#include "models/hullwhite_1factor.h"
#include "task/computepresentvalue.h"
#include "task/computetest.h"

#include <sstream>
#include <format>
#include <string>
#include <fmt/core.h>
#include <iostream>
#include <format>
#include <memory>

using namespace eden;


int main() {
    // Test fmt library
    fmt::print("Version: {}", getVersion());
    fmt::print("Version: {}", eden::analytics::getVersion());

    st_datetime nowdt;
    auto logFile = std::format("eden_logger_{}.log", nowdt.year_month_day_h());

    // Log Manager
    auto& logManager = LoggerManager::getInstance();
    logManager.addLogger(std::make_unique<LoggerConsole>(LOG_LEVEL::LOG_DEBUG));
    logManager.addLogger(std::make_unique<LoggerFile>(LOG_LEVEL::LOG_DEBUG, logFile));

    logManager.logInfo("Starting log - {}", DateTime().toString());

    auto cob = DateTime(2024, 6, 3);
    const auto params = std::make_shared<Attributes>(cob);

    logManager.log("COB: {}", params->cob().year_month_day_h());

    std::vector<std::shared_ptr<ITask>> tasks;
    
    auto task1 = std::make_shared<ComputePresentValue>(params);
    auto task2 = std::make_shared<SavePresentValue>(params);
    
    auto taskParams = std::make_shared<ScenarioTask>(cob, "Base");
    auto task3 = std::make_shared<ComputeTest>(taskParams);
    
    tasks.push_back(task1);
    tasks.push_back(task2);
    tasks.push_back(task3);

    std::cout << "Tasks loop:" << std::endl;
    for (auto t = 1; t <= tasks.size(); t++) {
        std::cout << "Task " << t << "/" << tasks.size() << std::endl;
        tasks[t-1]->execute();
    }

    return 0;
}