
#include "tasknode.h"

class DummyTask : public eden::TaskNode {
public:
    DummyTask(std::shared_ptr<eden::Attributes> attr) : TaskNode(attr) {}
    virtual ~DummyTask() {}

    eden::Response execute() override {
        const auto& cob = attributes()->cob();
        std::cout << "DummyTask executed: " << cob.year_month_day_h() << std::endl;
        return eden::resp::success;
    }
};

void example_tasknode_iterator() {
    // Create a shared attribute object
    auto attr = std::make_shared<eden::Attributes>(eden::DateTime(2021, 1, 1));
    auto task = std::make_shared<DummyTask>(attr);

    // Create task list
    eden::TaskNodeList<std::shared_ptr<eden::ITask>> taskList;

    taskList.addTask(std::make_shared<DummyTask>(attr));
    taskList.addTask(std::make_shared<DummyTask>(attr));
        
    auto it = taskList.createIterator();
    while (it->hasNext()) {
        it->next()->execute();
    }
}
