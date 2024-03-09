//
// Created by Wahares on 22.05.2023.
//

#ifndef PASSWORDMANAGER_STRINGEXPANSION_H
#define PASSWORDMANAGER_STRINGEXPANSION_H

#include <iostream>
#include <vector>

class StringExpansion {

public:
    static std::string dataBetween(const std::string& input, const std::string& start, const std::string& end);
    static std::vector<std::string> splitString(const std::string& input, const std::string& separator);
    static int NumOfDistinctChars(const std::string& text);
    static int parseToInt(const std::string& text);
    static bool parsableToInt(const std::string& text);
};


#endif //PASSWORDMANAGER_STRINGEXPANSION_H
