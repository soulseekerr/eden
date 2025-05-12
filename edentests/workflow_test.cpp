#include <gtest/gtest.h>
#include "workflow/workflow.h"
#include "workflow/workflowserializer.h"
#include "task/fetchdatatask.h"
#include "attributes.h"
#include "context.h"
#include "datetime.h"
#include <filesystem>
#include <fstream>

TEST(WorkflowTest, AddAndRetrieveTask) {
    auto cob = eden::DateTime(2024, 6, 3);
    // Attributes cannot be modified further but copied among other task instances
    const eden::AttributeSPtr& attr = std::make_shared<eden::Attributes>(cob);
    const eden::ContextSPtr& ctx = std::make_shared<eden::TaskContext>();

    auto wf = std::make_unique<eden::Workflow>("TestFlow", attr, ctx);

    auto task143 = std::make_shared<eden::FetchDataTask>(1, "Fetch1", 10, 11);
    auto task255 = std::make_shared<eden::FetchDataTask>(1, "Fetch2", 20, 21);
    wf->addTask(143, task143);
    wf->addTask(255, task255);

    auto retrieved = wf->tasks().at(143);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->name(), "Fetch1");
    EXPECT_EQ(retrieved->inputID(), 10);
    EXPECT_EQ(retrieved->outputID(), 11);

    retrieved = wf->tasks().at(255);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->name(), "Fetch2");
    EXPECT_EQ(retrieved->inputID(), 20);
    EXPECT_EQ(retrieved->outputID(), 21);
}

TEST(WorkflowTest, AddDependency) {
    auto cob = eden::DateTime(2024, 6, 3);
    // Attributes cannot be modified further but copied among other task instances
    const eden::AttributeSPtr& attr = std::make_shared<eden::Attributes>(cob);
    const eden::ContextSPtr& ctx = std::make_shared<eden::TaskContext>();

    auto wf = std::make_unique<eden::Workflow>("TestFlow", attr, ctx);

    auto task143 = std::make_shared<eden::FetchDataTask>(1, "Fetch1", 10, 11);
    auto task255 = std::make_shared<eden::FetchDataTask>(1, "Fetch2", 20, 21);
    wf->addTask(143, task143);
    wf->addTask(255, task255);

    wf->dependsOn(255, 143);  // T2 depends on T1

    const auto& deps = wf->deps();
    ASSERT_EQ(deps.at(255).size(), 1);
    EXPECT_EQ(deps.at(255)[0], 143);
}

TEST(WorkflowTest, SaveAndLoadJson) {
    auto cob = eden::DateTime(2024, 6, 3);
    // Attributes cannot be modified further but copied among other task instances
    const eden::AttributeSPtr& attr = std::make_shared<eden::Attributes>(cob);
    const eden::ContextSPtr& ctx = std::make_shared<eden::TaskContext>();

    auto wf = std::make_unique<eden::Workflow>("FlowRoundTrip", attr, ctx);

    auto task1 = std::make_shared<eden::FetchDataTask>(1, "TaskA", 100, 101);
    auto task2 = std::make_shared<eden::FetchDataTask>(2, "TaskB", 102, 103);
    wf->addTask(1, task1);
    wf->addTask(2, task2);

    wf->dependsOn(2, 1);  // T2 depends on T1
    wf->addLink(1, 101, 102);

    std::string path = "test_workflow.json";
    auto wfSerialiser = std::make_unique<eden::WorkflowSerializer>();
    wfSerialiser->save(wf, path);

    auto loaded = std::make_unique<eden::Workflow>("EmptyFlow", attr, ctx);
    wfSerialiser->load(loaded, path);

    EXPECT_EQ(loaded->name(), "EmptyFlow");
    EXPECT_NE(loaded->tasks().at(1), nullptr);
    EXPECT_NE(loaded->tasks().at(2), nullptr);
    EXPECT_EQ(loaded->deps().at(2).at(0), 1);
    EXPECT_EQ(loaded->links().at(1).at(0).first, 101);
    EXPECT_EQ(loaded->links().at(1).at(0).second, 102);

    std::filesystem::remove(path);  // clean up
}
