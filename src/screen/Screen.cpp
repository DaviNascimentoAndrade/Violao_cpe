#include "Screen/Screen.hpp"
#include <Arduino.h>

Screen::Screen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
               EnginesSet &p_guitar, SDCard &p_sd_card)
    : m_tft(p_tft), m_joystick(p_joystick),
      m_guitar(p_guitar), m_sd_card(p_sd_card)
{
    this->m_cursor_pos = 0;
    this->m_cursor_min = 0;
    this->m_cursor_max = 0;
    this->m_next_screen = SCREEN_MENU;
    this->m_button_up = 1;
    this->m_button_select = 1;
    this->m_button_down = 1;
}

void Screen::readButtons()
{
    // Mesma convenção do código antigo: 0 = pressionado/acionado.
    this->m_button_up = !(m_joystick.X_asButtonUp());
    this->m_button_select = !(m_joystick.get_button());
    this->m_button_down = !(m_joystick.X_asButtonDown());
}

void Screen::moveCursorUp()
{
    if (this->m_cursor_pos > this->m_cursor_min)
    {
        this->m_cursor_pos--;
    }
    delay(BUTTON_DELAY);
}

void Screen::moveCursorDown()
{
    if (this->m_cursor_pos < this->m_cursor_max)
    {
        this->m_cursor_pos++;
    }
    delay(BUTTON_DELAY);
}

void Screen::clearScreen()
{
    m_tft.fillScreen(ST7735_BLACK);
}

ScreenId Screen::run()
{
    clearScreen();
    bool keep_running = true;

    while (keep_running)
    {
        readButtons();
        drawScreen();

        if (this->m_button_up == 0)
        {
            clearScreen();
            moveCursorUp();
        }
        if (this->m_button_down == 0)
        {
            clearScreen();
            moveCursorDown();
        }
        if (this->m_button_select == 0)
        {
            keep_running = onSelect();
            delay(BUTTON_DELAY);
        }
    }

    clearScreen();
    return this->m_next_screen;
}
