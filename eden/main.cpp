
#include "hello.h"
#include "ver.h"
#include <sstream>
#include <format>
#include <string>
#include "fmt/core.h"

/**
 * Return EDEN version.
 *
 */
static const std::string getVersion()
{
	std::ostringstream lFlux;
	lFlux << EDEN_CORE_MAJOR_VERSION << "." << EDEN_CORE_MINOR_VERSION << "." << EDEN_CORE_REVISION_VERSION << std::endl;
	return lFlux.str();
}

int main() {

    hello();

    fmt::print("EDEN v{}.", getVersion());

    return 0;
}