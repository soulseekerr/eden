
#include "ver.h"
#include "hello.h"
#include "logger.h"
#include "time_elapsed.h"
#include "datetime.h"
#include "data_attributes.h"
#include "task/taskparameters.h"
#include "models/hullwhite_1factor.h"
#include "task/computepresentvalue.h"
#include "task/computecva.h"

#include <sstream>
#include <format>
#include <string>
#include <fmt/core.h>
#include <iostream>
#include <format>
#include <memory>

using namespace eden;

/**
 * Return EDEN version.
 *
 */
static const std::string getVersion()
{
	std::ostringstream lFlux;
	lFlux << "EDEN Core v" << EDEN_CORE_MAJOR_VERSION << "." << EDEN_CORE_MINOR_VERSION << "." << EDEN_CORE_REVISION_VERSION << std::endl;
	return lFlux.str();
}

// class MyService {
// public:
//     MyService(std::unique_ptr<Logger)
// };

int main() {
    // Test fmt library
    fmt::print("Version: {}", getVersion());

    st_datetime nowdt;
    auto logFile = std::format("eden_logger_{}.log", nowdt.year_month_day_h());

    // Log Manager
    auto& logManager = LoggerManager::getInstance();
    logManager.addLogger(std::make_unique<LoggerConsole>(LOG_LEVEL::LOG_DEBUG));
    logManager.addLogger(std::make_unique<LoggerFile>(LOG_LEVEL::LOG_DEBUG, logFile));

    logManager.logInfo("Starting log - {}", DateTime().toString());

    auto cob = DateTime(2024, 6, 3);
    const auto params = std::make_shared<DataAttributes>(cob);

    logManager.log("COB: {}", params->cob().year_month_day_h());

    std::vector<std::shared_ptr<ITaskNode>> tasks;
    
    auto task1 = std::make_shared<ComputePresentValue>(params);
    auto task2 = std::make_shared<SavePresentValue>(params);
    
    const auto taskParams = std::make_shared<TaskParameters>(cob, "Base");
    auto task3 = std::make_shared<ComputeCVA>(taskParams);
    
    tasks.push_back(task1);
    tasks.push_back(task2);
    tasks.push_back(task3);

    std::cout << "Tasks loop:" << std::endl;
    for (auto t = 1; t <= tasks.size(); t++) {
        std::cout << "Task " << t << "/" << tasks.size() << std::endl;
        tasks[t-1]->execute();
    }

    // eden::VectorTaskNodeAggregate aggregate(tasks);

    // eden::VectorTaskNodeIterator taskIterator(tasks);

    // std::cout << "Tasks iterator:" << std::endl;
    // auto iterator = aggregate.createIterator();

    // auto bHasNext = iterator->hasNext();
    // if (!bHasNext) {
    //     std::cout << "Unexpected Error: no next instance in iterator" << std::endl;
    //     return -1;
    // }

    // auto addr = iterator->next();
    // std::cout << "Next Instance: " << addr << " ";
    // addr->execute();

    // auto bHasPrev = iterator->hasPrev();
    // if (!bHasPrev) {
    //     std::cout << "Unexpected Error: no prev instance in iterator" << std::endl;
    //     return -1;
    // }

    // addr = iterator->prev();
    // std::cout << "Prev Instance: " << addr << " ";
    // addr->execute();

    // addr = iterator->next();
    // std::cout << "Next Instance: " << addr << " ";
    // addr->execute();

    // addr = iterator->next();
    // std::cout << "Next Instance: " << addr << " ";
    // addr->execute();

    // std::cout << std::endl;

    logManager.logInfo("Ending log - {}", DateTime().toString());

    return 0;
}