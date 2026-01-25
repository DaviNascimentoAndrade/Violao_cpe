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

//Mecanica, Core
#include "Engine.hpp"
#include "EnginesSet.hpp"
#include "Joystick.hpp"
#include "SDCard.hpp"

//Telas, UI
#include "ScreenManager.hpp"
#include "MenuScreen.hpp"
#include "MusicScreen.hpp"
#include "TuneScreen.hpp"
#include "ResetScreen.hpp"

//Definicoes de hardware
#define MOTORS_ENABLE 17

//MOSI (23) e SCLK (18) nao eram usados
#define TFT_DC   2
#define TFT_CS   15
#define TFT_RST  4

#define JOYSTICK_X 35
#define BUTTON_SELECT 16

volatile bool enableSwitch = 1;

SemaphoreHandle_t serialMutex;

//Display
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);

//Input
Joystick joy(JOYSTICK_X, -1, BUTTON_SELECT);

//Logica do core
EnginesSet guitar;
SDCard sdCard;

//Sistematizacao das telas
ScreenManager screenManager;

//Telas
MenuScreen  menuScreen(tft);
MusicScreen musicScreen(tft, guitar, sdCard, joy);
TuneScreen  tuneScreen(tft, guitar);
ResetScreen resetScreen(tft, guitar);

//Tasks
void enableUpdateTask(void* arg) {
  while (1) {
    digitalWrite(MOTORS_ENABLE, enableSwitch);
    vTaskDelay(15 / portTICK_PERIOD_MS);
  }
}

//Setup
void setup() {
  pinMode(MOTORS_ENABLE, OUTPUT);
  digitalWrite(MOTORS_ENABLE, enableSwitch);

  serialMutex = xSemaphoreCreateMutex();

  xTaskCreate(
    enableUpdateTask,
    "enableUpdate",
    2048,
    NULL,
    2,
    NULL
  );

  //motores
  guitar.insertMotor('E', 13, 12);
  guitar.insertMotor('A', 14, 27);
  guitar.insertMotor('D', 33, 32);
  guitar.insertMotor('G', 26, 25);
  guitar.insertMotor('B', 22, 1);
  guitar.insertMotor('e', 3, 21);

  //Display
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(-1);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.fillScreen(ST7735_BLACK);

  //Tela inicial
  screenManager.setScreen(&menuScreen);
}

//Loop
void loop() {
  //telas continuam lendo input da forma atual
  screenManager.update();
}