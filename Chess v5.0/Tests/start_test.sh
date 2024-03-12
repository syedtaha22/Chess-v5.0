#!/bin/bash

# Path to the text file containing FEN strings
FEN_FILE="FENDataBase.txt"

# Path to your C++ program
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

