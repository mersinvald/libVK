/* Copyright (c) 2016 Mike Lubinets (aka mersinvald)
 * See LICENSE */

#ifndef VKAPI_TYPES_HPP
#define VKAPI_TYPES_HPP
#include <string>
#include <vector>
#include <map>
#include <assert.h>
#include "json/json.h"

namespace vk {

using std::vector;
using std::string;
using std::map;
using std::to_string;

typedef intmax_t ID;

template<typename T>
using VKArray  = vector<T>;
using IDArray  = VKArray<ID>;
using StrArray = VKArray<string>;
using Args     = map<string, string>;

class VKAPI;

template<typename T>
string to_string(const VKArray<T>& value);
string to_string(const IDArray& value);
string to_string(const StrArray& value);
string to_string(const Args& value);

inline string to_string(string str) { return str; }

}

#endif // VKAPI_TYPES_HPP
