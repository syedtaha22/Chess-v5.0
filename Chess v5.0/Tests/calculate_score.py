# Date Added: 29th May, 2024

# Calculate the Score of the Engine based on Test results.
# Compares the Engines analysis against Stockfish analysis to Assign a Score to the Engine
# Formula used for Score Calculation is
    # weighted_CMC_score = (CMC / entries) * weightage_CMC
    # weighted_CM_score = (CM / entries) * weightage_CM
    # Score = weighted_CMC_score + weighted_CM_score
# CMC = correct move count(Number of Moves found in position was same)
# CM = correct move

# Correct Move Count has a higher weightage, as multiple good moves could exist
# in a given scenario, and lead to different answers

import json # Import Json for json file handling

weight_CMC = 7 #Weightage of Correct Move Count
weight_CM = 3 #Weightage of Correct Move

def calculate_score(engine_res, stockfish_res):
    
    # Load The data from results
    with open(engine_res, 'r') as Er, open(stockfish_res, 'r') as Sr:
        data_engine = json.load(Er)
        data_stockfish = json.load(Sr)

    # Initialise Scores
    correct_move_count = 0
    correct_move = 0

    # Loop over each entry in the engines analysis and comapre with corresponding entry in Stock fish Anaylysis
    for entry in data_engine:
        for entry_s in data_stockfish:
            if entry['fen'] == entry_s['fen']:
                if entry['TotalMoves'] == entry_s['TotalMoves']:
                    correct_move_count += 1
                if entry['bestMove'] == entry_s['bestMove']: 
                    correct_move += 1
                break

    # Get total entries
    total_entries = len(data_engine)
    
    # Return Score
    return ((correct_move_count / total_entries) * weight_CMC) + ((correct_move / total_entries) * weight_CM)

# Define Location to Engine Analysis
engine_results = 'Test_Results/analysis_results.json'

# Define location to Stockfish Analysis
stockfish_results = 'Test_Results/stockfish_analysis.json'

# Calculate Score
Score = calculate_score(engine_results, stockfish_results)

# Print Score
print("Score:", Score)
