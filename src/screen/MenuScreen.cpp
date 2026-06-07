#include "Screen/MenuScreen.hpp"
#include <Arduino.h>

// Itens do menu, na ordem do cursor (0..2).
static const char *MENU_ITEMS[] = {"Musicas", "Afinar", "Resetar"};
static const int MENU_ITEM_COUNT = 3;

MenuScreen::MenuScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                       EnginesSet &p_guitar, SDCard &p_sd_card)
    : Screen(p_tft, p_joystick, p_guitar, p_sd_card)
{
    this->m_cursor_min = 0;
    this->m_cursor_max = MENU_ITEM_COUNT - 1;
    this->m_cursor_pos = 0;
}

void MenuScreen::drawScreen()
{
    m_tft.setCursor(0, 0);
    m_tft.setTextSize(3);
    m_tft.println("Menu:");
    m_tft.setTextSize(2);
    m_tft.println("");

    for (int i = 0; i < MENU_ITEM_COUNT; i++)
    {
        if (i == this->m_cursor_pos)
        {
            m_tft.write(16); // seta do cursor
        }
        else
        {
            m_tft.print(" ");
        }
        m_tft.println(MENU_ITEMS[i]);
    }
}

bool MenuScreen::onSelect()
{
    switch (this->m_cursor_pos)
    {
    case 0:
        this->m_next_screen = SCREEN_MUSIC;
        break;
    case 1:
        this->m_next_screen = SCREEN_TUNE;
        break;
    case 2:
        this->m_next_screen = SCREEN_RESET;
        break;
    }
    return false; // sai do run() e troca de tela
}
