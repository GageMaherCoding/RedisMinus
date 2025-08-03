#pragma once
#include <string>
#include "database.h"

class CommandParser {
public:
    static std::string parseAndExecute(const std::string& input, Database& db);
};
