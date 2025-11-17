#ifndef ENGINESSET
#define ENGINESSET

/**
 * @file EngineSet.hpp
 * @brief Armazena todas as Engines e controla elas pra tocar música.
 * @version 1.0
 * @date 2023-07-28
 * @details This archive contains the class 'EngineSet', used to create the magic guitar.
 It store all the Engines and control them using the SD card data to play a music.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include "Engine.hpp"
#include "SDCard.hpp"
#include <vector>
#include <string>
#include <Arduino.h>

#define DELAY_SPEED 1800
#define STEP_NUMBER 13

class EnginesSet
{
public:

    /**
    * @brief Enumerador de direções da engine, UP = 1, DOWN = 0.
    */
    enum mDirection
    {
        UP = 0,
        DOWN = 1
    };

    /**
    * @brief Construtor do EnginesSet.
    */
    EnginesSet();

    /**
     * @brief Destrutor do EnginesSet
     * Apenas deleta as engines alocadas anteriormente.
     */
    ~EnginesSet();

    /**
    * @brief Adiciona uma Engine ao EnginesSet.
    * @param pGuitarString Qual corda a Engine toca.
    * @param pStep STEP pin of the Engine.
    * @param pDir DIR pin of the Engine.
    */
    void insertMotor(char pGuitarString, int pStep, int pDir);

    // void boot(); // Lógica de fim de curso

    /**
    * @brief Adiciona um Engine a mEnginesToPlay de acordo com a música.
    * @param pGuitarString Qual corda a Engine toca.
    */
    void addToenginesToPlay(std::string pGuitarString); 

    /**
    * @brief Roda os Engines em mEnginesToPlay.
    */
    void playMany();

    /**
    * @brief Toca uma música
    * @param pStream Sequência de notas
    * @param pTune Afinação da música
    */
    void parseFile(std::string pStream, int pTune);

    /**
    * @brief Faz todos os Engines subirem ou descerem
    * @param pDirection Engine current direction.
    * @param pGuitarStrings Guitar strings that will be played.
    */
    void runThrough(mDirection pDirection, std::string pGuitarStrings);

    void tune(int pTunePosition);

    /**
    * @brief Reseta um Engine fazendo ele mover um passo.
    * @param pGuitarReset Posição do Engine em mEngines.
    * @param pSignal Direction to reset the Engine (up or down).
    */
    void playOneStep(int pGuitarReset, int pSignal);

    /**
    * @brief Salva a posição do Engine no cartão SD
    * @param pSD SD Card.
    */
    void getEnginePos(SDCard pSd);

    /**
    * @brief Configura oposição após cada música.
    */
    void endMusic();

    /**
    * @brief Configura a velocidade dos engines.
    * @param pSD SD Card.
    */
    void setEngineSpeed(SDCard pSd);

    /**
    * @brief Configura o número de subdivisões de um compasso.
    * @param pSD SD Card.
    */
    void setSubdivision(SDCard pSd);

    /**
    * @brief Configura o delay entre pulsos
    * @param pSD SD Card.
    */
    void setDelay();

    /**
    * @brief Configura a variável mLastMillis para a última leitura de millis()
    */
    void setLastMillis();
  
    /**
     * @brief Toca cada corda com delay de .5 segundos 2 vezes
     * Usado antes de cada música para o usuário mover a mão e para alinhar as paletas
     */
    void standardBeat();

    void setCurrentTarget(String pcurrentTargets);

    /**
    * @brief Gira um Engine para metade da posição alvo.
    * @param pGuitarStringReset Engine position in mEngines.
    */
    void runHalfTarget(int pGuitarStringReset);

    /**
    * @brief Vetor de posições de Engines
    */
    std::vector<int> mEnginePos;

private:
    /**
    * @brief Vector of Engines.
    */
    std::vector<Engine *> mEngines;

    /**
    * @brief Vector of Engines que vão tocar a música
    */
    std::vector<Engine *> mEnginesToPlay;

    // int mExitLoop;
    // int mButtonUpState;
    // int mButtonSelectState;
    // int mButtonDownState;

    /**
    * @brief Engines BPM speed.
    */
    float mBPMSpeed; 

    /**
    * @brief Music subdivision.
    */
    string mSubdivision;

    /**
    * @brief Global Engine Targets.
    */
    int mGlobalTargets;

    /**
    * @brief Delay in miliseconds.
    */
    uint32_t mDelayMillis;

    /**
    * @brief Delay in microseconds.
    */
    uint32_t mDelayMicro;

    /**
    * @brief millis() reading on last stroke.
    */
    unsigned long mLastMillis;
};

#endif
