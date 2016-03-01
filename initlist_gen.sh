#!/bin/bash

last=""
single="0"

for method in `cat $1`; do
    current=${method%%.*}
    name=${method##*.}

    if [ "$current" != "$last" ]; then
        if echo $method | grep '\.' >> /dev/null; then
            printf ", $current(this)"
            single="0"
        fi;
    fi;
    last=$current
done;
