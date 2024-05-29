#!/bin/bash

for file in pgns/*
do
	if [ -f "$file" ]; then
		echo "Processing File: $file"
		grep "FEN" $file | grep -o '".*"' | grep " b " | sed 's/"//g; s/[^a-zA-Z]*$//' >> ExtractedFENs.txt
	fi
done

echo 'FENs Saved to "ExtractedFENs.txt"'