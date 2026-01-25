#ifndef MUSIC_SCREEN_HPP
#define MUSIC_SCREEN_HPP

#include "Screen.hpp"
#include "EnginesSet.hpp"
#include "SDCard.hpp"
#include "Joystick.hpp"
#include <Adafruit_ST7735.h>
#include <vector>
#include <string>

class MusicScreen : public Screen {
public:
  MusicScreen(
    Adafruit_ST7735& tft,
    EnginesSet& guitar,
    SDCard& sdCard,
    Joystick& joy
  );

  void onEnter() override;
  void onExit() override;
  void update() override;

private:
  enum class State {
    LIST,
    MENU,
    STROKES
  };

  //Dependencias
  Adafruit_ST7735& tft;
  EnginesSet& guitar;
  SDCard& sdCard;
  Joystick& joy;

  //Estado interno
  State state = State::LIST;

  //Dados
  std::vector<std::string> musicNames;
  int musicNumber = 0;
  int selectedMusic = 0;
  int menuPos = 0;
  int playingPos = 0;

  std::string firstStroke;
  std::string secondStroke;
  std::string thirdStroke;
  int nStrokes = 0;

  //Metodos internos
  void updateList();
  void updateMenu();
  void updateStrokes();

  void drawList();
  void drawMenu();
  void drawStrokes();

  void startSongTask();
  void stopSongTask();
};

#endif
