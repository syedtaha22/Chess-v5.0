# Chess Engine Test

## Overview

The `test_engine.cpp` file is designed to evaluate the functionality of the chess engine by analyzing various board positions represented in Forsyth-Edwards Notation (FEN). It utilizes command-line arguments to process FEN strings and provide feedback on the engine's performance, including the best move found, time taken for analysis, and search depth.

## Building the Test File

To build the `test_engine.cpp` file, follow these steps:

1. **Open Terminal:** Launch the terminal or command prompt on your system.
2. **Navigate to Directory:** Use the `cd` command to navigate to the directory containing the `test_engine.cpp` file.
3. **Compile the File:** Compile the `test_engine.cpp` file using the following command:

```bash
g++ -o test_engine.exe test_engine.cpp ../src/Engine/EngineState.cpp ../headers/Engine/ChessEngineStatistics.h ../src/Engine/ChessEngine.cpp ../src/Board/ChessBoard.cpp ../src/GameFlow/Flags.cpp ../src/Engine/TranspositionTables.cpp ../src/Other/GlobalVariables.cpp ../src/GameFlow/Settings.cpp ../src/Pieces/ChessPiece.cpp ../src/Pieces/Pieces.cpp ../src/Functors/ConvertNotation.cpp ../src/Exceptions/KingNotFound.cpp -std=c++20 -ljsoncpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

**Build Success:** Upon successful compilation, an executable file named test_engine.exe will be generated in the same directory.

## Testing Procedure

**Prepare FEN Database**

A file named ExtractedFENs.txt should be present in the directory. If not, run FenExtractor.sh using:

```bash
./FenExtractor.sh
```

**Execute Test Engine**

After the executable and FEN database are ready, use `start_test.sh` to run the test.

```bash
Usage: $0 <FEN database file> [loop_limit] [Depth]
```

By default, depth is set to two. For example:

```bash
./start_test.sh ExtractedFENs.txt
./start_test.sh ExtractedFENs.txt 10
./start_test.sh ExtractedFENs.txt 10 3
./start_test.sh ExtractedFENs.txt -1 3
```

Explanation:

1) Tests the engine on all the FENs in the database.
2) Tests the engine on the first 10 FENs in the database.
3) Tests the engine on the first 10 FENs in the database at depth three.
4) Tests the engine on all the FENs in the database at depth three.


## Analysis and Feedback


The test engine will process each FEN string from the database, analyzing the position and providing feedback on the engine's performance. Feedback may include:

- Best move found.
- Time taken for analysis.
- Search depth at which analyzed.
- Other relevant statistics.

**Save Data**
Data generated during testing, including FEN strings, analysis results, and performance metrics, will be saved to a .json file for further analysis.

## Conclusion
The test_engine.cpp file serves as a valuable tool for evaluating the chess engine's performance under various conditions and board positions. By analyzing feedback and data generated during testing, we can identify areas for improvement and optimize the engine's algorithms for enhanced gameplay and efficiency.
