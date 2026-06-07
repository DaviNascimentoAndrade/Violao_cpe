#include "Screen/MusicScreen.hpp"
#include <Arduino.h>

MusicScreen::MusicScreen(Adafruit_ST7735 &p_tft, Joystick &p_joystick,
                         EnginesSet &p_guitar, SDCard &p_sd_card,
                         Playback &p_playback)
    : Screen(p_tft, p_joystick, p_guitar, p_sd_card), m_playback(p_playback)
{
    this->m_music_names = m_sd_card.getList();
    this->m_music_count = this->m_music_names.size();
    this->m_menu_pos = 1;
    this->m_present_screen = 0;
    this->m_selected_music = 0;
    this->m_back_selected = false;

    // Número de páginas: 10 músicas por página, +1 se sobrar resto.
    this->m_screen_count = this->m_music_count / 10;
    if (this->m_music_count % 10 != 0)
    {
        this->m_screen_count += 1;
    }
    // Posição máxima do cursor: músicas + uma opção Voltar por página.
    this->m_menu_pos_max = this->m_music_count + this->m_screen_count;
}

void MusicScreen::drawMusicList()
{
    m_tft.setCursor(0, 0);
    m_tft.setTextSize(2);
    m_tft.println("Musicas:");
    m_tft.setTextSize(1);
    m_tft.println("");

    // Faixa de músicas exibidas na página atual.
    int first_index = (10 * this->m_present_screen) + 1;
    int last_index = (10 * (this->m_present_screen + 1));
    if (last_index > this->m_music_count)
    {
        last_index = this->m_music_count;
    }

    for (int j = first_index; j <= last_index; j++)
    {
        // Compensa as opções Voltar das páginas anteriores no índice do cursor.
        if ((this->m_menu_pos - this->m_present_screen) == j)
        {
            m_tft.write(16);
            this->m_selected_music = j;
        }
        else
        {
            m_tft.print(" ");
        }
        m_tft.print(j);
        m_tft.print("-");
        m_tft.println(this->m_music_names[j - 1].c_str());
    }

    // Opção Voltar: fim da página (múltiplo de 11) ou fim geral da lista.
    bool cursor_on_back = (this->m_menu_pos == this->m_menu_pos_max) ||
                          (this->m_menu_pos % 11 == 0);
    if (cursor_on_back)
    {
        m_tft.write(16);
        m_tft.print("Voltar ");
        this->m_back_selected = true;
    }
    else
    {
        m_tft.print(" Voltar ");
        this->m_back_selected = false;
    }
    delay(10);
}

ScreenId MusicScreen::run()
{
    clearScreen();
    bool keep_running = true;

    while (keep_running)
    {
        readButtons();
        drawMusicList();

        // Cursor para cima; ao cruzar o topo da página, volta uma página.
        if (this->m_button_up == 0 && this->m_menu_pos != 1)
        {
            m_tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
            this->m_menu_pos--;
            if (this->m_menu_pos % 11 == 0)
            {
                this->m_present_screen--;
                clearScreen();
            }
            delay(BUTTON_DELAY);
        }
        // Cursor para baixo; ao cruzar o fim da página, avança uma página.
        if (this->m_button_down == 0 && this->m_menu_pos != this->m_menu_pos_max)
        {
            m_tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
            if (this->m_menu_pos % 11 == 0)
            {
                this->m_present_screen++;
                clearScreen();
            }
            this->m_menu_pos++;
            delay(BUTTON_DELAY);
        }
        // Seleção.
        if (this->m_button_select == 0)
        {
            if (this->m_back_selected)
            {
                this->m_next_screen = SCREEN_MENU;
                keep_running = false;
            }
            else
            {
                showMusicMenu(this->m_selected_music);
                clearScreen();
            }
            delay(BUTTON_DELAY);
        }
    }

    clearScreen();
    return this->m_next_screen;
}

