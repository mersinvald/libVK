/* Copyright (c) 2016 Mike Lubinets (aka mersinvald)
 * See LICENSE */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "vkapi.hpp"
#include "log.hpp"

using namespace std;
using namespace vk;

/// ID and secret of official VK app
#define CLIENT_ID      "3697615"
#define CLIENT_SECRET  "AlVXZFMUqyrnABp8ncuU"

/// Username and password for autorization
#define USERNAME       "example@example.com"
#define PASSWORD       "examplepassword"

int main() {
    /// Initialize logging
    /// More details may be found in https://gist.github.com/mersinvald/5df1036aabd67e0f6fea
    mlog::print_timer = true;
    mlog::print_func  = true;
    mlog::print_level = true;
    mlog::log_level   = mlog::trace;

    INFO() << "Starting libvk example :)";

    /// Create api wrapper instance
    VKAPI api(CLIENT_ID, CLIENT_SECRET);

    /// Tell VK that we are using version 5.45
    api.SetDefaultAPIVersion("5.45");

    /// Authorize with login-password
    api.Authorize(USERNAME, PASSWORD);

    /// In order to make api request, define arguments for method
    /// and then call suitable api method. Pretty simple.
    Args args;
    args["count"] = "500";
    api.users.search(args);

    /// Auto method execution result will be placed in VKAPI::json,
    /// so it can be optained with getJSON() method, returning Json::Value
    auto response = api.getJSON()["response"];
    auto items    = response["items"];

    /// Print out ids of users, optained from vkapi
    size_t cnt = 0;
    for(auto it = items.begin(); it != items.end(); ++it) {
        std::cout << (*it)["id"] << std::endl;
        cnt++;
    }

    LOG() << "Got " << cnt << " users";

    return 0;
}
