#include "command_parser.h"
#include <sstream>
#include <vector>
#include <algorithm>

static std::vector<std::string> split(const std::string& str) {
    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

std::string CommandParser::parseAndExecute(const std::string& input, Database& db) {
    auto tokens = split(input);
    if (tokens.empty()) return "ERR empty command\n";

    std::string cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);

    if (cmd == "SET") {
        if (tokens.size() != 3) return "ERR wrong number of arguments for SET\n";
        db.set(tokens[1], tokens[2]);
        return "OK\n";
    }
    else if (cmd == "GET") {
        if (tokens.size() != 2) return "ERR wrong number of arguments for GET\n";
        return db.get(tokens[1]) + "\n";
    }
    else if (cmd == "DEL") {
        if (tokens.size() != 2) return "ERR wrong number of arguments for DEL\n";
        return db.del(tokens[1]) ? "OK\n" : "(nil)\n";
    }

    return "ERR unknown command\n";
}
