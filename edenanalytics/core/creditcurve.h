#pragma once

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include <format>
#include <nlohmann/json.hpp>
#include <optional>
#include <algorithm>
#include <unordered_map>
#include <iostream>

#include "datetime.h"
#include "utils.h"

using json = nlohmann::json;

// Example usage
int exampleCreditCurve();
int exampleCreditCurve2();

namespace eden {

// Define Tenor enum
enum class Tenor { T0M, T1M, T3M, T6M, T1Y, T2Y, T3Y, T5Y, T7Y, T10Y, T20Y, T30Y };

// std::unordered_map is a hash table-based associative container in C++.
// std::map is implemented as a balanced binary search tree (typically a red-black tree).
// std::unordered_map provides O(1) average-time complexity for lookups, insertions, and deletions.
// compared to O(log N) for std::map.

// Mapping tenor strings to enum values
const std::unordered_map<std::string, Tenor> StringToTenor = {
    { "T0M", Tenor::T0M }, { "T3M", Tenor::T3M }, { "T6M", Tenor::T6M },
    { "T1Y", Tenor::T1Y }, { "T2Y", Tenor::T2Y }, { "T3Y", Tenor::T3Y }, { "T5Y", Tenor::T5Y },
    {"T7Y", Tenor::T7Y}, { "T10Y", Tenor::T10Y }, { "T20Y", Tenor::T20Y }, { "T30Y", Tenor::T30Y }
};

// Mapping tenors to their string representations
const std::unordered_map<Tenor, std::string> TenorToString = {
    { Tenor::T0M, "T0M" }, { Tenor::T3M, "T3M" }, { Tenor::T6M, "T6M" },
    { Tenor::T1Y, "T1Y" }, { Tenor::T2Y, "T2Y" }, { Tenor::T3Y, "T3Y" }, 
    { Tenor::T5Y, "T5Y" }, { Tenor::T7Y, "T7Y" }, { Tenor::T10Y, "T10Y" },
    { Tenor::T20Y, "T20Y" }, { Tenor::T30Y, "T30Y" }
};

// Mapping tenors to their maturity in years
const std::unordered_map<Tenor, double> TenorMaturities = {
    { Tenor::T0M,  1.0 / 365 }, { Tenor::T3M,  3.0 / 12 },
    { Tenor::T6M,  6.0 / 12 }, { Tenor::T1Y,  1.0 },
    { Tenor::T2Y,  2.0 },      { Tenor::T3Y,  3.0 },
    { Tenor::T5Y,  5.0 },      { Tenor::T7Y,  7.0 }, { Tenor::T10Y, 10.0 },
    { Tenor::T20Y, 20.0 },     { Tenor::T30Y, 30.0 }
};


// @brief CreditCurveLoaderJSON
// @details Class that loads a credit curve from a JSON file
//
class CreditCurveLoaderJSON {
public:
    static json loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open JSON file");
        }
    
        json jsonData;
        file >> jsonData;
    
        return jsonData;
    }
};


namespace V1 {

// @brief CreditCurve class
// @details CreditCurve class is a class that represents a credit curve
//
class CreditCurve {
private:
    std::map<Tenor, double> data_;

public:
    explicit CreditCurve(const std::map<Tenor, double>&& data) : data_(std::move(data)) {}

    // Get spread for a given tenor, with optional interpolation
    double getSpread(const Tenor& tenor, bool allowInterp = false) const {
        auto it = data_.find(tenor);
        if (it != data_.end()) {
            return it->second;
        }

        if (allowInterp) {
            return interpolateSpread(tenor);
        }

        throw std::invalid_argument("Spread not found for requested tenor.");
    }

    double getMaturity(Tenor tenor) const {
        auto it = TenorMaturities.find(tenor);
        if (it == TenorMaturities.end())
            throw std::invalid_argument(std::format("Unknown tenor {}", static_cast<int>(tenor)));
        return it->second;
    }

    bool hasTenor(const Tenor& tenor) const {
        return data_.find(tenor) != data_.end();
    }

