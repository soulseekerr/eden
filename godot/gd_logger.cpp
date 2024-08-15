
#include "gd_logger.h"

bool Logger::should_log(bool p_err) {
	return (!p_err || CoreGlobals::print_error_enabled) && (p_err || CoreGlobals::print_line_enabled);
}

bool Logger::_flush_stdout_on_print = true;

void Logger::set_flush_stdout_on_print(bool value) {
	_flush_stdout_on_print = value;
}

void Logger::log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify, ErrorType p_type) {
	if (!should_log(true)) {
		return;
	}

	const char *err_type = "ERROR";
	switch (p_type) {
		case ERR_ERROR:
			err_type = "ERROR";
			break;
		case ERR_WARNING:
			err_type = "WARNING";
			break;
		default:
			ERR_PRINT("Unknown error type");
			break;
	}

	const char *err_details;
	if (p_rationale && *p_rationale) {
		err_details = p_rationale;
	} else {
		err_details = p_code;
	}

	if (p_editor_notify) {
		logf_error("%s: %s\n", err_type, err_details);
	} else {
		logf_error("USER %s: %s\n", err_type, err_details);
	}
	logf_error("   at: %s (%s:%i)\n", p_function, p_file, p_line);
}

void Logger::logf(const char *p_format, ...) {
	if (!should_log(false)) {
		return;
	}

	va_list argp;
	va_start(argp, p_format);

	logv(p_format, argp, false);

	va_end(argp);
}

void Logger::logf_error(const char *p_format, ...) {
	if (!should_log(true)) {
		return;
	}

	va_list argp;
	va_start(argp, p_format);

	logv(p_format, argp, true);

	va_end(argp);
}

void StdLogger::logv(const char *p_format, va_list p_list, bool p_err) {
	if (!should_log(p_err)) {
		return;
	}

	if (p_err) {
		vfprintf(stderr, p_format, p_list);
	} else {
		vprintf(p_format, p_list);
		if (_flush_stdout_on_print) {
			// Don't always flush when printing stdout to avoid performance
			// issues when `print()` is spammed in release builds.
			fflush(stdout);
		}
	}
}

CompositeLogger::CompositeLogger(std::vector<Logger *> p_loggers) :
		loggers__(p_loggers) {}

void CompositeLogger::logv(const char *p_format, va_list p_list, bool p_err) {
	if (!should_log(p_err)) {
		return;
	}

	for (int i = 0; i < loggers__.size(); ++i) {
		va_list list_copy;
		va_copy(list_copy, p_list);
		loggers__[i]->logv(p_format, list_copy, p_err);
		va_end(list_copy);
	}
}

void CompositeLogger::log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify, ErrorType p_type) {
	if (!should_log(true)) {
		return;
	}

	for (int i = 0; i < loggers__.size(); ++i) {
		loggers__[i]->log_error(p_function, p_file, p_line, p_code, p_rationale, p_editor_notify, p_type);
	}
}

void CompositeLogger::add_logger(Logger *p_logger) {
	loggers__.push_back(p_logger);
}

CompositeLogger::~CompositeLogger() {
	for (int i = 0; i < loggers__.size(); ++i) {
		// memdelete(loggers__[i]);
	}
}
