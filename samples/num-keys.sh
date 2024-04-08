#!/bin/bash

# Check if at least one IP:PORT combination is provided
if [ $# -eq 0 ]; then
    echo "Usage: $0 <IP1:PORT1> [<IP2:PORT2> ...]"
    exit 1
fi

# Initialize variable to store sum of replies
sum=0
replies=""

for address in "$@"; do
    reply=$(curl -s -X GET -d "__NUM_KEYS__" "http://$address")
    
    if [ $? -eq 0 ]; then
        _=$(curl -s -X DELETE -d "__LOCAL_ALL__" "http://$address")
        replies+=" $reply"
        sum=$((sum + reply))
    else
        echo "Error: Could not fetch reply for $address" >&2
    fi
done

echo "$sum:$replies"
