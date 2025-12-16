#ifndef SDCARD
#define SDCARD

/**
 * @file SDCard.hpp
 * @brief Control the SD Card.
 * @version 1.0
 * @date 2023-07-28
 * @details This archive contains the class 'SDCard', used to control the SD Card that stores the musics and data.
 * @authors CPE - Consultoria e Projetos Elétricos
 */

#include <vector>
#include <SD.h>
#include <string>
#include <regex>
using namespace std;

class SDCard
{
public:

    /**
    * @brief Construtor do SDCard.
    */
    SDCard();

    /**
    * @brief Retorna o vetor mFilesNames com os nomes das músicas.
    */
    vector<string> getList();

    /**
    * @brief Cria o vetor mFilesNames com os nomes das músicas.
    */
    void Makelist();

    /**
    * @brief Prints the position of an archive - used for tests.
    * @param pFileName File name.
    */
    void printPosition(std::string pFileName);

    /**
    * @brief Lê o arquivo de texto para obter os dados da música a ser tocada
    * @param pNumFile Posição numérica da música no cartão sd
    * @param pNumStroke Número da sequência de notas
    */
    string readFile(int pNumFile, char pNumStroke);

    /**
    * @brief Saves the Engines' position on the SD card.
    * @param pEnginePos Vector of Engines' position.
    */
    void writeInFile(vector<int> pEnginePos);

    /**
    * @brief Retorna os alvos atuais
    */
    String getCurrentTarget();

    /**
    * @brief Velocidade da música
    */
    int SD_SPEED;

    /**
    * @brief Subdivisão da música
    */
    string SD_SUBDIVISION;

    /**
    * @brief Nome do autor
    */
    string authorName;

private:
    /**
    * @brief Vetor com os nomes das músicas
    */
    vector<string> mFilesNames;
};

#endif