    // Save CreditCurve to JSON
    json toJson() const {
        json j;
        for (const auto& [tenor, spread] : data_) {
            // Assuming you want to store the string representation of the tenor
            std::string tenorStr = TenorToString.at(tenor); 
            j[tenorStr] = spread;
        }
        return j;
    }

private:
    // Linear interpolation between two known tenors
    double interpolateSpread(const Tenor& tenor) const {
        if (data_.empty()) {
            throw std::invalid_argument("Cannot interpolate in an empty curve.");
        }

        auto upper = data_.upper_bound(tenor);
        if (upper == data_.begin() || upper == data_.end()) {
            throw std::invalid_argument("Cannot interpolate: Tenor out of range.");
        }

        auto lower = std::prev(upper);
        double t1 = TenorMaturities.at(lower->first);
        double t2 = TenorMaturities.at(upper->first);
        double s1 = lower->second;
        double s2 = upper->second;
        double t = TenorMaturities.at(tenor);

        return s1 + (s2 - s1) * ((t - t1) / (t2 - t1));
    }
};

// @brief CreditCurveContainer
// @detail Container for multiple credit curves
//
class CreditCurveContainer {
private:
    std::map<std::string, std::shared_ptr<CreditCurve>> curves_;

public:
    CreditCurveContainer() = default;

    // Constructor that loads multiple CreditCurves from JSON data
    CreditCurveContainer(const json& jsonData) {
        for (const auto& [name, curveData] : jsonData.items()) {
            std::map<Tenor, double> curveMap;
            for (const auto& [tenorStr, spread] : curveData.items()) {
                auto it = StringToTenor.find(tenorStr);
                if (it == StringToTenor.end()) {
                    throw std::invalid_argument("Invalid tenor in JSON: " + std::string(tenorStr));
                }
                curveMap[it->second] = spread.get<double>();
            }
            curves_[name] = std::make_shared<CreditCurve>(std::move(curveMap));
        }
    }

    virtual ~CreditCurveContainer() = default;

    void addCreditCurve(const std::string& name, std::shared_ptr<CreditCurve> curve) {
        curves_[name] = std::move(curve);
    }

    const std::shared_ptr<CreditCurve>& getCreditCurve(const std::string& name) const {
        auto it = curves_.find(name);
        if (it == curves_.end()) {
            throw std::invalid_argument("No curve found for: " + name);
        }
        return it->second;
    }

    // Convert Tenor enum to string
    std::string tenorToString(Tenor tenor) const {
        for (const auto& [str, enumVal] : StringToTenor) {
            if (enumVal == tenor) return str;
        }
        return "Unknown";
    }

    // Save all CreditCurves to a JSON file
    void saveToJson(const std::string& filename) const {
        json j;
        for (const auto& [name, curve] : curves_) {
            j[name] = curve->toJson();
        }

        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing");
        }
        file << j.dump(4); // Pretty print with 4 spaces indentation
    }
};

} // namespace V1


namespace V2 {

// @brief CreditCurve class using std::vector for performance
// @details CreditCurve class represents a credit curve with tenors and corresponding spreads
//
class CreditCurve {
private:
    std::unordered_map<Tenor, double> spreads_;  // Store spreads in unordered_map for fast lookups
    std::vector<Tenor> tenors_;                   // Store tenors in a vector for ordered access

public:
    // Constructor that takes a map of tenors and spreads
    explicit CreditCurve(const std::unordered_map<Tenor, double>&& data) {
        spreads_ = std::move(data);

        // Sort the tenors in ascending order for interpolation purposes
        for (const auto& [tenor, spread] : spreads_) {
            tenors_.push_back(tenor);
        }

        // Sort the tenors by their maturities
        std::sort(tenors_.begin(), tenors_.end(), [](Tenor a, Tenor b) {
            return TenorMaturities.at(a) < TenorMaturities.at(b);
        });
    }

    // Get spread for a given tenor, with optional interpolation
    double getSpread(Tenor tenor, bool allowInterp = false) const {
        auto it = spreads_.find(tenor);
        if (it != spreads_.end()) {
            return it->second;
        }

        if (allowInterp) {
            return interpolateSpread(tenor);
        }

        throw std::invalid_argument("Spread not found for requested tenor.");
    }

    double getMaturity(const Tenor& tenor) const {
        auto it = TenorMaturities.find(tenor);
        if (it == TenorMaturities.end()) {
            throw std::invalid_argument("Unknown tenor");
        }
        return it->second;
    }

    bool hasTenor(Tenor tenor) const {
        return spreads_.find(tenor) != spreads_.end();
    }

