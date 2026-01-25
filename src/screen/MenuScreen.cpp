#include "MenuScreen.hpp"

MenuScreen::MenuScreen(Adafruit_ST7735& tft)
  : tft(tft) {}

void MenuScreen::onEnter() {
  tft.fillScreen(ST7735_BLACK);
  draw();
}

void MenuScreen::update() {
  readingButtons();

  if (buttonUpState == 0 && pos > 0) {
    pos--;
    draw();
    delay(200);
  }

  if (buttonDownState == 0 && pos < 2) {
    pos++;
    draw();
    delay(200);
  }

  if (buttonSelectState == 0) {
    // screenManager.setScreen(&outraTela);
  }
}

void MenuScreen::draw() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0,0);
  tft.setTextSize(2);

  tft.println(pos == 0 ? "> Musicas" : "  Musicas");
  tft.println(pos == 1 ? "> Afinar"  : "  Afinar");
  tft.println(pos == 2 ? "> Resetar" : "  Resetar");
}
