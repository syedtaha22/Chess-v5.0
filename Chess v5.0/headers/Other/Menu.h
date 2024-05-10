#ifndef MENU_H
#define MENU_H

#include "GlobalVariables.h"

class Menu{

private:
    Rectangle InfoBox;
    Rectangle StartButton;
    Rectangle MultiplayerButton;
    Rectangle FENButton;
    Rectangle SettingsMenu;

    const float height = 70.0f;
    const float width = 200.0f;

    const float buttonX = (InfoBoxWidth / 2.0f) - 40.0f;
    const float StartButtonY = 185.0f;
    const float MultiPlayerbuttonY = 305.0f;
    const float FenButtonY = 425.0f;
    const float SettingMenuY = 545.0f;



    /*
        Defined in GlobalVariable.h : 

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


    Menu();
    void ShowMenu();
    bool isOptionPressed() const;
    void DrawMenuBox() const;

};


#endif // !MENU_H
