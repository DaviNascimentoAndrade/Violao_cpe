#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include "Screen.hpp"
#include <Adafruit_ST7735.h>

class MenuScreen : public Screen {
public:
  MenuScreen(Adafruit_ST7735& tft);

  void onEnter() override;
  void update() override;

private:
  Adafruit_ST7735& tft;
  int pos = 0;

  void draw();
};

#endif
