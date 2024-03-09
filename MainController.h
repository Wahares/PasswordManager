//
// Created by Wahares on 18.05.2023.
//

#ifndef PASSWORDMANAGER_MAINCONTROLLER_H
#define PASSWORDMANAGER_MAINCONTROLLER_H
#include <iostream>
#include <vector>
#include "PassAccount.h"

class MainController {
public:
    static string fileDirectory;
    static string password;
    static string readedData;
    static void begin();
    static void pickFile(std::string dir);
    static bool passwordTry(const std::string& pass);
    static void touchFile();
    static void loadFromFile(std::vector<PassAccount>& vec);
    static void saveToFile(std::vector<PassAccount> &accounts);
    static void addNewPassword(const PassAccount& p,std::vector<PassAccount>& accounts);
    static void removePassword(const PassAccount& p,std::vector<PassAccount>& accounts);
    static void addNewCategory(const string& catName,std::vector<PassAccount>& accounts);
    static void removeCategory(const string& catName,std::vector<PassAccount>& accounts);

};


#endif //PASSWORDMANAGER_MAINCONTROLLER_H
