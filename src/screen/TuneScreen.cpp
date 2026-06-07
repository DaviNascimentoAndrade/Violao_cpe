#include "Screen/TuneScreen.hpp"
#include <Arduino.h>

// Cordas na ordem do cursor (0..5). O índice 6 é "Voltar".
static const char *STRING_NAMES[] = {"Corda E", "Corda A", "Corda D",
                                     "Corda G", "Corda B", "Corda e"};
static const int STRING_COUNT = 6;
static const int BACK_POS = STRING_COUNT; // posição 6 = Voltar

TuneScreen::TuneScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                       EnginesSet &p_guitar, SDCard &p_sd_card,
                       Playback &p_playback)
    : Screen(p_tft, p_joystick, p_guitar, p_sd_card), m_playback(p_playback)
{
    this->m_cursor_min = 0;
    this->m_cursor_max = BACK_POS;
    this->m_cursor_pos = 0;
}

ScreenId TuneScreen::run()
{
    enableSwitch = 0; // desliga atualização de motores enquanto afina
    ScreenId next = Screen::run();
    enableSwitch = 1;
    return next;
}

void TuneScreen::drawScreen()
{
    m_tft.setCursor(0, 0);
    m_tft.setTextSize(2);
    m_tft.println("Selecione:");
    m_tft.setTextSize(1);
    m_tft.println("");

    // Substitui os 7 blocos if(tunePos==N) repetidos do código antigo
    // por um único loop que marca o item sob o cursor.
    for (int i = 0; i < STRING_COUNT; i++)
    {
        if (i == this->m_cursor_pos)
        {
            m_tft.write(16);
        }
        else
        {
            m_tft.print(" ");
        }
        m_tft.println(STRING_NAMES[i]);
    }

    if (this->m_cursor_pos == BACK_POS)
    {
        m_tft.write(16);
        m_tft.println("Voltar");
    }
    else
    {
        m_tft.println(" Voltar");
    }
    delay(10);
}

bool TuneScreen::onSelect()
{
    if (this->m_cursor_pos != BACK_POS) // escolheu uma corda
    {
        m_playback.tuneAsync(this->m_cursor_pos);
        return true; // permanece na tela
    }

    // escolheu voltar
    m_guitar.endMusic();
    this->m_next_screen = SCREEN_MENU;
    return false;
}
