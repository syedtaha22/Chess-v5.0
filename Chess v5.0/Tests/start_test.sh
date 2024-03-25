#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <FEN database file>"
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

# Iterate over each FEN string in the file
while IFS= read -r fen; do
    # Execute your C++ program with the FEN string as argument
    "$CPP_PROGRAM" "$fen"
done < "$FEN_FILE"

