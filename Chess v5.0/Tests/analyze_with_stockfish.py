# Date Added: 29th May, 2024

# Analyze FEN strings with Stock Fish
# Get a .json file containing those results.
# These results can then be used to calculate the Accuracy of the Chess Engine

import chess
import chess.engine


import json # For json file handling
import argparse # To handle command line arguments


#Function to Analyze a FEN string with Stock Fish
def analyze_fen(fen, engine_path):
    # Create a chess board from the FEN string
    board = chess.Board(fen.strip())

    # Get all legal moves
    legal_moves = list(board.legal_moves)

    # Get the number of possible moves
    num_possible_moves = len(legal_moves)

    # Initialize Stockfish engine
    with chess.engine.SimpleEngine.popen_uci(engine_path) as engine:
        # Analyze the position
        result = engine.analyse(board, chess.engine.Limit(time=1.0))
        best_move = result['pv'][0]

    # Return a Dictionary with required info
    return {
        "fen": fen.strip(),
        "TotalMoves": num_possible_moves,
        "bestMove": best_move.uci()
    }

def main(fen_file, num_fens):
    
    # Path to the Stockfish executable
    engine_path = './stockfish.exe'

    # Output JSON file
    output_file = 'Test_Results/stockfish_analysis.json'

    with open(fen_file, 'r') as file:
        fen_strings = file.readlines()

    # Limit the number of FEN strings to analyze
    fen_strings = fen_strings[:num_fens]

    results = []
    count = 1

    for fen in fen_strings:
        try:
            result = analyze_fen(fen, engine_path)
            results.append(result)
            print("\rFEN strings Analyzed: ", count, end='', flush=True)
            count += 1
        except Exception as e:
            print(f"Error analyzing FEN {fen.strip()}: {e}")
            
    print('\n') #New Line

    # Save results to JSON file
    with open(output_file, 'w') as json_file:
        json.dump(results, json_file, indent=4)

if __name__ == "__main__":
    # Argument parser
    parser = argparse.ArgumentParser(description='Analyze chess positions from FEN strings using Stockfish.')
    parser.add_argument('fen_file', type=str, help='Path to the file containing FEN strings.')
    parser.add_argument('num_fens', type=int, help='Number of FEN strings to analyze.')

    args = parser.parse_args()

    main(args.fen_file, args.num_fens)
