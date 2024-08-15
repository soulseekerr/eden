
#ifndef GD_ERRORS_H
#define GD_ERRORS_H

#include "gd_string.h"

enum ErrorHandlerType {
	ERR_HANDLER_ERROR,
	ERR_HANDLER_WARNING,
	ERR_HANDLER_SCRIPT,
	ERR_HANDLER_SHADER,
};

// Pointer to the error handler printing function. Reassign to any function to have errors printed.
// Parameters: userdata, function, file, line, error, explanation, type.
typedef void (*ErrorHandlerFunc)(void *, const char *, const char *, int p_line, const char *, const char *, bool p_editor_notify, ErrorHandlerType p_type);

struct ErrorHandlerList {
	ErrorHandlerFunc errfunc = nullptr;
	void *userdata = nullptr;

	ErrorHandlerList *next = nullptr;

	ErrorHandlerList() {}
};

// Functions used by the error macros.
void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, bool p_editor_notify = false, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, bool p_editor_notify = false, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, const char *p_message, bool p_editor_notify = false, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, const char *p_message, bool p_editor_notify = false, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const char *p_error, const String &p_message, bool p_editor_notify = false, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_error(const char *p_function, const char *p_file, int p_line, const String &p_error, const String &p_message, bool p_editor_notify = false, ErrorHandlerType p_type = ERR_HANDLER_ERROR);
void _err_print_index_error(const char *p_function, const char *p_file, int p_line, int64_t p_index, int64_t p_size, const char *p_index_str, const char *p_size_str, const char *p_message = "", bool p_editor_notify = false, bool fatal = false);
void _err_print_index_error(const char *p_function, const char *p_file, int p_line, int64_t p_index, int64_t p_size, const char *p_index_str, const char *p_size_str, const String &p_message, bool p_editor_notify = false, bool fatal = false);

#ifdef __GNUC__
//#define FUNCTION_STR __PRETTY_FUNCTION__ - too annoying
#define FUNCTION_STR __FUNCTION__
#else
#define FUNCTION_STR __FUNCTION__
#endif

#define ERR_FAIL_COND_MSG(m_cond, m_msg)                                                                      \
	if (unlikely(m_cond)) {                                                                                   \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true.", m_msg); \
		return;                                                                                               \
	} else                                                                                                    \
		((void)0)

/**
 * Try using `ERR_FAIL_COND_MSG`, `ERR_FAIL_COND_V_MSG`, `ERR_CONTINUE_MSG` or `ERR_BREAK_MSG`.
 * Only use this macro at the start of a function that has not been implemented yet, or
 * if more complex error detection or recovery is required.
 *
 * Prints `m_msg`.
 */
#define ERR_PRINT(m_msg) \
	_err_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg)


// Null reference error macros.

/**
 * Try using `ERR_FAIL_NULL_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures a pointer `m_param` is not null.
 * If it is null, the current function returns.
 */
#define ERR_FAIL_NULL(m_param)                                                                          \
	if (unlikely(m_param == nullptr)) {                                                                 \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null."); \
		return;                                                                                         \
	} else                                                                                              \
		((void)0)

/**
 * Ensures a pointer `m_param` is not null.
 * If it is null, prints `m_msg` and the current function returns.
 */
#define ERR_FAIL_NULL_MSG(m_param, m_msg)                                                                      \
	if (unlikely(m_param == nullptr)) {                                                                        \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg); \
		return;                                                                                                \
	} else                                                                                                     \
		((void)0)

/**
 * Same as `ERR_FAIL_NULL_MSG` but also notifies the editor.
 */
#define ERR_FAIL_NULL_EDMSG(m_param, m_msg)                                                                          \
	if (unlikely(m_param == nullptr)) {                                                                              \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg, true); \
		return;                                                                                                      \
	} else                                                                                                           \
		((void)0)

/**
 * Try using `ERR_FAIL_NULL_V_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures a pointer `m_param` is not null.
 * If it is null, the current function returns `m_retval`.
 */
