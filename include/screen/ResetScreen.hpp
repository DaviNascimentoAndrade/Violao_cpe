#ifndef RESET_SCREEN
#define RESET_SCREEN

/**
 * @file ResetScreen.hpp
 * @brief Tela de ajuste/reset dos motores.
 * @details Substitui resetEngines() (seleção de corda) e adjustEngine()
 * (submenu Subir/Descer/Pronto) do código antigo. A seleção de corda
 * reaproveita o mesmo layout da TuneScreen.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "Screen/Screen.hpp"

/**
* @brief Submenu de ajuste fino de um motor (Subir / Descer / Pronto).
* @details Equivale à função adjustEngine() do código antigo.
*/
class AdjustScreen : public Screen
{
public:
    AdjustScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                 EnginesSet &p_guitar, SDCard &p_sd_card, int p_guitar_string);

protected:
    void drawScreen() override;
    bool onSelect() override;

private:
    int m_guitar_string;   // índice da corda em EnginesSet (0..5)
    char m_guitar_string_chr; // caractere da corda (E,A,D,G,B,e)
};

class ResetScreen : public Screen
{
public:
    ResetScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                EnginesSet &p_guitar, SDCard &p_sd_card);

    ScreenId run() override;

protected:
    void drawScreen() override;
    bool onSelect() override;
};

#endif
