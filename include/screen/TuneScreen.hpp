#ifndef TUNE_SCREEN
#define TUNE_SCREEN

/**
 * @file TuneScreen.hpp
 * @brief Tela de afinação das cordas.
 * @details Substitui a função afinar() e a task taskTune() do código antigo.
 * O disparo da afinação é delegado à interface Playback.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "Screen/Screen.hpp"
#include "Screen/Playback.hpp"

class TuneScreen : public Screen
{
public:
    TuneScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
               EnginesSet &p_guitar, SDCard &p_sd_card, Playback &p_playback);

    ScreenId run() override;

protected:
    void drawScreen() override;
    bool onSelect() override;

private:
    Playback &m_playback;
};

#endif
