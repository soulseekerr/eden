
#include "ver.h"
#include "core/edenanalytics.h"
#include "hello.h"
#include "logger.h"
#include "time_elapsed.h"
#include "datetime.h"
#include "data_attributes.h"
#include "task/taskparameters.h"
#include "models/hullwhite_1factor.h"
#include "task/computepresentvalue.h"
#include "task/computecva.h"
#include "sort.h"
#include "search.h"
#include "dfs.h"
#include "randomnumbers.h"
#include "redblacktree.h"

#include <sstream>
#include <format>
#include <string>
#include <fmt/core.h>
#include <iostream>
#include <format>
#include <memory>

using namespace eden;
using namespace edentree;

// class MyService {
// public:
//     MyService(std::unique_ptr<Logger)
// };

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

    heapExample();

    logManager.log("Testing Sorting Algorithms:");
    std::vector<int> arr = {10, 7, 8, 9, 1, 5, 45, 71, 4, 2};
    bubbleSortExample(arr, true);
    selectionSortExample(arr, true);
    mergeSortExample(arr, true);
    quickSortExample(arr, true);

    // Generate num random numbers between min and max
    int num = 10000;
    int min = 1;
    int max = 1000;
    std::vector<int> randomNumbers = generateRandomNumbers(num, min, max);
    bubbleSortExample(randomNumbers, false);
    selectionSortExample(randomNumbers, false);
    mergeSortExample(randomNumbers, false);
    quickSortExample(randomNumbers, false);

    // Generate num random numbers between min and max
    // num = 10000;
    // min = 1;
    // max = 1000;
    // randomNumbers = generateRandomNumbers(num, min, max);
    // bubbleSortExample(randomNumbers, false);
    // selectionSortExample(randomNumbers, false);
    // mergeSortExample(randomNumbers, false);
    // quickSortExample(randomNumbers, false);

    logManager.log("RedBlackTree:");
    // Red Black Tree
    RedBlackTree rbTree;

    rbTree.insert(10);
    rbTree.insert(20);
    rbTree.insert(1);
    rbTree.insert(3);
    rbTree.insert(8);
    rbTree.insert(99);
    rbTree.insert(3);
    rbTree.insert(6);
    rbTree.insert(13);

    std::cout << "Inorder traversal of the Red-Black Tree:";
    rbTree.inorder();
    std::cout << std::endl;

    logManager.log("Search:");

    int arr_search[] = {10, 23, 45, 70, 11, 15};
    int size = 6;
    int target = 70;
    linearSearchExample<int>(arr_search, size, target);

    int arr_binarysearch[] = {2, 3, 4, 10, 40};
    size = 5;
    target = 10;
    binarySearchExample(arr_binarysearch, size, target);

    logManager.log("DFS Example:");
    dfsExample();

    logManager.logInfo("Ending log - {}", DateTime().toString());

    return 0;
}