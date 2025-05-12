#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <ranges>
#include <algorithm>
#include <memory>
#include <cstdint>

namespace eden {

// Variant for field types, designed for performance and flexibility
using Field = std::variant<int32_t, double, std::string, bool>;

class Column {
public:
    std::string name;
    std::vector<Field> data;

    template <typename S>
    explicit Column(S&& name_) : name(std::forward<S>(name_)) {}

    void add(Field value) {
        data.push_back(std::move(value));
    }

    const Field& at(size_t index) const {
        return data.at(index);
    }

    Field& at(size_t index) {
        return data.at(index);
    }
};

class DataFrame {
    std::unordered_map<std::string, Column> columns;
    size_t row_count = 0;

public:
    void define_column(const std::string& name) {
        columns.emplace(name, Column{name});
    }

    void add_row(const std::unordered_map<std::string, Field>& row) {
        for (auto& [col_name, col] : columns) {
            auto it = row.find(col_name);
            if (it != row.end()) {
                col.add(it->second);
            } else {
                col.add(Field{}); // default-constructed variant
            }
        }
        ++row_count;
    }

    size_t size() const {
        return row_count;
    }

    void print_row(size_t index) const {
        for (const auto& [name, col] : columns) {
            std::cout << name << ": ";
            std::visit([](const auto& v) { std::cout << v; }, col.at(index));
            std::cout << "  ";
        }
        std::cout << "\n";
    }

    void print_all() const {
        for (size_t i = 0; i < row_count; ++i) {
            print_row(i);
        }
    }

    auto filter(auto predicate) const {
        std::vector<size_t> indices;
        for (size_t i = 0; i < row_count; ++i) {
            if (predicate(i)) {
                indices.push_back(i);
            }
        }
        return indices;
    }

    void print_filtered(const std::vector<size_t>& indices) const {
        for (size_t i : indices) {
            print_row(i);
        }
    }

    const auto& get_column(const std::string& name) const {
        return columns.at(name);
    }
};

} // namespace eden