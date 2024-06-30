#pragma once

#define  EDEN_ANALYTICS_MAJOR_VERSION         0
#define  EDEN_ANALYTICS_MINOR_VERSION         2
#define  EDEN_ANALYTICS_REVISION_VERSION      1

#include <sstream>
#include <format>
#include <string>

namespace eden {
namespace analytics {

/**
 * Return EDEN version.
 *
 */
const std::string getVersion()
{
	std::ostringstream lFlux;
	lFlux << "EDEN Analytics v" << EDEN_ANALYTICS_MAJOR_VERSION << "." << EDEN_ANALYTICS_MINOR_VERSION << "." << EDEN_ANALYTICS_REVISION_VERSION << std::endl;
	return lFlux.str();
}

} // namespace eden
} // namespace analytics