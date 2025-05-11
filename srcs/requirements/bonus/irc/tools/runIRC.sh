#!/bin/bash

if [ -z "$IRC_PASS" ]; then
    echo "❌ Missing environment variable: IRC_PASS"
    exit 1
fi

cd /IRCSERV
make
./ircserv 6667 $IRC_PASS