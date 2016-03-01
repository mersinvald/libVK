#include "vkapi.hpp"
#include "log.hpp"
#include "string_utils.hpp"
#include <string>
#include <iostream>
#include <string.h>

namespace vk {

#define VKAPI_INITIALIZER_LIST users(this), auth(this), wall(this), photos(this),                                   \
                               friends(this), widgets(this), storage(this), status(this),                           \
                               audio(this), pages(this), groups(this), board(this),                                 \
                               video(this), notes(this), places(this), account(this),                               \
                               messages(this), newsfeed(this), likes(this), polls(this),                            \
                               docs(this), fave(this), notifications(this), stats(this),                            \
                               search(this), apps(this), utils(this), database(this), gifts(this), market(this)     \

VKAPI::VKAPI() : VKAPI_INITIALIZER_LIST {
    this->app_id = "";
    this->app_secret = "";
    this->curl_handle = nullptr;
    this->curl_errno = CURLE_OK;
    this->def_access_token = "";
    this->def_api_version  = "";

    curl_handle = curl_easy_init();
    if(!curl_handle) {
        Log::error("%s: couldn't initialize API, curl initialization failed", __FUNCTION__);
        exit(1);
    }

    Log::log(4, "%s: initialized new curl handle", __FUNCTION__);

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, VKAPI::CurlWriteDataCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->json);

    Log::log(4, "%s: curl writefunction is JSON writer now", __FUNCTION__);
}

VKAPI::VKAPI(const string app_id, const string app_secret) : VKAPI() {
    this->app_id     = app_id;
    this->app_secret = app_secret;
}

VKResultCode_t
VKAPI::Authorize(const string login, const string passwd, string* access_token) {
    Args args;
    args["grant_type"]    = "password";
    args["client_id"]     = app_id;
    args["client_secret"] = app_secret;
    args["username"]      = login;
    args["password"]      = passwd;

    CustomRequest(VKAPI_AUTH_URL, "token", args);

    if(HandleError(json) != RESULT_SUCCESS) {
       return vk_errno;
    }

    if(!json.isMember("access_token")) {
        Log::error("%s: no \"access token\" field in VK API answer.", __FUNCTION__);
        return RESULT_ERROR;
    }

    def_access_token = json["access_token"].asString();
    if(access_token != NULL) {
        *access_token    = def_access_token;
    }

    Log::log(2, "%s: SUCCESS, access_token: %s", __FUNCTION__,  def_access_token.c_str());
    return RESULT_SUCCESS;
}

VKResultCode_t
VKAPI::Request(const string method, Args& arguments) {
    /// Append default access_token
    if(arguments.find("access_token") == arguments.end()) {
        if(def_access_token == "") {
            Log::warning("%s: access token wasn't passed, only few methods will work correctly", __FUNCTION__);
        } else {
            arguments["access_token"] = def_access_token;
        }
    }

    /// Append default api version
    if(arguments.find("v") == arguments.end()) {
        if(def_api_version == "") {
            Log::warning("%s: api version wasn't' passed, VK will presume it's default, probably can lead to UB", __FUNCTION__);
        } else {
            arguments["v"] = def_api_version;
        }
    }

    if(CustomRequest(VKAPI_URL, method, arguments) != RESULT_SUCCESS) {
        return RESULT_ERROR;
    }

    return HandleError(json);
}

VKResultCode_t
VKAPI::CustomRequest(const string url, const string method, const Args& arguments) {
    const string request_url = GenerateURL(url, method, arguments);
    Log::log(5, "%s request url: %s", __FUNCTION__, escape_percent(request_url).c_str());

    curl_easy_setopt(curl_handle, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, VKAPI::CurlWriteDataCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->buffer);

    curl_errno = curl_easy_perform(curl_handle);
    if(curl_errno != CURLE_OK) {
        Log::error("%s: curl returned \"%s\"", __FUNCTION__, curl_easy_strerror(curl_errno));
        return RESULT_ERROR;
    }

    ReadDataToJSON();

    return RESULT_SUCCESS;
}

VKResultCode_t
VKAPI::HandleError(const VKValue json) {
    if(!json.isMember("error")) return RESULT_SUCCESS;
    Value error          = json["error"];
    vk_errno             = (VKResultCode_t) error["error_code"].asInt();
    string err_msg       = error["error_msg"].asString();
    Value request_params = error["request_params"];

    std::stringstream ss;
    for(ArrayIndex i = 0; i < request_params.size()-1; i++) {
        ss << "\t\t\t"
           << request_params[i]["key"] << "  :  "
           << request_params[i]["value"]
           << std::endl;
    }

    Log::error("%s: VKAPI returned error %i:\n\t\t\tError Message: %s\n%s",
               __FUNCTION__, vk_errno, err_msg.c_str(), ss.str().c_str());

    return vk_errno;
}

const string
VKAPI::GenerateURL(const string url, const string method, const Args& args) {
    std::stringstream ss;
    ss << url << method << "?" << to_string(args);
    return escape_spaces(ss.str());
}

size_t
VKAPI::CurlWriteDataCallback(void* contents, size_t size, size_t nmemb, void* useptr) {
    string* buffer = reinterpret_cast<string*>(useptr);
    char*   data   = reinterpret_cast<char*>(contents);
    buffer->append(data);
    return size*nmemb;
}

VKResultCode_t
VKAPI::ReadDataToJSON() {
    json.clear();
    Reader reader;

    if(!reader.parse(buffer, json, false)) {
        Log::error("%s: json parsing error %s", __FUNCTION__, reader.getFormattedErrorMessages().c_str());
        buffer.clear();
        return RESULT_ERROR;
    }
    buffer.clear();
    return RESULT_SUCCESS;
}

/* ##### SETTERS ##### */

void
VKAPI::SetAppID(const string app_id) {
    this->app_id = app_id;
}

void
VKAPI::SetAppSecret(const string app_secret) {
    this->app_secret = app_secret;
}

void
VKAPI::SetefaultAccessToken(const string token) {
    this->def_access_token = token;
}

void
VKAPI::SetDefaultAPIVersion(const string version) {
    this->def_api_version = version;
}

/* ##### GETTERS ##### */

CURLcode
VKAPI::getCurlError() const {
    return curl_errno;
}

const VKValue&
VKAPI::getJSON() const {
    return json;
}

string
VKAPI::getAccessToken() const {
    return def_access_token;
}

}
