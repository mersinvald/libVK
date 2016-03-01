#include "types.hpp"

namespace vk {

using std::stringstream;

#define SEPARATOR(i, size, sep) (((i) < (size-1)) ? (sep) : (""))

template<typename T>
string to_string(const VKArray<T>& value) {
    stringstream ss;
    for(size_t i = 0; i < value.size(); i++) {
        ss << to_string(value[i])  << SEPARATOR(i, value.size(), ",");
    }
    return ss.str();
}

string to_string(const IDArray& value) {
    return to_string<ID>(value);
}

string to_string(const StrArray& value) {
    return to_string<string>(value);
}

string to_string(const Args& value) {
    stringstream ss;
    size_t cnt = 0;
    for(auto it = value.begin(); it != value.end(); ++it, cnt++) {
        ss << it->first << "=" << it->second << SEPARATOR(cnt, value.size(), "&");
    }
    return ss.str();
}

}
