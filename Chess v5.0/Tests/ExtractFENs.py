import os
import re

def extract_fen_from_pgn(file_path):
    fen_list = []
    with open(file_path, 'r') as file:
        data = file.read()
        fen_matches = re.findall(r'\[FEN "(.*?)"\]', data)
        fen_list.extend(fen_matches)
    return fen_list

def FilterGames(fenList, playerTosearch):
    fen_list = []
    
    for fen in fenList:
        #print(fen)
        if f" {playerTosearch} " in fen:
            fen_list.append(fen)
    return fen_list

def simplify_fen(UnsimplifiedFEN):
    simplified_fens = []
    for fen in UnsimplifiedFEN:
        # Find the index of the last alphabet character
        last_alpha_index = 0
        for i in range(len(fen) - 1, -1, -1):
            if fen[i].isalpha():
                last_alpha_index = i
                break

        simplified_fens.append(fen[:last_alpha_index + 1])
    
    return simplified_fens


def main():
    ColorToPlay = input("Enter 'Player to play': ").strip().lower()
    if ColorToPlay not in {'w', 'b'}:
        print("Invalid choice! Please enter 'w' or 'b'.")
        return
   
    directory = 'pgns/' 
    fen_database = []

    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.pgn'):
                file_path = os.path.join(root, file)
                fen_list = extract_fen_from_pgn(file_path)
                fen_database.extend(fen_list)

    FilteredFENs = FilterGames(fen_database, ColorToPlay)
    SimplifiedFENs = simplify_fen(FilteredFENs)
    


    print(f"Games Found with {ColorToPlay} : " + str(len(SimplifiedFENs)))



    # # Save FEN strings to FENDataBase.txt
    with open('FENDataBase.txt', 'w') as f:
        for fen in SimplifiedFENs:
            f.write(fen + '\n')

    print("FEN strings extracted and saved to FENDataBase.txt")

if __name__ == "__main__":
    main()
