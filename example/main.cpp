#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "vkapi.hpp"

#define LOG_VERB 5
#include "log.hpp"

using namespace std;
using namespace vk;

#define CLIENT_ID      "3697615"
#define CLIENT_SECRET  "AlVXZFMUqyrnABp8ncuU"
#define USERNAME       "lubinetsm@inbox.ru"
#define PASSWORD       "theunpredictableFortunes"

int main(int argc, char** argv) {
    srand(clock());

    log::print_timer = true;
    log::print_func  = true;
    log::print_level = true;
    log::log_level   = log::trace;

    ofstream log_file("./log.txt");
    log::log_ostream = cerr;
    log::log_ostream = log_file;

    INFO() << "Starting Liker :)";

    VKAPI api(CLIENT_ID, CLIENT_SECRET);
    api.SetDefaultAPIVersion("5.45");
    api.Authorize(USERNAME, PASSWORD);

    Args args;
    args["count"] = "500";
    api.users.search(args);

    Value defvalue;

    auto response = api.getJSON()["response"];
    auto items    = response["items"];

    size_t cnt = 0;
    for(auto it = items.begin(); it != items.end(); ++it) {
        //std::cout << (*it)["id"] << " ";
        cnt++;
    }

    std::cout << "\nCount: " << cnt << endl;

    string msg = to_string(rand() % 10000); msg += " ";
    msg += to_string(rand() % 10000); msg += " ";
    msg += to_string(rand() % 10000); msg += " ";
    msg += to_string(rand() % 10000); msg += " ";
    msg += to_string(rand() % 10000);

    //args.clear();
    //args["user_id"] = "319523999";
    //args["message"] = "Ублюдок, мать твою, а ну иди сюда, говно собачье, а ну решил ко мне лезть, ты... засранец вонючий, мать твою. А?!";
    //api.messages.send(args);

    return 0;
}
