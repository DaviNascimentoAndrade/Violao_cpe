#ifndef SCREEN_HPP
#define SCREEN_HPP

class Screen {
public:
  virtual ~Screen() {}

  virtual void onEnter() {}  
  virtual void onExit() {}    
  virtual void update() = 0;  
};

#endif
