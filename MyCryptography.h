//
// Created by Wahares on 18.05.2023.
//

#ifndef PASSWORDMANAGER_MYCRYPTOGRAPHY_H
#define PASSWORDMANAGER_MYCRYPTOGRAPHY_H
#include <iostream>
#include <vector>
#include "PassAccount.h"

class MyCryptography {
public: static std::string encode(const std::string& k, const std::string& t);
    static std::string decode(const std::string& k, const std::string& t);
    static std::string base16decode(const std::string& text);
    static std::string base16encode(const std::string& text);
    static bool validateInput(const std::string& text);
    static bool validateDecodedInput(const std::string& text);
    static std::string packData(const std::vector<PassAccount>& accounts, const std::string& key);
    static void unPackData(const string& data, std::vector<PassAccount>& vec);
    static string generatePassword(int charCount,bool hasLetters,bool hasSpecial);
    static int calcPasswordStrength(const string& password);
};


#endif //PASSWORDMANAGER_MYCRYPTOGRAPHY_H
