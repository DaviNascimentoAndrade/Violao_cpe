#ifndef SCREEN_MANAGER_HPP
#define SCREEN_MANAGER_HPP

#include "Screen.hpp"

class ScreenManager {
public:
  void setScreen(Screen* newScreen);
  void update();

private:
  Screen* current = nullptr;
};

#endif
