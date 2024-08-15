#ifndef GD_LOGGER_H
#define GD_LOGGER_H

#include "gd_classes.h"
#include "gd_errors.h"

#include <stdarg.h>

#include <vector>

#if defined(__GNUC__)
#define _PRINTF_FORMAT_ATTRIBUTE_2_0 __attribute__((format(printf, 2, 0)))
#define _PRINTF_FORMAT_ATTRIBUTE_2_3 __attribute__((format(printf, 2, 3)))
#else
#define _PRINTF_FORMAT_ATTRIBUTE_2_0
#define _PRINTF_FORMAT_ATTRIBUTE_2_3
#endif

class Logger {
protected:
	bool should_log(bool p_err);

    static bool _flush_stdout_on_print;

public:
	enum ErrorType {
		ERR_ERROR,
		ERR_WARNING
	};

	static void set_flush_stdout_on_print(bool value);

	virtual void logv(const char* p_format, va_list p_list, bool p_err) _PRINTF_FORMAT_ATTRIBUTE_2_0 = 0;
	virtual void log_error(const char* p_function, const char* p_file, int p_line, const char* p_code, const char *p_rationale, bool p_editor_notify = false, ErrorType p_type = ERR_ERROR);

	void logf(const char *p_format, ...) _PRINTF_FORMAT_ATTRIBUTE_2_3;
	void logf_error(const char *p_format, ...) _PRINTF_FORMAT_ATTRIBUTE_2_3;

	virtual ~Logger() {}
};

// Write messages to stdout/stderr
class StdLogger : public Logger {
public:
	virtual void logv(const char* p_format, va_list p_list, bool p_err) override _PRINTF_FORMAT_ATTRIBUTE_2_0;
	virtual ~StdLogger() {}
};

class CompositeLogger : public Logger {
	std::vector<Logger*> loggers__;

public:
	explicit CompositeLogger(std::vector<Logger*> p_loggers);

	virtual void logv(const char *p_format, va_list p_list, bool p_err) override _PRINTF_FORMAT_ATTRIBUTE_2_0;
	virtual void log_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify, ErrorType p_type = ERR_ERROR) override;

	void add_logger(Logger *p_logger);

	virtual ~CompositeLogger();
};


#endif // GD_LOGGER_H