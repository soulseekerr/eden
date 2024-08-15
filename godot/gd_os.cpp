
#include "gd_os.h"

OS* OS::singleton = nullptr;

OS* OS::get_singleton() {
	return singleton;
}

void OS::print_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify, Logger::ErrorType p_type) {
	if (!_stderr_enabled) {
		return;
	}

	if (_logger) {
		_logger->log_error(p_function, p_file, p_line, p_code, p_rationale, p_editor_notify, p_type);
	}
}