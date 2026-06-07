#ifndef SCREEN_MANAGER
#define SCREEN_MANAGER

/**
 * @file ScreenManager.hpp
 * @brief Orquestra a navegação entre as telas.
 * @details Substitui o switch(targetScreen) do loop() antigo. Mantém a tela
 * atual e, a cada passo, executa run() na tela ativa e troca para a tela
 * que ela retornar.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "Screen/Screen.hpp"
#include "Screen/Playback.hpp"

class ScreenManager
{
public:
    ScreenManager(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                  EnginesSet &p_guitar, SDCard &p_sd_card, Playback &p_playback);

    /**
    * @brief Executa um ciclo: roda a tela atual e troca para a próxima.
    * @details Deve ser chamada dentro do loop() do Arduino.
    */
    void update();

private:
    Adafruit_ST7735 &m_tft;
    Joystick &m_joystick;
    EnginesSet &m_guitar;
    SDCard &m_sd_card;
    Playback &m_playback;

    ScreenId m_current_screen;
};

#endif
