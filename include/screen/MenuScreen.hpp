#ifndef MENU_SCREEN
#define MENU_SCREEN

/**
 * @file MenuScreen.hpp
 * @brief Tela de menu inicial (Musicas / Afinar / Resetar).
 * @details Substitui a função settings() do código antigo.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "Screen/Screen.hpp"

class MenuScreen : public Screen
{
public:
    MenuScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
               EnginesSet &p_guitar, SDCard &p_sd_card);

protected:
    void drawScreen() override;
    bool onSelect() override;
};

#endif
