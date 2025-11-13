#pragma once
#include "Screen.hpp"

class ScreenManager {

private:
    Screen* current = nullptr;

public:
    ScreenManager();
    void changeTo(int id);
    void loop();
    
};
