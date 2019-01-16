#include <fstream>
#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include "GameSaveHandler.h"

GameSave GameSaveHandler::loadFromFile(const std::string& file)
{
    error = false;
    std::ifstream fs(file);

    if (fs.bad() || !fs.is_open())
    {
        error = true;
        return GameSave();
    }

    cereal::JSONInputArchive archive(fs);

    fs.close();

    GameSave save;

    archive( save );

    return save;
}

void GameSaveHandler::saveToFile(const std::string& file, GameSave &save)
{
    error = false;

    std::ofstream fs(file);

    {
        cereal::JSONOutputArchive archive(fs);
        archive(save);
    }

    fs.close();
}

bool GameSaveHandler::isError() const {
    return error;
}
