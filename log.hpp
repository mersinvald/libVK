#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include <stdarg.h>

class Log {
public:
    static void log(int32_t verblevel, const char* message, ...);
    static void log(const char* message, ...);
    static void warning(const char* message, ...);
    static void error(const char* message, ...);
    static void set_verbosity(int32_t v);
    static void enable_warnongs(bool choice);
private:
    static int32_t VERB;
    static bool    WARNINGS;

    static double getUnixTime();
    static double getElapsedTime();
    static void   output(const char*, va_list);
};


#endif // LOG_H
