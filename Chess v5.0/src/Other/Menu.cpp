#include "../../headers/Other/Menu.h"

Menu::Menu() {
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

void Menu::ShowMenu() {
    StartSingleplayer = GuiButton(StartButton, "Start");
    StartMultiplayer = GuiButton(MultiplayerButton, "Multiplayer");
    LoadFromFen = GuiButton(FENButton, "Load From FEN\n(New)");
    OpenSettings = GuiButton(SettingsMenu, "Settings\n(New)");
}

bool Menu::isOptionPressed() const { return StartSingleplayer || StartMultiplayer; }

void Menu::DrawMenuBox() const { DrawRectangleRounded(InfoBox, Roundedness, Segments, Translucent); }
