#include <SD.h>
#include <SPI.h>
#include "FS.h"
#include <Arduino.h>
#include <vector>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h>
#include <esp_ipc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "SDCard.hpp"
#include "Engine.hpp"
#include "EnginesSet.hpp"
#include "Joystick.hpp"

using namespace std;

SemaphoreHandle_t serialMutex;
void enableUpdateTask(void *arg);

vector<string> musicNames; // Vetor com os nomes das músicas
int musicNumber; // Quantidade de músicas armazenadas
int j;

#define Motors_Enable 17

// Variáveis do display TFT
#define TFT_MOSI 23 //também representado por SDA
#define TFT_SCLK 18
#define TFT_DC 2 // também representado por A0
#define TFT_CS 15
#define TFT_RST 4
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int delayButtons = 250; // delay do sistema em atuação contínua de botões
#define buttonSelect 16
#define JoystickIn 35

int exitLoop = 0;

int GLOBAL_SPEED = 90;
const int GLOBAL_STEPS = 200;
const int GLOBAL_TARGET = 30;

int buttonUpState; // @brief armazena a direção do tilt do joystick
int buttonSelectState; // @brief armazena o aperto do botão do joystick
int buttonDownState; // @brief armazena a direção do tilt do joystick (direção contrária)
int *enginePos; // possível variável inutilizada
int lastStroke = 0; // variável de controle, fica 0 quando o usuário que parar de tocar
int tunePos = 0;

Joystick Joy(JoystickIn,-1, buttonSelect); // Declaração da inst

EnginesSet guitar;

SDCard sdCard;
int targetScreen = 1; // @brief Armazena o número da tela que está sendo utilizada pelo usuário
string defStroke = "";

bool breakLoopFlag = false;

// References the current songLoop. Used to delete it from outside the task
TaskHandle_t loopHandle = NULL; 


// FUNÇÕES DE UTILIDADES

/**
* @brief Salva os valores atuais do joystick
*/
void readingButtons()
{
  buttonUpState = !(Joy.X_asButtonUp());
  buttonSelectState = !(Joy.get_button());
  buttonDownState = !(Joy.X_asButtonDown());
}



void taskStroke(void *parameter)
{
  readingButtons();
  guitar.parseFile(defStroke,0);
  lastStroke = 0;
  // vTaskDelete(NULL);
}

/**
* @brief Essa função é executada em paralelo e manda tocar a música
*/
void songLoop(void* parameter)
{

  guitar.standardBeat();

  guitar.setLastMillis();

  while(1)
  {
    guitar.parseFile(defStroke, 0);  

    readingButtons();
    if (breakLoopFlag) break;
  }

  lastStroke = 0;
  vTaskDelete(NULL);
}



