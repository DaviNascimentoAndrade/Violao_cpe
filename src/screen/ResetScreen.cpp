#include "Screen/ResetScreen.hpp"
#include <Arduino.h>

static const char *RESET_STRING_NAMES[] = {"Corda E", "Corda A", "Corda D",
                                           "Corda G", "Corda B", "Corda e"};
static const int RESET_STRING_COUNT = 6;
static const int RESET_BACK_POS = RESET_STRING_COUNT;

static const char *GUITAR_STRINGS = "EADGBe";

// ---------------------------------------------------------------------------
// AdjustScreen (era adjustEngine)
// ---------------------------------------------------------------------------

static const char *ADJUST_ITEMS[] = {"Subir", "Descer", "Pronto"};
static const int ADJUST_ITEM_COUNT = 3;

AdjustScreen::AdjustScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                           EnginesSet &p_guitar, SDCard &p_sd_card,
                           int p_guitar_string)
    : Screen(p_tft, p_joystick, p_guitar, p_sd_card)
{
    this->m_guitar_string = p_guitar_string;
    this->m_guitar_string_chr = GUITAR_STRINGS[p_guitar_string];
    this->m_cursor_min = 0;
    this->m_cursor_max = ADJUST_ITEM_COUNT - 1;
    this->m_cursor_pos = 0;
}

void AdjustScreen::drawScreen()
{
    m_tft.setCursor(0, 0);
    m_tft.setTextSize(2);
    m_tft.print("Corda ");
    m_tft.print(this->m_guitar_string_chr);
    m_tft.println(":");
    m_tft.println("");

    for (int i = 0; i < ADJUST_ITEM_COUNT; i++)
    {
        if (i == this->m_cursor_pos)
        {
            m_tft.write(16);
        }
        else
        {
            m_tft.print(" ");
        }
        m_tft.println(ADJUST_ITEMS[i]);
    }
}

bool AdjustScreen::onSelect()
{
    if (this->m_cursor_pos == 0) // Subir
    {
        m_guitar.playOneStep(this->m_guitar_string, 1);
        m_guitar.getEnginePos(m_sd_card);
        return true;
    }
    if (this->m_cursor_pos == 1) // Descer
    {
        m_guitar.playOneStep(this->m_guitar_string, -1);
        m_guitar.getEnginePos(m_sd_card);
        return true;
    }
    // Pronto
    m_guitar.runHalfTarget(this->m_guitar_string);
    this->m_next_screen = SCREEN_RESET;
    return false;
}

// ---------------------------------------------------------------------------
// ResetScreen (era resetEngines)
// ---------------------------------------------------------------------------

ResetScreen::ResetScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                         EnginesSet &p_guitar, SDCard &p_sd_card)
    : Screen(p_tft, p_joystick, p_guitar, p_sd_card)
{
    this->m_cursor_min = 0;
    this->m_cursor_max = RESET_BACK_POS;
    this->m_cursor_pos = 0;
}

ScreenId ResetScreen::run()
{
    enableSwitch = 0;
    ScreenId next = Screen::run();
    enableSwitch = 1;
    return next;
}

void ResetScreen::drawScreen()
{
    m_tft.setCursor(0, 0);
    m_tft.setTextSize(2);
    m_tft.println("Selecione:");
    m_tft.setTextSize(1);
    m_tft.println("");

    for (int i = 0; i < RESET_STRING_COUNT; i++)
    {
        if (i == this->m_cursor_pos)
        {
            m_tft.write(16);
        }
        else
        {
            m_tft.print(" ");
        }
        m_tft.println(RESET_STRING_NAMES[i]);
    }

    if (this->m_cursor_pos == RESET_BACK_POS)
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

bool ResetScreen::onSelect()
{
    if (this->m_cursor_pos != RESET_BACK_POS) // escolheu uma corda
    {
        AdjustScreen adjust(m_tft, m_joystick, m_guitar, m_sd_card,
                            this->m_cursor_pos);
        adjust.run();
        // ao voltar do ajuste, redesenha a lista do zero
        clearScreen();
        return true;
    }

    // escolheu voltar
    this->m_next_screen = SCREEN_MENU;
    return false;
}
