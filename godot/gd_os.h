#ifndef GD_OS_H
#define GD_OS_H

#include "gd_logger.h"

class OS {
    static OS* singleton;
    bool _stderr_enabled = true;
    CompositeLogger* _logger = nullptr;

public:
    OS() {
        singleton = this;


    }

    ~OS(){
        singleton = nullptr;
    }

    static OS* get_singleton();

    void print_error(const char *p_function, const char *p_file, int p_line, const char *p_code, const char *p_rationale, bool p_editor_notify = false, Logger::ErrorType p_type = Logger::ERR_ERROR);
};

#endif // GD_OS_H