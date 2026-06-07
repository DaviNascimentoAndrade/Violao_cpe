#ifndef MUSIC_SCREEN
#define MUSIC_SCREEN

/**
 * @file MusicScreen.hpp
 * @brief Tela de seleção de músicas, confirmação e escolha de batida.
 * @details Reúne as funções menu() (lista paginada), music() (confirmação)
 * e strokes() (menu de batidas) do código antigo. A reprodução em si é
 * delegada à interface Playback.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "Screen/Screen.hpp"
#include "Screen/Playback.hpp"
#include <vector>
#include <string>

class MusicScreen : public Screen
{
public:
    MusicScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                EnginesSet &p_guitar, SDCard &p_sd_card, Playback &p_playback);

    /**
    * @brief Executa a tela de seleção paginada de músicas.
    * @details Sobrescreve run() porque a navegação tem paginação
    * (10 músicas por página) e não se encaixa no cursor simples da base.
    */
    ScreenId run() override;

protected:
    // Não usados nesta tela (navegação própria), mas exigidos pela base.
    void drawScreen() override {}
    bool onSelect() override { return false; }

private:
    /**
    * @brief Desenha a página atual da lista de músicas.
    * @details Marca o item sob o cursor e a opção Voltar.
    */
    void drawMusicList();

    /**
    * @brief Mostra a confirmação de uma música (Tocar / Voltar).
    * @param p_music Posição numérica da música no cartão SD.
    */
    void showMusicMenu(int p_music);

    /**
    * @brief Menu de escolha de batida durante a reprodução.
    * @param p_first Primeira sequência de cordas.
    * @param p_second Segunda sequência.
    * @param p_third Terceira sequência.
    * @param p_stroke_count Número de sequências disponíveis.
    */
    void showStrokeMenu(const std::string &p_first, const std::string &p_second,
                        const std::string &p_third, int p_stroke_count);

    Playback &m_playback;

    std::vector<std::string> m_music_names; // nomes das músicas
    int m_music_count;                       // quantidade de músicas
    int m_menu_pos;                          // posição do cursor (1-based)
    int m_menu_pos_max;                      // posição máxima (inclui Voltar)
    int m_screen_count;                      // número de páginas
    int m_present_screen;                    // página atual
    int m_selected_music;                    // música sob o cursor
    bool m_back_selected;                    // cursor está em "Voltar"
};

#endif
