#ifndef ENGINE
#define ENGINE

extern volatile bool enableSwitch;

/**
 * @file Engine.hpp
 * @brief Control and store a step-enginer.
 * @version 1.0
 * @date 2023-07-28
 * @details This archive contains the class 'Engine', used to control individually and store a step-enginer of the Magic Guitar.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "SDCard.hpp"

class Engine
{
public:
    
    /**
    * @brief Construtor de uma Engine.
    * @param pGuitarString Corda que a Engine toca.
    * @param pStep Pino STEP da Engine.
    * @param pDir Pino DIR da Engine.
    */
    Engine(char pGuitarString, int pStep, int pDir);

    /**
    * @brief Retorna a corda que o Engine toca.
    */
    char getGuitarString();

    /**
    * @brief Retorna o alvo do Engine
    */
    int getTarget();

    /**
    * @brief Retorna o pino DIR do Engine.
    */
    int getDir();

    /**
    * @brief Retorna o pino STEP do engine.
    */
    int getStep();

    /**
    * @brief Inverte o alvo do Engine.
    */
    void reverseTarget();

    /**
    * @brief Retorna a posição do Engine.
    */
    int getPosition();

    /**
    * @brief Retorna o ponteiro de posição
    */
    int *getPosPtr();

    /**
    * @brief Gira o Engine um passo - usado para resetar
    */
    void oneStep(int pSignal);

    /**
    * @brief Set the Engine speed.
    * @param pSDSpeed New Engine speed.
    */
    void setSpeed(int pSDSpeed); // utilizado para determinar o valor da velocidade

    /**
    * @brief Configura a velocidade de delay do Engine
    * @param pSDSpeed New Engine delay speed.
    */
    void setDelaySpeed(int pSDSpeed);

    /**
    * @brief Retorna a velocidade de delay do Engine
    */
    int getDelaySpeed();

    /**
    * @brief Next Engine position.
    */
    int mTarget;        // próxima posição

    /**
    * @brief Engine current position.
    */
    int mPosition;      // posição atual

private:
    /**
    * @brief Engine guitar string.
    */ 
    char mGuitarString; // corda

    /**
    * @brief Pointer to the engine position.
    */ 
    int *mPosPtr; // ponteiro para gravar a posição

    /**
    * @brief Speed of the engine in RPM.
    */ 
    int mSpeedRPM;

    /**
    * @brief Delay speed of the engine.
    */ 
    int mDelaySpeed;

    /**
    * @brief DIR pin of the engine driver.
    */ 
    int mDIR; // pino do drive

    /**
    * @brief STEP pin of the engine driver.
    */ 
    int mSTEP; // pino do drive

    /**
    * @brief Steps needed for a full revolution.
    */ 
    int mRevolution; // quantidade de passos necessários para dar uma volta completa
};

#endif