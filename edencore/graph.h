#pragma once

// graph node
// graph iterator

#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace eden {
namespace analytics {

struct ITreeNodeDoStuff {
    virtual int DoStuff() = 0;
    virtual int DoOtherStuff() = 0;
};

// TODO add compostion with Strategies = computation task 
class TreeNode : public ITreeNodeDoStuff {
public:
    TreeNode(const std::string& name) : name_(name) {}
    virtual ~TreeNode() {}

    const std::string& getName() const { return name_; }

    int DoStuff() override {
        std::cout << "DoStuff" << std::endl;
        return 0;
    }

    int DoOtherStuff() override {
        std::cout << "DoOtherStuff" << std::endl;
        return 0;
    }

private:
    std::string name_;
};

// Iterator interface
template <typename T>
struct TreeNodeIterator {
    virtual ~TreeNodeIterator() = default;
    virtual bool hasNext() const = 0;
    virtual T next() = 0;
    // virtual bool hasPrev() const = 0;
    // virtual T prev() = 0;
};

template <typename T>
struct TreeAggregate {
    virtual ~TreeAggregate() = default;
    virtual std::unique_ptr<TreeNodeIterator<T>> createIterator() const = 0;
};

template <typename T>
class VectorTaskIterator : public TreeNodeIterator<T> {
public:
    VectorTaskIterator(const std::vector<T>& data)
        : data_(data), index_(0) {}

    bool hasNext() const override {
        return index_ < data_.size();
    }

    T next() override {
        if (!hasNext()) {
            throw std::out_of_range("No more elements");
        }
        return data_[index_++];
    }

private:
    const std::vector<T>& data_;
    size_t index_;
};

template <typename T>
class VectorTaskAggregate : public TreeAggregate<T> {
public:
    VectorTaskAggregate(const std::vector<T>& data) : data_(data) {}

    std::unique_ptr<TreeNodeIterator<T>> createIterator() const override {
        return std::make_unique<VectorTaskIterator<T>>(data_);
    }

private:
    std::vector<T> data_;
};

} // namespace analytics
} // namespace eden