void strokes(string firstStroke, string secondStroke, string thirdStroke, int nStrokes)
{
  tft.fillScreen(ST7735_BLACK);
  int returnPlaying = 0;
  int playingPos = 1;
  guitar.setDelay();

  while (returnPlaying == 0)
  {
    readingButtons();
    if (playingPos == 1)
    {
      tft.setCursor(35, 0);//80,0
      tft.setTextSize(2);
      tft.println("BATIDA 1");
      tft.setCursor(0, 55);//0,100
      tft.write(27);
      tft.setCursor(148, 55);//300,100
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(40, 100);//100,160
      tft.write(16);
      tft.println("Voltar");
      defStroke = firstStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        breakLoopFlag = false;
        xTaskCreatePinnedToCore(songLoop, "songLoop", 1000, NULL, 1, &loopHandle, 0);
        
        // taskStroke(NULL);
        
        // esp_ipc_call(PRO_CPU_NUM, taskStroke, NULL);
      }
    }
    if (playingPos == 2)
    {
      tft.setCursor(35, 0);
      tft.setTextSize(2);
      tft.println("BATIDA 2");
      tft.setCursor(0, 55);
      tft.write(27);
      tft.setCursor(148, 55);
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(40, 100);
      tft.write(16);
      tft.println("Voltar");
      defStroke = secondStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        breakLoopFlag = false;
        xTaskCreatePinnedToCore(songLoop, "songLoop", 1000, NULL, 1, &loopHandle, 0);
        
        // taskStroke(NULL);

        // esp_ipc_call(PRO_CPU_NUM, taskStroke, NULL);
      }
    }
    if (playingPos == 3)
    {
      //tft.setCursor(0, 0);
      tft.setCursor(35, 0);
      tft.setTextSize(2);
      tft.println("BATIDA 3");
      tft.setCursor(0, 55);
      tft.write(27);
      tft.setCursor(148, 55);
      tft.write(26);
      tft.setTextSize(2);
      tft.setCursor(40, 100);
      tft.write(16);
      tft.println("Voltar");
      defStroke = thirdStroke;
      if (lastStroke == 0)
      {
        lastStroke = 1;
        breakLoopFlag = false;
        xTaskCreatePinnedToCore(songLoop, "songLoop", 1000, NULL, 1, &loopHandle, 0);

        // taskStroke(NULL);
        
        // esp_ipc_call(PRO_CPU_NUM, taskStroke, NULL);
      }
    }

    guitar.getEnginePos(sdCard);

    if (buttonUpState == 0)
    {
      tft.fillRect(114, 0, 20, 20, ST7735_BLACK);
      if (nStrokes == 3)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 3;
        }
      }
      if (nStrokes == 2)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 2;
        }
      }
      if (nStrokes == 1)
      {
        if (playingPos != 1)
        {
          playingPos--;
        }
        else
        {
          playingPos = 1;
        }
      }
      delay(delayButtons);
    }
    if (buttonDownState == 0)
    {
      tft.fillRect(114, 0, 20, 20, ST7735_BLACK);
      if (nStrokes == 3)
      {
        if (playingPos != 3)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      if (nStrokes == 2)
      {
        if (playingPos != 2)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      if (nStrokes == 1)
      {
        if (playingPos != 1)
        {
          playingPos++;
        }
        else
        {
          playingPos = 1;
        }
      }
      delay(delayButtons);
    }
    if (buttonSelectState == 0)
    {
      if (loopHandle != NULL)
      {
        vTaskDelete(loopHandle);
        lastStroke = 0;
      }

      breakLoopFlag = true;

      returnPlaying = 1;
      delay(delayButtons);
    }
  }
  tft.fillScreen(ST7735_BLACK);

}


