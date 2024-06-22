#pragma once

#include <map>
#include <vector>
#include <string>

#include "datetime.h"
#include "utils.h"


namespace eden {

class CreditCurve {
public:
    CreditCurve() = delete;
    explicit CreditCurve(const std::vector<std::string>& tenors, const std::vector<double>& spreads) : tenors_(tenors), spreads_(spreads) {
        int i = 0;
        for (std::string t : tenors_) {
            mapTenors_[t] = i;
            i++;
        }
    }

    virtual ~CreditCurve() {}

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

private:
    std::vector<std::string> tenors_;
    std::vector<double> spreads_;
    std::map<std::string, int> mapTenors_;
};

} // namespace eden