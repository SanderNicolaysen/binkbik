#ifndef BINKBIK_SAVELOADER_H
#define BINKBIK_SAVELOADER_H


#include "GameSave.h"

class GameSaveHandler {
public:
    void saveToFile(const std::string& file, GameSave &save);

    GameSave loadFromFile(const std::string& file);

    bool isError() const;

private:
    bool error = false;
};


#endif //BINKBIK_SAVELOADER_H
