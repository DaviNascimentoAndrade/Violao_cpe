//Nesse arquivo é definida a classe que modela o dispositivo joystick

#ifndef JOYSTICK_
#define JOYSTICK_
#define DEAD_ZONE 25 // O valor mínimo de inclinação do joystick até que ele retorne o primeiro valor

//Essa classe modela um dispositivo real do tipo joystick. Ela lê as entradas analógicas passadas pelo dispositivo e faz a conversões necessárias para repassar a inclinação dele e o pressionar do botão.

class Joystick
{
    private:
        int pin_X, pin_Y, button; //pinos do joystick (funciona como dois potenciômetros e um botão)
        int X, Y; //Inclinação do joystick
        int pinNumber;

    public:

        /**
        * @brief Construtor do joystick
        * @param cpin_X Pino das coordenadas x
        * @param cpin_Y Pino das coordenadas y
        * @param cbutton Pino do botão
        */
        Joystick(int cpin_X, int cpin_Y, int cbutton);
        Joystick(int cpin_X, int cbutton);

        /*Os métodos abaixo retornam as inclinações dos joysticks, em porcentagem valores de -100 a 100,
        onde -100 o Joystick está totalmente inclinado pra esquerda (quando na direção X) ou para cima (quando na direção Y)
        e 100 indica que o Joystick está totalmente inclinado para a direita (quando na direção X) ou para baixo, (quando
        na direção Y). Zero indica que o Joystick está centralizado, a menos da deadzone */
        
        /**
        * @brief Retorna o acionamento do botão
        */
        bool get_button(); //retorna o acionamento do botão sob o joystick

        /**
        * @brief Retorna a inclinação no eixo X
        */
        int get_X();

        /**
        * @brief Retorna a inclinação no eixo Y
        */
        int get_Y();

        /**
        * @brief Retorna 1 se a inclinação Y for positiva e 0 se negativa
        */
        bool Y_asButtonUp();

        /**
        * @brief Retorna 0 se a inclinação Y for positiva e 1 se negativa
        */
        bool Y_asButtonDown();

        /**
        * @brief Retorna 0 se a inclinação X for positiva e 1 se negativa
        */
        bool X_asButtonDown();

        /**
        * @brief Retorna 1 se a inclinação X for positiva e 0 se negativa
        */
        bool X_asButtonUp();

};


#endif