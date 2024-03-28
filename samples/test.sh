#!/bin/bash
if [ $# -eq 0 ]; then
    echo "Usage: $0 <input_file>"
    exit 1
fi

# Read each line from the input file
while IFS= read -r line; do
    key=$(echo "$line" | awk '{print $1}')
    value=$(echo "$line" | awk '{print $2}')

    # Send HTTP PUT request using curl
    curl -X PUT -d "$key=$value" http://localhost:8080

done < "$1"  
