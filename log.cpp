#include "log.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <chrono>
#include <memory>


#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

int32_t Log::VERB = 1;
bool    Log::WARNINGS = true;

using namespace std;
using namespace std::chrono;

double
Log::getUnixTime() {
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    return (double)ms.count() / 1000;
}

double
Log::getElapsedTime() {
    static double start_time;
    static bool start_flag = 0;

    if(!start_flag){
        start_time = getUnixTime();
        start_flag = true;
    }

    return getUnixTime() - start_time;
}

void
Log::output(const char* message, va_list va) {
    char buff[512];

    vsprintf(buff, message, va);

    unique_ptr<char> out(new char[strlen(buff) + 32]);

    sprintf(out.get(), "[%.5lf] %s\n", getElapsedTime(), buff);

    printf(out.get());
}

void
Log::set_verbosity(int32_t v) {
    VERB = v;
}

void
Log::log(int32_t verblevel, const char *message, ...) {
    if(verblevel <= VERB){
        va_list va;
        va_start(va, message);
        output(message, va);
        va_end(va);
    }
}

void
Log::log(const char *message, ...) {
    va_list va;
    va_start(va, message);
    output(message, va);
    va_end(va);
}

void
Log::warning(const char *message, ...) {
    if(!WARNINGS) return;
    va_list va;
    va_start(va, message);

    unique_ptr<char> buff(new char[strlen(message) + 64]);

    sprintf(buff.get(), "WARNING: " RED "%s" RESET, message);
    output(buff.get(), va);
    va_end(va);
}

void
Log::error(const char *message, ...) {
    va_list va;
    va_start(va, message);

    unique_ptr<char> buff(new char[strlen(message) + 64]);
    sprintf(buff.get(), "ERROR: " RED "%s" RESET, message);
    output(buff.get(), va);
    va_end(va);
}
