/* Copyright (c) 2016 Mike Lubinets (aka mersinvald)
 * See LICENSE */

#include "log.hpp"

namespace mlog {

lvl      log_level    = info;       ///< Default log level, every message above it will not be displayed
osref    log_ostream  = std::cout;  ///< Default ostream for any loglevel but ERROR and WARNING
osref    err_ostream  = std::cerr;  ///< Default ostream fot ERROR and WARNING loglevels
bool     print_level  = false;      ///< Display loglevel?
bool     print_func   = false;      ///< Display logging function?
bool     print_source = false;      ///< Display logging place in source code?
bool     print_timer  = false;      ///< Display timer?
bool     print_date   = false;      ///< Display date? (timer and datetime can't be displayer together, timer has priority if both selected)
bool     print_time   = false;      ///< Display time?

};
