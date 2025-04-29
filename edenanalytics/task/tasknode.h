#pragma once

#include <string>
#include <memory>
#include <vector>
#include <format>
#include <memory>

#include "attributes.h"

namespace eden {

/**
 * @brief ITasks interface
 * @details This class is used to define the interface for tasks
 * 
 */
struct ITaskNode {
    virtual ~ITaskNode() = default;
    virtual Response execute() = 0;
};

/**
 * @brief TaskNode class
 * @details Abstract class for tasks
 */
class TaskNode : public ITaskNode {
private:
    std::shared_ptr<Attributes> attr_;

public:
    TaskNode(std::shared_ptr<Attributes> attr) : attr_(attr) {}
    virtual ~TaskNode() {}

    virtual Response execute() = 0;

    const std::shared_ptr<Attributes>& attributes() const { return attr_; }
};

/**
 * @brief ITaskNodeIterator interface
 * @details Iterator Pattern for iteration over TaskNodes
 * abstract interface for iterating over a collection of elements
 */
template <typename T>
struct ITaskNodeIterator {
    virtual ~ITaskNodeIterator() = default;
    virtual bool hasNext() const = 0;
    virtual bool hasPrev() const = 0;
    virtual T next() = 0;
    virtual T prev() = 0;
};

/**
 * @brief ITaskNodeAggregate interface
 * @details Aggregate Pattern for TaskNodes
 * interface for collections (or containers) that can create an iterator to traverse their elements
 */
template <typename T>
struct ITaskNodeAggregate {
    virtual ~ITaskNodeAggregate() = default;
    virtual std::unique_ptr<ITaskNodeIterator<T>> createIterator() const = 0;
};

/**
 * @brief TaskNodeIterator class
 * @details Concrete Implementation of the iterator
 */
class TaskNodeIterator : public ITaskNodeIterator<std::shared_ptr<ITaskNode>> {
private:
    const std::vector<std::shared_ptr<ITaskNode>>& data;
    size_t index;

public:
    explicit TaskNodeIterator(const std::vector<std::shared_ptr<ITaskNode>>& nodes) 
        : data(nodes), index(0) {}

    bool hasNext() const override {
        return index < data.size();
    }

    bool hasPrev() const override {
        return index > 0;
    }

    std::shared_ptr<ITaskNode> next() override {
        if (!hasNext()) throw std::out_of_range("No more elements");
        return data[index++];
    }

    std::shared_ptr<ITaskNode> prev() override {
        if (!hasPrev()) throw std::out_of_range("No previous elements");
        return data[--index];
    }
};

/**
 * @brief TaskNodeList class
 * @details Concrete Implementation of the aggregate
 */
template <typename T>
class TaskNodeList : public ITaskNodeAggregate<std::shared_ptr<ITaskNode>> {
private:
    std::vector<std::shared_ptr<ITaskNode>> tasks_;

public:
    TaskNodeList() = default;
    virtual ~TaskNodeList() = default;

    void addTask(std::shared_ptr<ITaskNode> task) {
        tasks_.push_back(std::move(task));
    }

    std::unique_ptr<ITaskNodeIterator<std::shared_ptr<ITaskNode>>> createIterator() const override {
        return std::make_unique<TaskNodeIterator>(tasks_);
    }
};

} // namespace eden