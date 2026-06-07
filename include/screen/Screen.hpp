#ifndef SCREEN
#define SCREEN

/**
 * @file Screen.hpp
 * @brief Classe base para todas as telas do display.
 * @version 1.0
 * @date 2026
 * @details Captura o padrão repetido nas telas antigas (settings, afinar,
 * resetEngines, adjustEngine): um cursor que sobe/desce sobre uma lista de
 * itens, lê os botões do Joystick e redesenha. As telas concretas herdam
 * desta e implementam apenas o que muda: como desenhar e o que fazer ao
 * selecionar um item.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include <Adafruit_ST7735.h>
#include <vector>
#include <string>

#include "Joystick.hpp"
#include "EnginesSet.hpp"
#include "SDCard.hpp"

/**
* @brief Identifica qual tela deve ser exibida.
* @details Substitui os "números mágicos" de targetScreen do código antigo.
*/
enum ScreenId
{
    SCREEN_MENU = 1,
    SCREEN_MUSIC = 2,
    SCREEN_TUNE = 3,
    SCREEN_RESET = 4
};

class Screen
{
public:
    /**
    * @brief Construtor da tela base. Recebe por referência o hardware e o
    * estado compartilhado, evitando o uso de variáveis globais.
    * @param p_tft Display TFT.
    * @param p_joystick Joystick de entrada.
    * @param p_guitar Conjunto de Engines.
    * @param p_sd_card Cartão SD.
    */
    Screen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
           EnginesSet &p_guitar, SDCard &p_sd_card);

    virtual ~Screen() {}

    /**
    * @brief Executa o loop da tela até que o usuário saia dela.
    * @return Próxima tela a ser exibida.
    * @details Implementa o esqueleto comum (ler botões, mover cursor,
    * redesenhar, tratar seleção). As telas concretas costumam apenas
    * sobrescrever drawScreen() e onSelect().
    */
    virtual ScreenId run();

protected:
    /**
    * @brief Desenha o conteúdo da tela na posição atual do cursor.
    * @details Cada tela concreta implementa o seu layout aqui.
    */
    virtual void drawScreen() = 0;

    /**
    * @brief Trata a seleção do item sob o cursor.
    * @return true se a tela deve continuar; false para sair de run().
    */
    virtual bool onSelect() = 0;

    /**
    * @brief Lê o estado atual dos botões para os membros m_button_*.
    */
    void readButtons();

    /**
    * @brief Move o cursor um item para cima, respeitando o limite mínimo.
    */
    void moveCursorUp();

    /**
    * @brief Move o cursor um item para baixo, respeitando o limite máximo.
    */
    void moveCursorDown();

    /**
    * @brief Limpa todo o display (preto).
    */
    void clearScreen();

    Adafruit_ST7735 &m_tft;
    Joystick &m_joystick;
    EnginesSet &m_guitar;
    SDCard &m_sd_card;

    /**
    * @brief Posição atual do cursor na lista de itens da tela.
    */
    int m_cursor_pos;

    /**
    * @brief Menor índice válido de cursor.
    */
    int m_cursor_min;

    /**
    * @brief Maior índice válido de cursor.
    */
    int m_cursor_max;

    /**
    * @brief Próxima tela; cada tela define ao sair do run().
    */
    ScreenId m_next_screen;

    /**
    * @brief Estado dos botões na última leitura (0 = pressionado).
    */
    int m_button_up;
    int m_button_select;
    int m_button_down;

    /**
    * @brief Delay anti-repique para atuação contínua dos botões.
    */
    static const int BUTTON_DELAY = 250;
};

#endif
