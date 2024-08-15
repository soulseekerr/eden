#ifndef GD_STRING_H
#define GD_STRING_H

#include "gd_classes.h"

class String {

public:
	enum {
		npos = -1 ///<for "some" compatibility with std::string (npos is a huge value in std::string)
	};

    _ALWAYS_INLINE_ String() {}
    _ALWAYS_INLINE_ String(const String& p_str) {}
    _ALWAYS_INLINE_ void operator=(const String& p_str) {}

};

#endif // GD_STRING_H