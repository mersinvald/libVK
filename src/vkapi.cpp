/* Copyright (c) 2016 Mike Lubinets (aka mersinvald)
 * See LICENSE */

#include "vkapi.hpp"
#include "log.hpp"
#include "string_utils.hpp"
#include <string>
#include <iostream>
#include <string.h>
#include <thread>

namespace vk {

using std::chrono::system_clock;
using std::chrono::duration_cast;

#define VKAPI_INITIALIZER_LIST users(this), auth(this), wall(this), photos(this),                                   \
                               friends(this), widgets(this), storage(this), status(this),                           \
                               audio(this), pages(this), groups(this), board(this),                                 \
                               video(this), notes(this), places(this), account(this),                               \
                               messages(this), newsfeed(this), likes(this), polls(this),                            \
                               docs(this), fave(this), notifications(this), stats(this),                            \
                               search(this), apps(this), utils(this), database(this), gifts(this), market(this)

#define current_time() duration_cast<milliseconds>(system_clock().now().time_since_epoch())

VKAPI::VKAPI() : VKAPI_INITIALIZER_LIST {
    this->app_id = "";
    this->app_secret = "";
    this->curl_handle = nullptr;
    this->curl_errno = CURLE_OK;
    this->def_access_token = "";
    this->def_api_version  = "";
    this->def_lang         = "ru";
    this->max_requests_per_second = 3;
    this->request_counter = 0;
    this->last_time = current_time();

    curl_handle = curl_easy_init();
    if(!curl_handle) {
        throw CurlException("curl_easy_init() failed");
    }

    LOG3() << "initialized new curl handle";

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, VKAPI::CurlWriteDataCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->json);

    LOG3() << "curl writefunction is JSON writer now";
}

VKAPI::VKAPI(const string& app_id, const string& app_secret) : VKAPI() {
    this->app_id     = app_id;
    this->app_secret = app_secret;
}

API_RETURN_VALUE
VKAPI::Authorize(const string& login, const string& passwd, string* access_token) {
    Args args = {
        {"grant_type",   "password"},
        {"client_id",     app_id},
        {"client_secret", app_secret},
        {"username",      login},
        {"password",      passwd}
    };

    CustomRequest(VKAPI_AUTH_URL, "token", args);

    if(!json.isMember("access_token") || json.isMember("error")) {
        string error_msg;

        try {
            error_msg = this->json["error_description"].asString();
        } catch(std::exception&) {
            error_msg = "Authorization failed: VK returned no access token";
        }

        throw VKException(RESULT_AUTORIZATION_ERROR, error_msg);
    }

    def_access_token = json["access_token"].asString();
    if(access_token != NULL) {
        *access_token    = def_access_token;
    }

    LOG2() << "SUCCESS, access_token:" << def_access_token;

    return json;
}

API_RETURN_VALUE
VKAPI::Request(const string& method, Args& arguments) {
    /// Make sure we won't exceed requests limit
    if(request_counter++ >= max_requests_per_second) {
        milliseconds current_time = current_time();
        milliseconds diff         = current_time - last_time;
        milliseconds request_time = milliseconds(1000 / max_requests_per_second);
        if(diff < request_time) {
            std::this_thread::sleep_for(request_time);
            request_counter--;
        } else {
            uint32_t missed = diff / request_time;
            if(missed < request_counter) request_counter -= missed;
            else                         request_counter  = 0;
        }
        last_time = current_time();
    }

    /// Append default access_token
    if(arguments.find("access_token") == arguments.end()) {
        if(def_access_token == "") {
            WARNING() << "access token wasn't passed, only few methods will work correctly";
        } else {
            arguments["access_token"] = def_access_token;
        }
    }

    /// Append default api version
    if(arguments.find("v") == arguments.end()) {
        if(def_api_version == "") {
            WARNING() << "api version wasn't passed, VK will presume it's default, probably can lead to UB";
        } else {
            arguments["v"] = def_api_version;
        }
    }

    /// Append default lang
    if(arguments.find("lang") == arguments.end()) {
        if(def_lang == "") {
            WARNING() << "api default lang wasn't passed, VK will presume it's default";
        } else {
            arguments["lang"] = def_lang;
        }
    }

    CustomRequest(VKAPI_URL, method, arguments);
    HandleError(json);

    return json;
}

void
VKAPI::CustomRequest(const string& url, const string& method, const Args& arguments) {
    const string request_url = GenerateURL(url, method, arguments);
    LOG3() << "request url: " << escape_percent(request_url);

    curl_easy_setopt(curl_handle, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, VKAPI::CurlWriteDataCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->buffer);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT_MS, 5000L);

    /// Try curl perform max_tries times with 5s request timeout
    size_t max_tries = 3;
    curl_errno = CURLE_OK;
    while((curl_errno = curl_easy_perform(curl_handle)) == CURLE_OPERATION_TIMEDOUT
          && max_tries--);

    if(curl_errno != CURLE_OK) {
        throw CurlException(curl_errno, curl_easy_strerror(curl_errno));
    }

    ReadDataToJSON();
}

void
VKAPI::HandleError(const VKValue& json) {
    if(!json.isMember("error")) return;
    Value error = json["error"];

    try {    
        vk_errno    = (VKResultCode_t) error["error_code"].asInt();
    } catch (JsonException&) {
        vk_errno    = RESULT_ERROR;
    };

    throw VKException(json);
}

const string
VKAPI::GenerateURL(const string& url, const string& method, const Args& args) {
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

void
VKAPI::ReadDataToJSON() {
    json.clear();
    Reader reader;

    if(!reader.parse(buffer, json, false)) {
        buffer.clear();
        throw JsonException(reader.getFormattedErrorMessages());
    }

    buffer.clear();
}

/* ##### SETTERS ##### */

void
VKAPI::SetAppID(const string& app_id) {
    this->app_id = app_id;
}

void
VKAPI::SetAppSecret(const string& app_secret) {
    this->app_secret = app_secret;
}

void
VKAPI::SetDefaultLang(const string& lang) {
    this->def_lang = lang;
}

void
VKAPI::SetDefaultAccessToken(const string& token) {
    this->def_access_token = token;
}

void
VKAPI::SetDefaultAPIVersion(const string& version) {
    this->def_api_version = version;
}

void
VKAPI::SetMaxRequestsPerSec(const uint8_t max_requests) {
    this->max_requests_per_second = max_requests;
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

const string&
VKAPI::getAccessToken() const {
    return def_access_token;
}

}
