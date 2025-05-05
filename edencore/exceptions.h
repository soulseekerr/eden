#pragma once
#include <stdexcept>
#include <string>
#include <format>

#include "pathmanager.h"

namespace eden {

/**
 * @brief Exception thrown when a required file is missing.
 * @details This exception is used to indicate that a file that is required for the
 */
class MissingFileException : public std::runtime_error {
public:
    explicit MissingFileException(const std::string& fileType, const std::string& message)
        : std::runtime_error(std::format("MissingFileException: Type {} - {}", fileType, message)) {}
};

} // namespace eden