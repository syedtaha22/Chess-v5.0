# Chess v5.0


## Chess Engine Overview
This is a chess engine developed in C++. The engine implements various chess algorithms, including minimax with alpha-beta pruning, transposition tables, and piece-square tables for evaluation.


## Fixes - v5.2.0
- Fixes a debug error causing an abort() call when closing the window.
- Ensures that information related to unexpected exits is now properly saved in the appdata folder.
- Adds functionality to create a 'setting.json' file to remember the last depth entered by the user.
- Enhances the application to save ELO ratings to the 'setting.json' file after calculation.


## Features
The Current Release v5.2.0 Includes The Following Features

- Singleplayer Mode(Against AI)
- Multiplayer Mode
- Loading Game From FEN
- Settings to adjust Depth
  

## Installation
To run the chess engine, follow these steps:

### Method 1
Dowload The .msi file Provided in the realeases section and follow the setup. (This will Create a shortcut on your desktop and Startmenu)

### Method 2
If you recieve a security warning, then you can download the zip file, and Run the .msi file provided there under Installer/Chess/Debug. 

### Method 3
Alternatively you can also open the porject on Visual Studio, and build the project from there


## Usage
Click on the shortcut on your Desktop. Choose a game mode 
- Start(Single Player)
- Multiplayer
- Load from FEN
- Settings
	 
By Defualt, "Load from FEN" will work Only for Multiplayer Game


## Navigating
- Press "r" to Restart
- Press "m" to return to Menu
  

## Configuration
The engine's behavior and settings can be configured in the following ways:

The Search depth Can be set inside settings. For more Control you can can navigate over to ChessEngine.h header file, and change the following Property `MAX_DEPTH`. To avoid Long search times This is set to 3 by Default. You can Change this to any value, according to your device. 


*Changing `MAX_DEPTH` will increase Search times, and might even cause the program to crash completely*


## Concepts relevant to Project
v5.2.0 uses polymorphism to Assign Chess Pieces to the Board. Instead of Using a single Class for Chess Pieces, I have derived Six Classes from the ChessPiece class, and then Dynamically Assigned Chess Pieces on the Board. (This is still a Very basic Implementation of Polymorphism


## Incase of Unexpected Exits, Weired Behaviors
Incase of any weird behavior, like message not showing on Win, Engine calculation not stopping on game end etc., Kindly do the following
- Take a ScreenShot
- Close the program
- Navigate over to AppData\Roaming\ChessData
- Email the UnexpectedExits.txt file to me on my email along with a screenshot/explanation of the error

Alternatively report an issue.
  
## Contact

For any inquiries or issues related to the chess engine, please contact me at syetaha@gmail.com.
Incase you're able to run the engine at higher Depth( Depths > 5), Do let me know.
