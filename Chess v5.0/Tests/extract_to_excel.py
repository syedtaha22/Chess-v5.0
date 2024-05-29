#Date Added: 29th May 2024

# Simple Program to Convert Larger Test Results to .xlsx for readability and any futher analysis

import pandas as pd
import json

# Read data from JSON file
with open('Test_Results/analysis_results.json', 'r') as file:
    data = json.load(file)

# Convert JSON data to DataFrame
df = pd.DataFrame(data)

# Write DataFrame to Excel
df.to_excel('output.xlsx', index=False)