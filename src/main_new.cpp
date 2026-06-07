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

#include "Screen/Playback.hpp"
#include "Screen/ScreenManager.hpp"

using namespace std;

// --- Pinos e constantes de hardware ---------------------------------------
#define Motors_Enable 17

#define TFT_MOSI 23 // também representado por SDA
#define TFT_SCLK 18
#define TFT_DC 2 // também representado por A0
#define TFT_CS 15
#define TFT_RST 4

#define buttonSelect 16
#define JoystickIn 35

// --- Objetos globais de hardware/estado ------------------------------------
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Joystick Joy(JoystickIn, -1, buttonSelect);
EnginesSet guitar;
SDCard sdCard;

SemaphoreHandle_t serialMutex;
void enableUpdateTask(void *arg);

// Estado da reprodução, compartilhado com as tasks.
string defStroke = "";          // sequência atual em reprodução
int lastStroke = 0;             // 0 = não está tocando
bool breakLoopFlag = false;     // sinaliza parada do songLoop
int tunePos = 0;                // corda a afinar (lido pela taskTune)
TaskHandle_t loopHandle = NULL; // handle do songLoop, para exclusão externa

// ---------------------------------------------------------------------------
// Tasks de reprodução (permanecem no main nesta etapa).
// ---------------------------------------------------------------------------
void songLoop(void *parameter)
{
    guitar.standardBeat();
    guitar.setLastMillis();

    while (1)
    {
        guitar.parseFile(defStroke, 0);
        if (breakLoopFlag)
            break;
    }

    lastStroke = 0;
    vTaskDelete(NULL);
}

void taskTune(void *parameter)
{
    guitar.tune(tunePos);
    vTaskDelete(NULL);
}

// ---------------------------------------------------------------------------
// Implementação concreta da interface Playback.
// Fina de propósito: apenas conecta as telas às tasks acima. Numa próxima
// etapa isto vira uma classe PlaybackController dedicada.
// ---------------------------------------------------------------------------
class MainPlayback : public Playback
{
public:
    void startLoop(const std::string &p_stroke) override
    {
        if (lastStroke == 0)
        {
            defStroke = p_stroke;
            lastStroke = 1;
            breakLoopFlag = false;
            xTaskCreatePinnedToCore(songLoop, "songLoop", 1000, NULL, 1,
                                    &loopHandle, 0);
        }
    }

    void stopLoop() override
    {
        if (loopHandle != NULL)
        {
            vTaskDelete(loopHandle);
            loopHandle = NULL;
            lastStroke = 0;
        }
        breakLoopFlag = true;
    }

    void tuneAsync(int p_tune_pos) override
    {
        tunePos = p_tune_pos;
        xTaskCreatePinnedToCore(taskTune, "taskTune", 1000, NULL, 1, NULL, 0);
    }

    bool isPlaying() override
    {
        return lastStroke != 0;
    }
};

MainPlayback playback;
ScreenManager screenManager(tft, Joy, guitar, sdCard, playback);

// ---------------------------------------------------------------------------
// Funções operacionais do microcontrolador
// ---------------------------------------------------------------------------
void setup()
{
    pinMode(Motors_Enable, OUTPUT);
    digitalWrite(Motors_Enable, enableSwitch);
    serialMutex = xSemaphoreCreateMutex();
    xTaskCreate(enableUpdateTask, "enable update", 4096, NULL, 2, NULL);

    //                  string, step, dir
    guitar.insertMotor('E', 13, 12);
    guitar.insertMotor('A', 14, 27);
    guitar.insertMotor('D', 33, 32);
    guitar.insertMotor('G', 26, 25);
    guitar.insertMotor('B', 22, 1);
    guitar.insertMotor('e', 3, 21);

    // Inicialização do display TFT
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(-1);
    tft.setTextSize(2);
    tft.setTextColor(ST7735_WHITE);
    tft.fillScreen(ST7735_BLACK);
}

void loop()
{
    screenManager.update();
}

void enableUpdateTask(void *arg)
{
    while (1)
    {
        bool tmp = enableSwitch;
        vTaskDelay(15 / portTICK_PERIOD_MS);
        if (enableSwitch != tmp)
        {
            digitalWrite(Motors_Enable, enableSwitch);
        }
    }
}