#ifndef MENU_H
#define MENU_H

#include "GlobalVariables.h"

class Menu{


    Rectangle InfoBox;
    Rectangle StartButton;
    Rectangle MultiplayerButton;
    Rectangle FENButton;
    Rectangle SettingsMenu;

    const float height = 70;
    const float width = 200;

    const float buttonX = (InfoBoxWidth / 2) - 40;
    const float StartButtonY = 185;
    const float MultiPlayerbuttonY = 305;
    const float FenButtonY = 425;
    const float SettingMenuY = 545;

    /*
        Define in GlobalVariable.h : 

        float InfoBoxX = 60;
        float InfoBoxY = 80;
        float InfoBoxWidth = 610;
        float InfoBoxHeight = 640;
    */


public:
    bool StartSingleplayer;
    bool StartMultiplayer;
    bool LoadFromFen;
    bool OpenSettings;

    /*
       StartButtonY       = 80 + 160                   -  10 - 45 ---> (-  10 - 45 - height/2 = 50) --> Distance b/w Buttons
       MultiPlayerbuttonY = 80 + 160 + 160             -  50 - 45 ---> (-  50 - 45 - height/2 = 50) --> Distance b/w Buttons 
       FenButtonY         = 80 + 160 + 160 + 160       -  90 - 45 ---> (-  90 - 45 - height/2 = 50) --> Distance b/w Buttons
       SettingMenuY       = 80 + 160 + 160 + 160 + 160 - 130 - 45 ---> (- 130 - 45 - height/2 = 50) --> Distance b/w Buttons  
    
    ------------------------------------------------------Other Things------------------------------------------------------

                                     80 = Distance from top of Screen top to InfoBox
                                    160 = InfoBoxHeight/4



    */


    Menu() {
        InfoBox = { InfoBoxX, InfoBoxY, InfoBoxWidth, InfoBoxHeight };
        StartButton = { buttonX, StartButtonY, width, height };
        MultiplayerButton = { buttonX, MultiPlayerbuttonY, width, height };
        FENButton = { buttonX, FenButtonY, width, height };
        SettingsMenu = { buttonX, SettingMenuY , width, height };



        StartSingleplayer = false;
        StartMultiplayer = false;
        LoadFromFen = false;
        OpenSettings = false;

    }

    void ShowMenu() {

        StartSingleplayer = GuiButton(StartButton, "Start");
        StartMultiplayer = GuiButton(MultiplayerButton, "Multiplayer");
        LoadFromFen = GuiButton(FENButton, "Load From FEN\n(New)");
        OpenSettings = GuiButton(SettingsMenu, "Settings\n(New)");

    }

    bool isOptionPressed() const {
        return StartSingleplayer || StartMultiplayer;
    }

    void DrawMenuBox() {
        DrawRectangleRounded(InfoBox, Roundedness, Segments, Translucent);
    }





};




#endif // !MENU_H



