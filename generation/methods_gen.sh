#!/bin/bash

last=""
single="0"

for method in `cat $1`; do
    current=${method%%.*}
    name=${method##*.}

    if [ "$current" != "$last" ] ; then
        if [ "$last" != "" ] && [ $single == "0" ] ; then
            printf "} $last;\n\n"
        fi;
        if echo $method | grep '\.' >> /dev/null; then
            classname=${current}_api
            printf "class $classname {\n"
            printf "    API_SUBCLASS_INIT($classname)\n"
        fi
    fi;

    if echo $method | grep '\.' >> /dev/null; then
        printf "    inline VKResultCode_t $name (API_METHOD_ARGS)\t\t\t\tAPI_SUBCLASS_METHOD_REQUEST(\"$method\")\n"
        single="0"
    else
        printf "inline VKResultCode_t $name(API_METHOD_ARGS)\t\t\t\tAPI_METHOD_REQUEST(\"$current\")\n\n"
        single="1"
    fi;

    last=$current
done;

if [ $single == "0" ]; then
    printf "} $last;\n\n"
fi;