/**
* @brief Prepara o sistema para tocar uma música e mostra o menu de confirmação
* @param music posição numérica da música no cartão SD
*/
void music(int music)
{
  tft.fillScreen(ST7735_BLACK); // reseta o display
  // salva as sequências de notas e os dados da música a ser tocada
  string firstStroke = sdCard.readFile(music, '1');
  string secondStroke = sdCard.readFile(music, '2');
  string thirdStroke = sdCard.readFile(music, '3');

  guitar.setEngineSpeed(sdCard); // salva velocidade dos motores
  guitar.setSubdivision(sdCard); // salva subdivisão da música
  
  int nStrokes = 3; // Armazena número de seguências a serem tocadas
  if (secondStroke.length() == 0 && thirdStroke.length() == 0)
  {
    nStrokes = 1;
  }
  if (secondStroke.length() != 0 && thirdStroke.length() == 0)
  {
    nStrokes = 2;
  }

  int goBack = 0; // variável de controle do loop do menu de confirmação
  int goBackMusica = 0; // variável de controle do loop principal
  int menuPos = 1; // posição do cursor no menu de confirmação

  // Loop principal
  while (goBackMusica == 0)
  {
    // Loop que roda o menu de confirmação
    while (goBack == 0)
    {
      // se acabou de tocar, reseta a posição inicial
      if(lastStroke == 0)
      {
        guitar.endMusic();
      }

      readingButtons();
      // Impreção do menu para cada posição do cursor
      if (menuPos == 1)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(2);
        tft.print(musicNames[music - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.print("-");
        tft.println(sdCard.authorName.c_str());
        tft.setCursor(0, 80);
        tft.write(16);
        tft.println("Tocar");
        tft.println(" Voltar");
      }
      if (menuPos == 2)
      {
        tft.setCursor(0, 0);
        tft.setTextSize(2);
        tft.print(musicNames[music - 1].c_str());
        tft.println(":");
        tft.setTextSize(2);
        tft.println("");
        tft.print("-");
        tft.println(sdCard.authorName.c_str());
        tft.setCursor(0, 80);
        tft.println(" Tocar");
        tft.write(16);
        tft.println("Voltar");
      }

      // lógica de atualização das variáveis de controle do menu
      // manda o cursor para cima
      if (buttonUpState == 0 && menuPos != 1)
      {
        menuPos--;
        tft.fillRect(0, 80, 10, 150, ST7735_BLACK);
        delay(delayButtons);
      }
      // manda o cursor para baixo
      if (buttonDownState == 0 && menuPos != 2)
      {
        menuPos++;
        tft.fillRect(0, 80, 10, 150, ST7735_BLACK);
        delay(delayButtons);
      }
      // botão pressionado
      if (buttonSelectState == 0)
      {
        goBack = 1;
        tft.fillScreen(ST7735_BLACK);
        delay(delayButtons);
      }
    }
    // Lógica de escolha do menu de confirmação
    if (menuPos == 1) // escolheu tocar a musica
    {
      strokes(firstStroke, secondStroke, thirdStroke, nStrokes);
      goBack = 0;
    }
    if (menuPos == 2) // escolheu voltar
    {
      goBackMusica = 1;
    }
  }
}


/**
* @brief Chama a função de afinação do violão
*/
void taskTune(void *parameter)
{
  guitar.tune(tunePos);
  vTaskDelete(NULL);
}


/**
* @brief Função que realiza o ajuste de motores
* @param resetPos corda do motor que será ajustado
*/
void adjustEngine(int *resetPos)
{
  int guitarString = *resetPos; // Armazena a posição numérica da corda
  string guitarStrings = "EADGBe"; // string com as cordas do violão
  char guitarStringChr = guitarStrings[guitarString]; // armazena o caractere da corda
  tft.fillScreen(ST7735_BLACK); // reseta o display
  int goBack = 0; // variável de controle do loop
  int adjustPos = 0; // variável de posição do cursor

  // Loop que implementa o menu de ajuste do motor
  while (goBack == 0)
  {
    readingButtons();
    // lógica que imprime o menu de ajuste para cada posição do cursor
    if (adjustPos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Subir");
      tft.println(" Descer");
      tft.println(" Pronto");
    }
    if (adjustPos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.write(16);
      tft.println("Descer");
      tft.println(" Pronto");
    }
    if (adjustPos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.print("Corda ");
      tft.print(guitarStringChr);
      tft.println(":");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Subir");
      tft.println(" Descer");
      tft.write(16);
      tft.println("Pronto");
    }

    // Lógica de atualização das variáveis de controle
    // Manda o cursor para cima
    if (buttonUpState == 0 && adjustPos != 0)
    {
      adjustPos--;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      delay(delayButtons);
    }
    // Manda o cursor para baixo
    if (buttonDownState == 0 && adjustPos != 2)
    {
      adjustPos++;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      delay(delayButtons);
    }
    // Botão pressionado
    if (buttonSelectState == 0)
    {
      if (adjustPos == 0) // escolheu subir
      {
        guitar.playOneStep(guitarString, 1);
        guitar.getEnginePos(sdCard);
      }
      if (adjustPos == 1) // escolheu descer
      {
        guitar.playOneStep(guitarString, -1);
        guitar.getEnginePos(sdCard);
      }
      if (adjustPos == 2) // escolheu o pronto
      {
        guitar.runHalfTarget(guitarString);
        tft.fillScreen(ST7735_BLACK);
        goBack = 1;
        *resetPos = 1;
        delay(delayButtons);
      }
    }
  }
}


// FUNÇÕES DO MENU PRINCIPAL

/**
* @brief Executa a tela de menu inicial
* @param targetScreen Número da tela que está sendo utilizada pelo usuário
*/
void settings(int *targetScreen)
{
  tft.fillScreen(ST7735_BLACK); // Reseta o display
  int goBack = 0; // Variável de controle do loop
  int posSettings = 2; // Indica qual opção será escolhida pelo usuário

  // Loop que executa o menu
  while (goBack == 0)
  {
    readingButtons();

    // Gráficos de cada opção, controlados pela variável de escolha
    if (posSettings == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.write(16);
      tft.println("Musicas");
      tft.println(" Afinar");
      tft.println(" Resetar");
    }
    if (posSettings == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Musicas");
      tft.write(16);
      tft.println("Afinar");
      tft.println(" Resetar");
    }
    if (posSettings == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(3);
      tft.println("Menu:");
      tft.setTextSize(2);
      tft.println("");
      tft.println(" Musicas");
      tft.println(" Afinar");
      tft.write(16);
      tft.println("Resetar");
    }
    
    // Lógica de atualização da variável de escolha
    if (buttonUpState == 0 && posSettings != 2)
    {
      posSettings--;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK); // apaga só a parte do display qeu será atualizada
      delay(delayButtons);
    }
    if (buttonDownState == 0 && posSettings != 4)
    {
      posSettings++;
      tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      delay(delayButtons);
    }

    // Sai do loop quando a escolha é realizada
    if (buttonSelectState == 0)
    {
      goBack = 1;
      delay(delayButtons);
    }
  }
  *targetScreen = posSettings; // passa o valor que indica a tela escolhida
}


