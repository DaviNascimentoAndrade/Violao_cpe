#include "MusicScreen.hpp"

MusicScreen::MusicScreen(
  Adafruit_ST7735& tft,
  EnginesSet& guitar,
  SDCard& sdCard,
  Joystick& joy
) : tft(tft), guitar(guitar), sdCard(sdCard), joy(joy) {}

void MusicScreen::onEnter() {
  tft.fillScreen(ST7735_BLACK);

  musicNames = sdCard.getList();
  musicNumber = musicNames.size();

  state = State::LIST;
  menuPos = 0;

  drawList();
}

void MusicScreen::update() {
  switch (state) {
    case State::LIST:
      updateList();
      break;

    case State::MENU:
      updateMenu();
      break;

    case State::STROKES:
      updateStrokes();
      break;
  }
}

void MusicScreen::updateList() {
  readingButtons();

  if (buttonUpState == 0 && menuPos > 0) {
    menuPos--;
    drawList();
    delay(200);
  }

  if (buttonDownState == 0 && menuPos < musicNumber) {
    menuPos++;
    drawList();
    delay(200);
  }

  if (buttonSelectState == 0) {
    if (menuPos == musicNumber) {
      // Voltar
      // screenManager.setScreen(&menuScreen);
    } else {
      selectedMusic = menuPos;
      state = State::MENU;
      drawMenu();
    }
    delay(200);
  }
}

void MusicScreen::drawList() {
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0,0);
  tft.println("Musicas:");

  for (int i = 0; i < musicNumber; i++) {
    tft.print(menuPos == i ? "> " : "  ");
    tft.println(musicNames[i].c_str());
  }

  tft.println(menuPos == musicNumber ? "> Voltar" : "  Voltar");
}

void MusicScreen::updateMenu() {
  readingButtons();

  if (buttonUpState == 0 && menuPos > 0) {
    menuPos--;
    drawMenu();
    delay(200);
  }

  if (buttonDownState == 0 && menuPos < 1) {
    menuPos++;
    drawMenu();
    delay(200);
  }

  if (buttonSelectState == 0) {
    if (menuPos == 0) {
      // Tocar
      firstStroke  = sdCard.readFile(selectedMusic + 1, '1');
      secondStroke = sdCard.readFile(selectedMusic + 1, '2');
      thirdStroke  = sdCard.readFile(selectedMusic + 1, '3');

      guitar.setEngineSpeed(sdCard);
      guitar.setSubdivision(sdCard);

      state = State::STROKES;
      drawStrokes();
    } else {
      state = State::LIST;
      drawList();
    }
    delay(200);
  }
}

void MusicScreen::updateStrokes() {
  readingButtons();

  if (buttonSelectState == 0) {
    stopSongTask();
    state = State::MENU;
    drawMenu();
    delay(200);
  }
} 

void MusicScreen::drawStrokes() {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(20,0);
  tft.setTextSize(2);
  tft.println("TOCANDO");

  startSongTask();
}

