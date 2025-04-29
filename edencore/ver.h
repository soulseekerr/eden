#pragma once

#define  EDEN_CORE_MAJOR_VERSION         0
#define  EDEN_CORE_MINOR_VERSION         1
#define  EDEN_CORE_REVISION_VERSION      5

#include <sstream>
#include <format>
#include <string>

namespace eden {

/**
 * Return EDEN version.
 *
 */
const std::string getVersion()
{
	std::ostringstream lFlux;
	lFlux << "EDEN Core v" << EDEN_CORE_MAJOR_VERSION << "." << EDEN_CORE_MINOR_VERSION << "." << EDEN_CORE_REVISION_VERSION << std::endl;
	return lFlux.str();
}

} // namespace eden