#ifndef _FN_UTILS_H
#define _FN_UTILS_H

#include <string>

#define __BEGIN_IGNORE_UNUSEDVARS _Pragma("GCC diagnostic push")    \
    _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"") \
        _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")
#define __END_IGNORE_UNUSEDVARS _Pragma("GCC diagnostic pop")

#define __IGNORE_UNUSED_VAR(v) (void)v
#define __IGNORE_UNSUED_PVAR(v) (void*)v

void util_ltrim(std::string &s);
void util_rtrim(std::string &s);
void util_trim(std::string &s);
long util_parseInt(FILE *f, char skipChar);
long util_parseInt(FILE *f);

#endif // _FN_UTILS_H