void MusicScreen::showMusicMenu(int p_music)
{
    clearScreen();

    std::string first_stroke = m_sd_card.readFile(p_music, '1');
    std::string second_stroke = m_sd_card.readFile(p_music, '2');
    std::string third_stroke = m_sd_card.readFile(p_music, '3');

    m_guitar.setEngineSpeed(m_sd_card);
    m_guitar.setSubdivision(m_sd_card);

    int stroke_count = 3;
    if (second_stroke.length() == 0 && third_stroke.length() == 0)
    {
        stroke_count = 1;
    }
    else if (second_stroke.length() != 0 && third_stroke.length() == 0)
    {
        stroke_count = 2;
    }

    bool keep_running = true;
    int confirm_pos = 1; // 1 = Tocar, 2 = Voltar

    while (keep_running)
    {
        if (!m_playback.isPlaying())
        {
            m_guitar.endMusic();
        }

        readButtons();

        m_tft.setCursor(0, 0);
        m_tft.setTextSize(2);
        m_tft.print(this->m_music_names[p_music - 1].c_str());
        m_tft.println(":");
        m_tft.println("");
        m_tft.print("-");
        m_tft.println(m_sd_card.authorName.c_str());
        m_tft.setCursor(0, 80);
        if (confirm_pos == 1)
        {
            m_tft.write(16);
            m_tft.println("Tocar");
            m_tft.println(" Voltar");
        }
        else
        {
            m_tft.println(" Tocar");
            m_tft.write(16);
            m_tft.println("Voltar");
        }

        if (this->m_button_up == 0 && confirm_pos != 1)
        {
            confirm_pos--;
            m_tft.fillRect(0, 80, 10, 150, ST7735_BLACK);
            delay(BUTTON_DELAY);
        }
        if (this->m_button_down == 0 && confirm_pos != 2)
        {
            confirm_pos++;
            m_tft.fillRect(0, 80, 10, 150, ST7735_BLACK);
            delay(BUTTON_DELAY);
        }
        if (this->m_button_select == 0)
        {
            clearScreen();
            if (confirm_pos == 1)
            {
                showStrokeMenu(first_stroke, second_stroke, third_stroke,
                               stroke_count);
            }
            else
            {
                keep_running = false;
            }
            delay(BUTTON_DELAY);
        }
    }
}

void MusicScreen::showStrokeMenu(const std::string &p_first,
                                 const std::string &p_second,
                                 const std::string &p_third, int p_stroke_count)
{
    clearScreen();
    m_guitar.setDelay();

    bool keep_running = true;
    int playing_pos = 1; // batida atual (1..p_stroke_count)

    while (keep_running)
    {
        readButtons();

        const std::string *current_stroke = &p_first;
        if (playing_pos == 2)
        {
            current_stroke = &p_second;
        }
        else if (playing_pos == 3)
        {
            current_stroke = &p_third;
        }

        m_tft.setCursor(35, 0);
        m_tft.setTextSize(2);
        m_tft.print("BATIDA ");
        m_tft.println(playing_pos);
        m_tft.setCursor(0, 55);
        m_tft.write(27); // seta esquerda
        m_tft.setCursor(148, 55);
        m_tft.write(26); // seta direita
        m_tft.setCursor(40, 100);
        m_tft.write(16);
        m_tft.println("Voltar");

        // Inicia o loop de reprodução da batida atual, se ainda não tocando.
        if (!m_playback.isPlaying())
        {
            m_playback.startLoop(*current_stroke);
        }

        m_guitar.getEnginePos(m_sd_card);

        // Navegação circular entre as batidas disponíveis.
        if (this->m_button_up == 0)
        {
            m_tft.fillRect(114, 0, 20, 20, ST7735_BLACK);
            playing_pos--;
            if (playing_pos < 1)
            {
                playing_pos = p_stroke_count;
            }
            delay(BUTTON_DELAY);
        }
        if (this->m_button_down == 0)
        {
            m_tft.fillRect(114, 0, 20, 20, ST7735_BLACK);
            playing_pos++;
            if (playing_pos > p_stroke_count)
            {
                playing_pos = 1;
            }
            delay(BUTTON_DELAY);
        }
        if (this->m_button_select == 0)
        {
            m_playback.stopLoop();
            keep_running = false;
            delay(BUTTON_DELAY);
        }
    }
    clearScreen();
}
