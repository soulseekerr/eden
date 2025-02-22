#pragma once

#include <string>
#include <memory>
#include <vector>
#include <format>
#include <memory>

#include "attributes.h"

namespace eden {

enum class Status {
    OK = 1,
    ERROR = -1 
};

struct Response {
    Status status;
    std::string msg;
    Response(const Status s, const std::string& m) : status(s), msg(m) {}
    virtual ~Response() {}
};

struct ITaskNode {
    virtual Response execute() = 0;
};

class ATaskNode : public ITaskNode {
public:
    ATaskNode(const std::shared_ptr<Attributes>& p) : params_(p) {}
    virtual ~ATaskNode() {}

    virtual Response execute() = 0;

    const std::shared_ptr<Attributes>& params() const { return params_; }

private:
    std::shared_ptr<Attributes> params_;
};

template <typename T>
struct ITaskNodeIterator {
    virtual ~ITaskNodeIterator() = default;
    virtual bool hasNext() const = 0;
    virtual bool hasPrev() const = 0;
    virtual T next() = 0;
    virtual T prev() = 0;
};

template <typename T>
struct ITaskNodeAggregate {
    virtual ~ITaskNodeAggregate() = default;
    virtual std::unique_ptr<ITaskNodeIterator<T>> createIterator() const = 0;
};

// class VectorTaskNodeIterator : public ITaskNodeIterator<std::shared_ptr<ITaskNode>> {
// public:
//     VectorTaskNodeIterator(const std::vector<std::shared_ptr<ITaskNode>>& data)
//         : data_(data), index_(0) {}

//     bool hasNext() const override {
//         return index_ < data_.size();
//     }

//     bool hasPrev() const override {
//         return index_ >= 0;
//     }

//     std::shared_ptr<ITaskNode>& next() override {
//         if (!hasNext()) {
//             throw std::out_of_range("No more elements");
//         }
//         return data_[index_++];
//     }

//     std::shared_ptr<ITaskNode>& prev() override {
//         if (!hasPrev()) {
//             throw std::out_of_range("No prev elements");
//         }
//         return data_[index_--];
//     }

// private:
//     const std::vector<std::shared_ptr<ITaskNode>>& data_;
//     size_t index_;
// };

// template <typename T>
// struct VectorTaskNodeAggregate : public ITaskNodeAggregate<std::shared_ptr<ITaskNode>> {
//     VectorTaskNodeAggregate(const std::vector<std::shared_ptr<ITaskNode>>& data) : data_(data) {}

//     virtual ~VectorTaskNodeAggregate() = default;
    
//     std::unique_ptr<ITaskNodeIterator<std::shared_ptr<ITaskNode>>> createIterator() const override {
//         return std::make_unique<VectorTaskNodeIterator<std::shared_ptr<ITaskNode>>>(data_);
//     }

// private:
//     const std::vector<std::shared_ptr<ITaskNode>>& data_;
// };

} // namespace eden