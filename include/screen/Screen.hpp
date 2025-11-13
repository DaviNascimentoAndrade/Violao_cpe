#pragma once

class Screen {

protected:
    bool exitRequested = false;
    int nextScreenId = 1;   

public:
    virtual ~Screen() {}

    virtual void update() = 0;  
    virtual void draw() = 0;    

    bool shouldExit() const { return exitRequested; }
    int nextScreen() const { return nextScreenId; }

};
