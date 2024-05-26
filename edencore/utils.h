#pragma once

#include <string>
#include <fstream>
#include <sstream> 

namespace eden {

template<typename T> std::string ToString(T t) 
{ 
    std::stringstream ss;
    ss << t;
    return ss.str();
}

} // namespace eden