#include "Joystick.hpp"
#include <Arduino.h>

Joystick::Joystick(int cpin_X, int cpin_Y, int cbutton): 
    pin_X(cpin_X), pin_Y(cpin_Y), button(cbutton){
        pinMode(pin_X,INPUT);
        pinMode(pin_Y,INPUT);
        pinMode(button,INPUT_PULLUP);
}

Joystick::Joystick(int cpin_X, int cbutton): 
    pin_X(cpin_X), button(cbutton){
        pinMode(pin_X,INPUT);
        pinMode(button,INPUT_PULLUP);
        pinNumber = 2;
}

int Joystick::get_X(){
    this->X = (abs(((analogRead(pin_X))/20.475)-100)<DEAD_ZONE)? 0 : (((analogRead(pin_X))/20.475)-100);
    return this->X;
}

int Joystick::get_Y(){
    switch(pinNumber){
        case 3:
            this->Y = (abs(((analogRead(pin_Y))/20.475)-100)<DEAD_ZONE)? 0 : (((analogRead(pin_Y))/20.475)-100);
            return this->Y;
            break;            
        case 2:
            return 0;
            break;
        /*
        - falta de caso default pode bugar o switch e ficar sem retorno.
        */
    }
}

bool Joystick::get_button(){
    return !digitalRead(button);
}

/*
- antiga logica das funcoes booleanas Y/X:

if (n < 0) return true;
else if (n > 0) return false;

- (clean code) podem ser escritas diretamente como return (n > 0).  

*/

bool Joystick::Y_asButtonUp(){
    return (get_Y() > 0);
}

bool Joystick::Y_asButtonDown(){
    return (get_Y() < 0);
}

bool Joystick::X_asButtonDown(){
    return (get_Y() < 0);
}

bool Joystick::X_asButtonUp(){
    return (get_X() < 0);
}
