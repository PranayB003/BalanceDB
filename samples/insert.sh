#!/bin/bash

# Reads a list of key-value pairs from the input file and
# inserts them into the database.

if [ $# -lt 2 ]; then
    echo "Usage: $0 <input_file> <url>"
    exit 1
fi

input_file=$1
url=$2

# Check if input file exists
if [ ! -f "$input_file" ]; then
    echo "Error: Input file '$input_file' not found."
    exit 1
fi

while IFS= read -r line; do
    key=$(echo "$line" | awk '{print $1}')
    value=$(echo "$line" | awk '{print $2}')

    curl -X PUT -d "$key=$value" "$url"

done < "$input_file"

