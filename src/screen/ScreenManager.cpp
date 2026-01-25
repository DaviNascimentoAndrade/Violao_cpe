#include "ScreenManager.hpp"

void ScreenManager::setScreen(Screen* newScreen) {
  if (current) current->onExit();
  current = newScreen;
  if (current) current->onEnter();
}

void ScreenManager::update() {
  if (current) current->update();
}
