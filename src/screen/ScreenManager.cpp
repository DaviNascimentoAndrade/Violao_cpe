#include "Screen/ScreenManager.hpp"
#include "Screen/MenuScreen.hpp"
#include "Screen/MusicScreen.hpp"
#include "Screen/TuneScreen.hpp"
#include "Screen/ResetScreen.hpp"

ScreenManager::ScreenManager(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                             EnginesSet &p_guitar, SDCard &p_sd_card,
                             Playback &p_playback)
    : m_tft(p_tft), m_joystick(p_joystick), m_guitar(p_guitar),
      m_sd_card(p_sd_card), m_playback(p_playback)
{
    this->m_current_screen = SCREEN_MENU;
}

void ScreenManager::update()
{
    // Instancia apenas a tela ativa, executa o seu loop e troca para a
    // tela que ela retornar. As telas são leves e criadas sob demanda,
    // preservando o comportamento do switch antigo.
    switch (this->m_current_screen)
    {
    case SCREEN_MENU:
    {
        MenuScreen screen(m_tft, m_joystick, m_guitar, m_sd_card);
        this->m_current_screen = screen.run();
        break;
    }
    case SCREEN_MUSIC:
    {
        MusicScreen screen(m_tft, m_joystick, m_guitar, m_sd_card, m_playback);
        this->m_current_screen = screen.run();
        break;
    }
    case SCREEN_TUNE:
    {
        TuneScreen screen(m_tft, m_joystick, m_guitar, m_sd_card, m_playback);
        this->m_current_screen = screen.run();
        break;
    }
    case SCREEN_RESET:
    {
        ResetScreen screen(m_tft, m_joystick, m_guitar, m_sd_card);
        this->m_current_screen = screen.run();
        break;
    }
    }
}
