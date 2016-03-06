#include "log.hpp"
#include "vkapi.hpp"
#include "backward/backward.hpp"
#include <string>

namespace vk {

#ifdef __linux__
void
printStackTrace(uint16_t max_frames, bool forced) {
    if(mlog::log_level < mlog::lvl::trace && !forced) return;
    backward::StackTrace st;
    st.load_here(max_frames);

    backward::Printer p;
    
    p.object  = true;
    p.color   = true;
    p.address = true;
#ifndef NDEBUG
    p.snippet = true;
#endif
    p.print(st, stderr);
}

#else
/// Just dummy function
void
getStackTrace(uint16_t max_frames) {
    if(log::log_level < log::lvl::trace || !forced) return;
    WARNING() << "Stack tracing is not availible on this platform :C";
}
#endif

libVKException::libVKException()
    : err_str("NULL"), err_code(-1) {}

libVKException::libVKException(int error_code, const string& error_msg)
    : err_str(error_msg), err_code(error_code) {}

libVKException::libVKException(const string& error_msg)
    : err_str(error_msg), err_code(-1) {}

libVKException::libVKException(const char* error_msg)
    : err_str(error_msg), err_code(-1) {}

static string exception_str_buffer = "";
const char*
libVKException::what() const noexcept {
    std::stringstream ss;
    ss << err_str << std::endl;

    if(err_code != -1) {
        ss << "           Error code: " << err_code << std::endl;
    }

    exception_str_buffer = ss.str();
    return exception_str_buffer.c_str();
}

std::ostream&
operator<< (std::ostream& os, const VKException& e) {
    os << "Exception:\n"
       << e.what();

    return os;
}

VKException::VKException(const VKValue& json) {
    if(!json.isMember("error")) return;
    Value error          = json["error"];
    Value request_params = error["request_params"];
    err_code             = static_cast<VKResultCode_t>(error["error_code"].asInt());

    std::stringstream ss;
    ss << error["error_msg"].asString() << "\n";
    for(ArrayIndex i = 0; i < request_params.size()-1; i++) {
        ss << request_params[i]["key"] << "  :  "
           << request_params[i]["value"]
           << std::endl;
    }

    err_str = ss.str();
}

}
