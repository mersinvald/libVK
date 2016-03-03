/* This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef SIMPLELOGGER_HPP
#define SIMPLELOGGER_HPP
#include <functional>
#include <iostream>
#include <chrono>
#include <string>
#include <sstream>

namespace log {

using std::string;
using std::ostream;
using std::stringstream;
using namespace std::chrono;

/// Log levels
enum lvl {
    error   = 0,
    warning = 1,
    info    = 2,
    log     = 3,
    log1    = 4,
    log2    = 5,
    log3    = 6,
    trace   = 7
};

const string lvl_strings[] = {
    "ERROR",
    "WARNING",
    "INFO",
    "LOG0",
    "LOG1",
    "LOG2",
    "LOG3",
    "TRACE"
};

/// Printers
#define PRINT_LEVEL(level)          ((log::print_level)                     ? log::Logger::resizeString(log::lvl_strings[level] + " ", 8,  ' ')             : "")
#define PRINT_FUNC()                ((log::print_func)                      ? log::Logger::resizeString(string(__FUNCTION__)    + ": ",16, ' ')             : "")
#define PRINT_SOURCE()              ((log::print_source)                    ? log::Logger::trimPath(string(__FILE__ ":") + std::to_string(__LINE__)) + " "  : "")
#define PRINT_TIMER()               ((log::print_timer)                     ? string("[") + log::Logger::getTimer()                                  + "] " : "")
#define PRINT_DATE()                ((log::print_date && !log::print_timer) ? log::Logger::getTimeFmt("%D")                                          + "/"  : "")
#define PRINT_TIME()                ((log::print_time && !log::print_timer) ? log::Logger::getTimeFmt("%T")                                          + " " : "")

/// Logger macros
#define LOG_OSTREAM(level)          ((level == log::error || level == log::warning) ? log::err_ostream : log::log_ostream)
#define Log(level)                  log::Logger(LOG_OSTREAM(level), level) \
                                             << PRINT_TIMER()      << PRINT_DATE()     << PRINT_TIME() \
                                             << PRINT_LEVEL(level) << PRINT_SOURCE() << PRINT_FUNC()

#define ERROR()                     Log(log::error)
#define WARNING()                   Log(log::warning)
#define INFO()                      Log(log::info)
#define LOG()                       Log(log::log)
#define LOG0()                      Log(log::log)
#define LOG1()                      Log(log::log1)
#define LOG2()                      Log(log::log2)
#define LOG3()                      Log(log::log3)
#define TRACE()                     Log(log::trace)

/// Rebindable reference
typedef std::reference_wrapper<ostream> osref;

/// Configuration
extern lvl   log_level;
extern osref log_ostream;
extern osref err_ostream;
extern bool  print_level;
extern bool  print_func;
extern bool  print_source;
extern bool  print_timer;
extern bool  print_date;
extern bool  print_time;

class Logger {
public:
    explicit Logger(ostream& os, lvl level) : out(os), level(level) {}
    virtual ~Logger() {
        if(level <= log_level) {
            out << std::endl;
            out.flush();
        }
    }

    template<typename T>
    Logger& operator<< (const T& var) {
        if(level <= log_level) {
            out << var;
        }
        return *this;
    }

    static string getTimer() {
        static milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        milliseconds now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

        stringstream builder;
        builder.precision(5);
        builder << std::fixed << static_cast<double>((now - start).count()) / 1000;
        return builder.str();
    }

    static string getTimeFmt(const char* fmt) {
        time_t time    = system_clock::to_time_t(system_clock::now());
        tm*    time_tm = localtime(&time);

        char time_buf[64];
        if(!strftime(time_buf, 64, fmt, time_tm)) {
            return "";
        }
        return string(time_buf);
    }

    static string resizeString(string str, size_t n, char c) {
        str.resize(n, c);
        return str;
    }

    static string trimPath(string path) {
        size_t pos = 0;
        while((pos = path.find("../")) != path.npos) {
            path.replace(pos, 3, "");
            pos += 3;
        }
        return resizeString(path, 24, ' ');
    }

private:
    ostream& out;
    lvl      level;
};

}
#endif // SIMPLELOGGER_HPP