    // Save CreditCurve to JSON
    json toJson() const {
        json j;
        for (const auto& [tenor, spread] : spreads_) {
            // Assuming you want to store the string representation of the tenor
            std::string tenorStr = TenorToString.at(tenor); 
            j[tenorStr] = spread;
        }
        return j;
    }

private:
    // Linear interpolation for missing spread values
    double interpolateSpread(const Tenor& tenor) const {
        if (spreads_.empty()) {
            throw std::invalid_argument("Cannot interpolate in an empty curve.");
        }

        // Find the index of the tenor in the ordered list of tenors
        auto it = std::find(tenors_.begin(), tenors_.end(), tenor);
        if (it != tenors_.end()) {
            return spreads_.at(tenor);  // Return spread directly if tenor exists
        }

        // If tenor does not exist, we need to interpolate between two surrounding tenors
        size_t index = std::distance(tenors_.begin(), it);
        if (index == 0 || index == tenors_.size()) {
            throw std::invalid_argument("Cannot interpolate: Tenor out of range.");
        }

        Tenor lowerTenor = tenors_[index - 1];
        Tenor upperTenor = tenors_[index];

        double lowerSpread = spreads_.at(lowerTenor);
        double upperSpread = spreads_.at(upperTenor);

        double lowerMaturity = TenorMaturities.at(lowerTenor);
        double upperMaturity = TenorMaturities.at(upperTenor);

        double maturity = TenorMaturities.at(tenor);

        // Perform linear interpolation
        return lowerSpread + (upperSpread - lowerSpread) * ((maturity - lowerMaturity) / (upperMaturity - lowerMaturity));
    }
};

// @brief CreditCurveContainer
// @detail Container for multiple credit curves
//
class CreditCurveContainer {
private:
    std::map<std::string, std::shared_ptr<CreditCurve>> curves_;
    
public:
    CreditCurveContainer() = default;
    
    // Constructor that loads multiple CreditCurves from JSON data
    CreditCurveContainer(const json& jsonData) {
        for (const auto& [name, curveData] : jsonData.items()) {
            std::unordered_map<Tenor, double> curveMap;
            for (const auto& [tenorStr, spread] : curveData.items()) {
                auto it = StringToTenor.find(tenorStr);
                if (it == StringToTenor.end()) {
                    throw std::invalid_argument("Invalid tenor in JSON: " + std::string(tenorStr));
                }
                curveMap[it->second] = spread.get<double>();
            }
            curves_[name] = std::make_shared<CreditCurve>(std::move(curveMap));
        }
    }
    
    virtual ~CreditCurveContainer() = default;
    
    void addCreditCurve(const std::string& name, std::shared_ptr<CreditCurve> curve) {
        curves_[name] = std::move(curve);
    }
    
    const std::shared_ptr<CreditCurve>& getCreditCurve(const std::string& name) const {
        auto it = curves_.find(name);
        if (it == curves_.end()) {
            throw std::invalid_argument("No curve found for: " + name);
        }
        return it->second;
    }
    
    // Convert Tenor enum to string
    std::string tenorToString(Tenor tenor) const {
        for (const auto& [str, enumVal] : StringToTenor) {
            if (enumVal == tenor) return str;
        }
        return "Unknown";
    }
    
    // Save all CreditCurves to a JSON file
    void saveToJson(const std::string& filename) const {
        json j;
        for (const auto& [name, curve] : curves_) {
            j[name] = curve->toJson();
        }
    
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Failed to open file for writing");
        }
        file << j.dump(4); // Pretty print with 4 spaces indentation
    }
};

} // namespace V2


// Another (bad) version 

class CreditCurveBad {
private:
    std::vector<std::string> tenors_;
    std::vector<double> spreads_;
    std::map<std::string, int> mapTenors_;

    public:
    CreditCurveBad() = delete;
    explicit CreditCurveBad(const std::vector<std::string>& tenors, const std::vector<double>& spreads) 
        : tenors_(tenors), spreads_(spreads) {
        int i = 0;
        for (std::string t : tenors_) {
            mapTenors_[t] = i;
            i++;
        }
    }

    virtual ~CreditCurveBad() {}

    const std::string toString() const {
        std::ostringstream oss;
        for (auto t: tenors_) {
            oss << t << ",";
        }
        auto n = spreads_.size();
        for (auto i=0; i < n-1; i++) {
            oss << ToString<double>(spreads_[i]) << ",";
        }
        // last one
        oss << ToString<double>(spreads_[n-1]);
        return oss.str();
    }

    const std::vector<std::string> getTenors() const { return tenors_; }

    const std::vector<double> getSpreads() const { return spreads_; }

    const double getSpread(const std::string& tenor) {
        std::map<std::string, int>::iterator it = mapTenors_.find(tenor);
        if (it == mapTenors_.end())
            throw std::invalid_argument(std::format("could not find spread with tenor {}", tenor));
        
        auto index = it->second;
        if (index > spreads_.size())
            throw std::invalid_argument(std::format("could not find spread with index {}", index));
        
        return spreads_[index];
    }
};

} // namespace eden