/**
* @brief Executa a tela seleção de músicas
* @param targetScreen Número da tela que está sendo utilizada pelo usuário
*/
void menu(int *targetScreen)
{
  tft.fillScreen(ST7735_BLACK); // reseta a tela
  int goBackCursor = 0; // indica que "voltar" foi selecionado no menu
  int selectedMusic = 0; // Salva o número da música selecionada
  int goBack = 0; // controla o loop principal
  int menuPos = 1; // Posição que o 'cursor' está no menu
  int screenNumber = musicNumber / 10; // Contém o número de "telas" no menu de músicas
  int lastMusicScreenNumber = musicNumber % 10; // Só é usada essas 2 vezes
  int presentScreen = 0; // Número da tela atual do menu de musicas

  // lógica pra fazer o número de telas ser 1/10 do número de músicas contando com as opçoes de voltar
  if (lastMusicScreenNumber != 0)
  {
    screenNumber += 1;
  }
  int menuPosMax = musicNumber + screenNumber; // posição máxima no menú de músicas

  // Loop principal de execução do menu
  while (goBack == 0)
  {
    readingButtons();

    // A lógica de impressão de telas está separada em 3 casos
    // Caso da primeira tela
    if (presentScreen == 0)
    {
      // Se tem menos de 10 músicas
      if (musicNumber < 10)
      {
        tft.setCursor(0, 0); // Imprime o título do menu
        tft.setTextSize(2);
        tft.println("Musicas:");
        tft.setTextSize(1);
        tft.println("");

        // Loop que imprime as músicas (vai até o número máximo de músicas nesse caso)
        for (j = 1; j < musicNumber + 1; j++)
        {
          if (menuPos == j) // se é a posição do cursor
          {
            tft.write(16); // imprime com letra maior
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            selectedMusic = j;
          }
          else // se não é a posição do cursor
          {
            tft.print(" "); // imprime normal
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        
        // Se o cursor estiver na posição final do menu
        if (menuPos == menuPosMax)
        {
          tft.write(16); // imprime opção de voltar com letra grande
          tft.print("Voltar ");
          goBackCursor = 1;
        }
        else // se o cursor não estiver na posição final
        {
          tft.print(" Voltar "); // imprime opção de voltar com letra pequena
          goBackCursor = 0;
        }
      }
      
      // Se tem 10 ou mais músicas
      if (musicNumber > 9)
      {
        tft.setCursor(0, 0); // Imprime o título do menu
        tft.setTextSize(2);
        tft.println("Musicas:");
        tft.setTextSize(1);
        tft.println("");

        // Loop que imprime as músicas (vai até 10 nesse caso(cada tela tem 10 músicas))
        for (j = 1; j < 11; j++)
        {
          if (menuPos == j) // se é a posição do cursor
          {
            tft.write(16); // imprime com letra maior
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
            selectedMusic = j;
          }
          else // se não é a posição do cursor
          {
            tft.print(" "); // imprime normal
            tft.print(j);
            tft.print("-");
            tft.println(musicNames[j - 1].c_str());
          }
        }
        
        // Se o cursor estiver na décima posição da tela
        if (menuPos % 11 == 0)
        {
          tft.write(16); // imprime a opção de voltar com letra grande
          tft.print("Voltar ");
          goBackCursor = 1;
        }
        else // se o cursor não estiver na decima posição
        {
          tft.print(" Voltar "); // imprime o voltar normal
          goBackCursor = 0;
        }
      }
      delay(10);
    }
    
    // Caso de telas intermediárias
    if (presentScreen > 0 && presentScreen < screenNumber - 1)
    {
      tft.setCursor(0, 0); // Imprime o título do menu
      tft.setTextSize(2);
      tft.println("Musicas:");
      tft.setTextSize(1);
      tft.println("");
      
      // Loop que imprime as músicas (com cálculos pra percorrer os limites da tela atual)
      for (j = (10 * presentScreen) + 1; j < (10 * (presentScreen + 1)) + 1; j++)
      {
        // se é a posição do cursor (discontando as opções voltar)
        if ((menuPos - presentScreen) == j)
        {
          tft.write(16); // imprime com letra grande
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          selectedMusic = j;
        }
        else // se não é a posição do cursor
        {
          tft.print(" "); // imprime normal
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      
      // se o cursor estiver na posição final da tela
      if (menuPos % 11 == 0)
      {
        tft.write(16); // Imprime o voltar com letra grande
        tft.print("Voltar ");
        goBackCursor = 1;
      }
      else // se não
      {
        tft.print(" Voltar "); // imprime o voltar normal
        goBackCursor = 0;
      }
    }

    // Caso da última tela, quando tem mais de 1 tela
    if (presentScreen == screenNumber - 1 && screenNumber != 1)
    {
      tft.setCursor(0, 0); // Imprime o título do menu
      tft.setTextSize(2);
      tft.println("Musicas:");
      tft.setTextSize(1);
      tft.println("");

      // loop que imprime as músicas
      for (j = (10 * presentScreen) + 1; j < musicNumber + 1; j++)
      {
        // se é a posição do cursor (discontando os voltar)
        if ((menuPos - presentScreen) == j)
        {
          tft.write(16); // imprime com letra grande
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
          selectedMusic = j;
        }
        else // se não
        {
          tft.print(" "); // imprime normal
          tft.print(j);
          tft.print("-");
          tft.println(musicNames[j - 1].c_str());
        }
      }
      
      // se o cursor etiver na última posição
      if (menuPos == menuPosMax)
      {
        tft.write(16); // imprime o voltar grande
        tft.print("Voltar ");
        goBackCursor = 1;
      }
      else // se não
      {
        tft.print(" Voltar "); // imprime o voltar normal
        goBackCursor = 0;
      }
    }
    delay(10);

    // lógica de atualização das variáveis de controle
    // manda o cursor para cima
    if (buttonUpState == 0 && menuPos != 1)
    {
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      //tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      menuPos--;
      if (menuPos % 11 == 0)
      {
        presentScreen--;
        tft.fillScreen(ST7735_BLACK);
      }
      delay(delayButtons);
    }
    // manda o cursor para baixo
    if (buttonDownState == 0 && menuPos != menuPosMax)
    {
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      //tft.fillRect(0, 24, 10, 225, ST7735_BLACK);
      if (menuPos % 11 == 0)
      {
        presentScreen++;
        tft.fillScreen(ST7735_BLACK);
      }
      menuPos++;
      delay(delayButtons);
    }

    // botão pressionado
    if (buttonSelectState == 0)
    {
      if (goBackCursor == 0) // escolheu uma música
      {
        music(selectedMusic);
        delay(delayButtons);
      }
      else // escolheu voltar
      {
        *targetScreen = 1;
        goBack = 1;
        delay(delayButtons);
      }
    }
  }
}


/**
* @brief Executa a tela de afinação
* @param targetScreen Número da tela que está sendo utilizada pelo usuário
*/
void afinar(int *targetScreen)
{
  enableSwitch = 0;
  tft.fillScreen(ST7735_BLACK); // reseta o display
  int goBack = 0; // variável de controle do loop
  tunePos = 0; // variável de posição do cursor

  // loop que implementa o menu de afinação
  while (goBack == 0)
  {
    readingButtons();
    // Impressão do menu para cada caso de posição do cursor
    if (tunePos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.write(16);
      tft.println("Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.write(16);
      tft.println("Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.write(16);
      tft.println("Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.write(16);
      tft.println("Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.write(16);
      tft.println("Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 5)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.write(16);
      tft.println("Corda e");
      tft.println(" Voltar");
    }
    if (tunePos == 6)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.write(16);
      tft.println("Voltar");
    }
    delay(10);

    // lógica de atualização das variáveis de controle
    // Manda cursor para cima
    if (buttonUpState == 0 && tunePos != 0)
    {
      tunePos--;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    // Manda cursor para baixo
    if (buttonDownState == 0 && tunePos != 6)
    {
      tunePos++;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    // Botão pressionado
    if (buttonSelectState == 0)
    {
      if (tunePos != 6) // escolheu uma corda
      {
        xTaskCreatePinnedToCore(taskTune, "taskTune", 1000, NULL, 1, NULL, 0);
      }
      else // escolheu voltar
      {
        guitar.endMusic();
        tft.fillScreen(ST7735_BLACK);
        goBack = 1;
        *targetScreen = 1;
      }
      delay(delayButtons);
    }
    guitar.getEnginePos(sdCard);
  }
  enableSwitch = 1;
}


/**
* @brief Executa a tela de ajuste de motores
* @param targetScreen Número da tela que está sendo utilizada pelo usuário
*/
void resetEngines(int *targetScreen)
{
  enableSwitch = 0;
  tft.fillScreen(ST7735_BLACK); // Reseta o display
  int goBack = 0; // variável de controle do loop
  int resetPos = 0; // variável de posição do cursor (qual corda)

  // loop que implementa o menu de ajuste
  while (goBack == 0)
  {
    readingButtons();
    // Impressão do menu para cada caso de posição do cursor
    if (resetPos == 0)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.write(16);
      tft.println("Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 1)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.write(16);
      tft.println("Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 2)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.write(16);
      tft.println("Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 3)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.write(16);
      tft.println("Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 4)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.write(16);
      tft.println("Corda B");
      tft.println(" Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 5)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.write(16);
      tft.println("Corda e");
      tft.println(" Voltar");
    }
    if (resetPos == 6)
    {
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.println("Selecione:");
      tft.setTextSize(1);
      tft.println("");
      tft.println(" Corda E");
      tft.println(" Corda A");
      tft.println(" Corda D");
      tft.println(" Corda G");
      tft.println(" Corda B");
      tft.println(" Corda e");
      tft.write(16);
      tft.println("Voltar");
    }
    delay(10);

    // Lógica de atualização das variáveis de controle
    // Manda cursor para cima
    if (buttonUpState == 0 && resetPos != 0)
    {
      resetPos--;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    // Manda cursor para baixo
    if (buttonDownState == 0 && resetPos != 6)
    {
      resetPos++;
      tft.fillRect(0, 20, 5, 100, ST7735_BLACK);
      delay(delayButtons);
    }
    // Botão pressionado
    if (buttonSelectState == 0)
    {
      if (resetPos != 6) // escolheu uma corda
      {
        adjustEngine(&resetPos);
      }
      else // escolheu voltar
      {
        tft.fillScreen(ST7735_BLACK);
        goBack = 1;
        *targetScreen = 1;
      }
      delay(delayButtons);
    }
  }
  enableSwitch = 1;
}


// FUNÇÕES OPERACIONAIS DO MICROCONTROLADOR
void setup()
{
  // Serial.begin(115200);
  pinMode(Motors_Enable, OUTPUT);
  digitalWrite(Motors_Enable, enableSwitch);
  serialMutex = xSemaphoreCreateMutex();
  xTaskCreate(enableUpdateTask, "enable update", 4096, NULL, 2, NULL);
  //                    step, dir)
  guitar.insertMotor('E', 13, 12);
  guitar.insertMotor('A', 14, 27); 
  guitar.insertMotor('D', 33, 32);
  guitar.insertMotor('G', 26, 25);
  guitar.insertMotor('B', 22, 1);
  guitar.insertMotor('e', 3, 21);
  // guitar.insertMotor('E', 26, 25);
  //   guitar.insertMotor('A', 3, 21); 
  //   guitar.insertMotor('D', 14, 27);
  //   guitar.insertMotor('G', 22, 1);
  //   guitar.insertMotor('B', 13, 12);
  //   guitar.insertMotor('e', 33, 32);

  // pinMode(buttonUp, INPUT_PULLUP);
  // pinMode(buttonSelect, INPUT_PULLUP);
  // pinMode(JoystickIn, INPUT);


  musicNames = sdCard.getList();
  musicNumber = musicNames.size();

  String currentTargets = "";
  // currentTargets = sdCard.getCurrentTarget();
  // guitar.setCurrentTarget(currentTargets);

  /*guitar.runHalfTarget(0);
  guitar.runHalfTarget(1);
  guitar.runHalfTarget(2);
  guitar.runHalfTarget(3);
  guitar.runHalfTarget(4);
  guitar.runHalfTarget(5);*/
  //Serial.println("Inicializado display...");

  // Teste para a inicialização do TFT
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(-1);

  // Inicializar a fonte com tamanho padrão
  tft.setTextSize(2);              // Tamanho do texto
  tft.setTextColor(ST7735_WHITE); // Cor do texto

  // Limpa o display
  tft.fillScreen(ST7735_BLACK);

  readingButtons();
}



void loop()
{
  switch (targetScreen)
  {
  case 1:
  {
    settings(&targetScreen);
    break;
  }
  case 2:
  {
    menu(&targetScreen);
    break;
  }
  case 3:
  {
    afinar(&targetScreen);
    break;
  }
  case 4:
  {
    resetEngines(&targetScreen);
    break;
  }
  }

}



void enableUpdateTask(void *arg){
  while(1){
    bool tmp = enableSwitch;
    vTaskDelay(15/portTICK_PERIOD_MS);
    if(enableSwitch!=tmp){
      digitalWrite(Motors_Enable,enableSwitch);
    }
  }
}
