/* Copyright (c) 2016 Mike Lubinets (aka mersinvald)
 * See LICENSE */

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
        throw CurlException("curl_easy_init() failed");
    }

    LOG3() << "initialized new curl handle";

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, VKAPI::CurlWriteDataCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->json);

    LOG3() << "curl writefunction is JSON writer now";
}

VKAPI::VKAPI(const string app_id, const string app_secret) : VKAPI() {
    this->app_id     = app_id;
    this->app_secret = app_secret;
}

API_RETURN_VALUE
VKAPI::Authorize(const string login, const string passwd, string* access_token) {
    Args args;
    args["grant_type"]    = "password";
    args["client_id"]     = app_id;
    args["client_secret"] = app_secret;
    args["username"]      = login;
    args["password"]      = passwd;

    CustomRequest(VKAPI_AUTH_URL, "token", args);

    HandleError(json);

    if(!json.isMember("access_token")) {
        throw VKException("Authorization failed: VK returned no access token");
    }

    def_access_token = json["access_token"].asString();
    if(access_token != NULL) {
        *access_token    = def_access_token;
    }

    LOG2() << "SUCCESS, access_token:" << def_access_token;
}

API_RETURN_VALUE
VKAPI::Request(const string method, Args& arguments) {
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
            WARNING() << "api version wasn't' passed, VK will presume it's default, probably can lead to UB";
        } else {
            arguments["v"] = def_api_version;
        }
    }

    CustomRequest(VKAPI_URL, method, arguments);
    HandleError(json);
}

API_RETURN_VALUE
VKAPI::CustomRequest(const string url, const string method, const Args& arguments) {
    const string request_url = GenerateURL(url, method, arguments);
    LOG3() << "request url: " << escape_percent(request_url);

    curl_easy_setopt(curl_handle, CURLOPT_URL, request_url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, VKAPI::CurlWriteDataCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &this->buffer);

    curl_errno = curl_easy_perform(curl_handle);
    if(curl_errno != CURLE_OK) {
        throw CurlException(curl_errno, curl_easy_strerror(curl_errno));
    }

    ReadDataToJSON();
}

API_RETURN_VALUE
VKAPI::HandleError(const VKValue json) {
    if(!json.isMember("error")) return;

    Value error          = json["error"];
    vk_errno             = (VKResultCode_t) error["error_code"].asInt();

    throw VKException(json);
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
