// fetch_data_task.h
#pragma once

#include "itask.h"
#include <iostream>

namespace eden {

/**
 * @brief FetchDataTask is a task that fetches data from a given URL.
 * It uses the Context to get the URL and timeout parameters.
 * The task can be run in a separate thread and its status can be checked.
 * @details
 * - The task is initialized with a status of Pending.
 * - When run, it changes the status to Running.
 * - If the fetch is successful, the status changes to Completed.
 * - If an error occurs, the status changes to Failed.
 * - The task uses the Context to get the URL and timeout parameters.
 * - The URL and timeout are expected to be present in the Context.
 */
class FetchDataTask : public ITask {
public:
    FetchDataTask() = delete;
    explicit FetchDataTask(const TaskID& taskID, const std::string& taskName, int inpuID, int outputID) 
        : ITask(taskID, taskName, inpuID, outputID) {}

    ~FetchDataTask() override = default;

    void prepare(const AttributeSPtr& /*attrs*/, const ContextSPtr& /*ctx*/) override {
        // Prepare the task for execution. This may involve loading resources or initializing variables.
        // status_ = Status::Pending;
        std::cout << "Preparing FetchDataTask...\n";
    }

    void run(const AttributeSPtr& /*attr*/, const ContextSPtr& /*ctx*/) override {
        // status_ = Status::Running;
        try {
            // all parameters live in the shared JSON root
            // auto url     = ctx->get<std::string>("url");
            // auto timeout = ctx->get<int>("timeoutSeconds");

            // std::cout << "Fetching " << url << " timeout=" << timeout << "\n";
            std::cout << "Fetching data...\n";
            
            // status_ = Status::Completed;
        }
        catch (...) {
            // status_ = Status::Failed;
        }
    }
};

} // namespace eden
