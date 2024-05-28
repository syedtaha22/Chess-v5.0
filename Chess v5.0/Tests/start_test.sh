# Date added: 29th May, 2024

# This is an automated script to compile, run, and analyze the Engine
# It starts of by compiling the test_engine.cpp program if the execuatable is not present
# Then it runs the program ./test_engine.exe
# Then Generates Stockfish results
# And finally compares the Two to score the Engine
# -f : flag to force compile the .cpp program
# -n : flag to provide the number of Fens to test against (Defualt = 10)


#!/bin/bash

# Set the default value for force_compile
force_compile=false

# Set the Defualt Number of Fens to analyze
num_fens=10

# Set the search depth for Engine
depth=1

# Parse command line options
if [[ $1 == "-f" ]]; then
    compile_even_if_exists=true
    shift
fi

if [[ $1 == "-n" ]]; then
    num_fens=$2
    shift 2
fi

# Define the name of the executable
executable="test_engine.exe"

# Check if the executable file exists and force_compile is false
if ! $force_compile && [ -f "$executable" ]; then
    echo "Executable '$executable' already exists. Proceeding with execution."
else
    echo "Executable '$executable' not found or force compilation enabled. Compiling C++ code..."

    # Compile the C++ code
    if g++ -o "$executable" test_engine.cpp ../src/Engine/EngineState.cpp ../headers/Engine/ChessEngineStatistics.h ../src/Engine/ChessEngine.cpp ../src/Board/ChessBoard.cpp ../src/GameFlow/Flags.cpp ../src/Engine/TranspositionTables.cpp ../src/Other/GlobalVariables.cpp ../src/GameFlow/Settings.cpp ../src/Pieces/ChessPiece.cpp ../src/Pieces/Pieces.cpp ../src/Functors/ConvertNotation.cpp ../src/Exceptions/KingNotFound.cpp -std=c++20 -ljsoncpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11; then
        echo "Compilation successful!"
    else
        echo "Compilation failed!"
        exit 1
    fi
fi

# Run the script to analyze fens with Engine
./feed_fens.sh ExtractedFENs.txt $num_fens $depth

# Run a Python script for analysis with Stockfish
python3 analyze_with_stockfish.py ExtractedFENs.txt $num_fens
# Check if Stockfish analysis executed successfully
if [ $? -eq 0 ]; then
    echo "Stockfish Analysis Complete"
    # Run another Python script for calculating scores
    python3 calculate_score.py
else
    echo "Stockfish Analysis Failed!"
fi
