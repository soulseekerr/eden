#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>
#include <string>

namespace eden {

class IContext {
public:
    IContext() = default;
    virtual ~IContext() = default;

    /// Query any key from the root object. Throws if missing or wrong type.
    template<typename T>
    T get(const std::string& /*key*/) const {
        throw std::runtime_error("get() is not implemented!");
    }
};

class TaskContext : public IContext {
public:
    TaskContext() = default;
    virtual ~TaskContext() = default;

    /// Query any key from the root object. Throws if missing or wrong type.
    template<typename T>
    T get(const std::string& /*key*/) const {
        return "no key";
    }
};

/// Holds arbitrary configuration loaded once from a JSON file.
class JsonContext : public IContext {
private:
    nlohmann::json data_;

public:
    JsonContext() = default;
    virtual ~JsonContext() = default;

    /// Load the entire JSON config at startup.
    explicit JsonContext(const std::string& filepath) {
        std::ifstream in{filepath};
        if (!in) 
            throw std::runtime_error("Cannot open config file: " + filepath);
        in >> data_;
    }

    /// Query any key from the root object. Throws if missing or wrong type.
    template<typename T>
    T get(const std::string& key) const {
        if (!data_.contains(key))
            throw std::runtime_error("Context missing key: " + key);
        return data_.at(key).get<T>();
    }

    /// Access the raw JSON if you need nested lookups.
    const nlohmann::json& raw() const noexcept { return data_; }
};

using ContextSPtr = std::shared_ptr<IContext>;

} // namespace eden