#define ERR_FAIL_NULL_V(m_param, m_retval)                                                              \
	if (unlikely(m_param == nullptr)) {                                                                 \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null."); \
		return m_retval;                                                                                \
	} else                                                                                              \
		((void)0)

/**
 * Ensures a pointer `m_param` is not null.
 * If it is null, prints `m_msg` and the current function returns `m_retval`.
 */
#define ERR_FAIL_NULL_V_MSG(m_param, m_retval, m_msg)                                                          \
	if (unlikely(m_param == nullptr)) {                                                                        \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg); \
		return m_retval;                                                                                       \
	} else                                                                                                     \
		((void)0)

/**
 * Same as `ERR_FAIL_NULL_V_MSG` but also notifies the editor.
 */
#define ERR_FAIL_NULL_V_EDMSG(m_param, m_retval, m_msg)                                                              \
	if (unlikely(m_param == nullptr)) {                                                                              \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg, true); \
		return m_retval;                                                                                             \
	} else                                                                                                           \
		((void)0)

/**
 * Try using `ERR_FAIL_COND_MSG`.
 * Only use this macro if there is no sensible error message.
 * If checking for null use ERR_FAIL_NULL_MSG instead.
 * If checking index bounds use ERR_FAIL_INDEX_MSG instead.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, the current function returns.
 */
#define ERR_FAIL_COND(m_cond)                                                                          \
	if (unlikely(m_cond)) {                                                                            \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true."); \
		return;                                                                                        \
	} else                                                                                             \
		((void)0)

/**
 * Ensures `m_cond` is false.
 * If `m_cond` is true, prints `m_msg` and the current function returns.
 *
 * If checking for null use ERR_FAIL_NULL_MSG instead.
 * If checking index bounds use ERR_FAIL_INDEX_MSG instead.
 */
#define ERR_FAIL_COND_MSG(m_cond, m_msg)                                                                      \
	if (unlikely(m_cond)) {                                                                                   \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true.", m_msg); \
		return;                                                                                               \
	} else                                                                                                    \
		((void)0)

/**
 * Same as `ERR_FAIL_COND_MSG` but also notifies the editor.
 */
#define ERR_FAIL_COND_EDMSG(m_cond, m_msg)                                                                          \
	if (unlikely(m_cond)) {                                                                                         \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true.", m_msg, true); \
		return;                                                                                                     \
	} else                                                                                                          \
		((void)0)

/**
 * Try using `ERR_FAIL_COND_V_MSG`.
 * Only use this macro if there is no sensible error message.
 * If checking for null use ERR_FAIL_NULL_V_MSG instead.
 * If checking index bounds use ERR_FAIL_INDEX_V_MSG instead.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, the current function returns `m_retval`.
 */
#define ERR_FAIL_COND_V(m_cond, m_retval)                                                                                         \
	if (unlikely(m_cond)) {                                                                                                       \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Returning: " _STR(m_retval)); \
		return m_retval;                                                                                                          \
	} else                                                                                                                        \
		((void)0)

/**
 * Ensures `m_cond` is false.
 * If `m_cond` is true, prints `m_msg` and the current function returns `m_retval`.
 *
 * If checking for null use ERR_FAIL_NULL_V_MSG instead.
 * If checking index bounds use ERR_FAIL_INDEX_V_MSG instead.
 */
#define ERR_FAIL_COND_V_MSG(m_cond, m_retval, m_msg)                                                                                     \
	if (unlikely(m_cond)) {                                                                                                              \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Returning: " _STR(m_retval), m_msg); \
		return m_retval;                                                                                                                 \
	} else                                                                                                                               \
		((void)0)

/**
 * Same as `ERR_FAIL_COND_V_MSG` but also notifies the editor.
 */
#define ERR_FAIL_COND_V_EDMSG(m_cond, m_retval, m_msg)                                                                                         \
	if (unlikely(m_cond)) {                                                                                                                    \
		_err_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Returning: " _STR(m_retval), m_msg, true); \
		return m_retval;                                                                                                                       \
	} else                                                                                                                                     \
		((void)0)


#endif // GD_ERRORS_H