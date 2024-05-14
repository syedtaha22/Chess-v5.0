#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -lt 1 ] || [ "$#" -gt 3 ]; then
    echo "Usage: $0 <FEN database file> [loop_limit] [Depth]"
    exit 1
fi

# Path to the text file containing FEN strings
FEN_FILE="$1"

CPP_PROGRAM="./test_engine.exe"

# Check if the FEN file exists
if [ ! -f "$FEN_FILE" ]; then
    echo "Error: FEN file '$FEN_FILE' not found."
    exit 1
fi

# Check if the C++ program exists
if [ ! -f "$CPP_PROGRAM" ]; then
    echo "Error: C++ program '$CPP_PROGRAM' not found."
    exit 1
fi

# Set the loop limit
if [ "$#" -eq 2 ]; then
    LOOP_LIMIT=$2
else
    LOOP_LIMIT=-1  # Default value, meaning run the loop for the complete file
fi

# Set the Depth
if [ "$#" -eq 3 ]; then
    Depth=$3
else
    Depth=2  # Default value
fi

# Iterate over each FEN string in the file
counter=0
while IFS= read -r fen && ([ "$LOOP_LIMIT" -eq -1 ] || [ "$counter" -lt "$LOOP_LIMIT" ]); do
    # Execute your C++ program with the FEN string as argument
    "$CPP_PROGRAM" "$fen" "$Depth"
    ((counter++))
done < "$FEN_FILE"
