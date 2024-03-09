//
// Created by Wahares on 18.05.2023.
//

#include "MainController.h"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <ctime>
#include <utility>
#include "UiStateMachine.h"
#include "MyCryptography.h"
#include "ViewTypes.h"
#include "StringExpansion.h"

std::string MainController::fileDirectory;
std::string MainController::readedData;
std::string MainController::password;

/**
 * main method to setup UiStateMachine and start program
 */
void MainController::begin() {
    UiStateMachine::createViews();
    UiStateMachine::fireTransition(ViewTypes::FilePick);
}


/**
 * checks file compatibility, picks it for later actions and transit to PasswordUI
 * @param dir - directory of source file
 */
void MainController::pickFile(std::string dir) {

    fileDirectory = std::move(dir);

    std::ifstream stream(fileDirectory);
    if(!stream.is_open()){
        std::cout<<"Couldn't open file"<<std::endl;
        UiStateMachine::fireTransition(ViewTypes::FilePick);
        return;
    }
    std::stringstream buffer;
    buffer << stream.rdbuf();
    readedData = buffer.str();
    stream.close();


    if(!MyCryptography::validateInput(readedData)){
        std::cout<<"Corrupted or incorrect file architecture"<<std::endl;
        UiStateMachine::fireTransition(ViewTypes::FilePick);
        return;
    }
    UiStateMachine::fireTransition(ViewTypes::FilePassword);
}
/**
 * checks if given password is correct
 * @param pass - password
 * @return bool if correct
 */
bool MainController::passwordTry(const std::string& pass) {
    if(fileDirectory.empty())
        return false;
    if(readedData.empty())
        return false;
    if(pass.empty())
        return false;
    if(!MyCryptography::validateDecodedInput(MyCryptography::decode(pass,
                                                                    MyCryptography::base16decode(
                                                                            MyCryptography::base16decode(readedData)))))
        return false;
    password = pass;
    return true;
}

/**
 * method to touch file - read file, decode with base16, edit in-file TimeStamp, code, and save
 */
void MainController::touchFile() {
    if(fileDirectory.empty())
        return;
    if(readedData.empty())
        return;
    auto stream = std::fstream(fileDirectory);
    std::string decodedReaded = MyCryptography::base16decode(readedData);
    std::string tmp = StringExpansion::splitString(decodedReaded,"[TIM]")[0];
    tmp +="[TIM]";
    std::time_t timestamp = std::time(nullptr);
    tmp += std::ctime(&timestamp);
    tmp +="[ETI]";
    tmp += StringExpansion::splitString(decodedReaded,"[ETI]")[1];
    if(stream.is_open())
        stream<<MyCryptography::base16encode(tmp);
}
/**
 * loads records from directory to given vector
 * @param vec - vector to be loaded to
 */
void MainController::loadFromFile(std::vector<PassAccount>& vec) {
    if(fileDirectory.empty())
        return;
    if(readedData.empty())
        return;
    if(password.empty())
        return;
    string tmp = MyCryptography::base16decode(readedData);
    tmp = StringExpansion::splitString(tmp,"\n[TIM]")[0];
    tmp = MyCryptography::base16decode(tmp);
    tmp = MyCryptography::decode(password, tmp);
    tmp = StringExpansion::splitString(tmp,"[success123!@#]\n")[1];
    MyCryptography::unPackData(tmp,vec);
}
/**
 * code, validate, pack, and save record
 * @param accounts - vector of records to be saved
 */
void MainController::saveToFile(std::vector<PassAccount> &accounts) {
    if(fileDirectory.empty())
        return;
    //pack accounts with password validation at beginning
    string data = MyCryptography::packData(accounts,password);
    //make timestamp
    data = MyCryptography::base16encode(data);
    data +="\n[TIM]";
    std::time_t timestamp = std::time(nullptr);
    data += std::ctime(&timestamp);
    data +="[ETI]";
    //add validation
    data +="\n[success123!@#]";
    data = MyCryptography::base16encode(data);

    auto ostream = std::ofstream(fileDirectory);
    //save data
    ostream<<data;
    ostream.close();

    //set readedData to new data to be used later on
    std::ifstream stream(fileDirectory);
    std::stringstream buffer;
    buffer << stream.rdbuf();
    readedData = buffer.str();
    stream.close();
}

void MainController::addNewPassword(const PassAccount &p, std::vector<PassAccount> &accounts) {
accounts.push_back(p);
    saveToFile(accounts);
    saveToFile(accounts);
}

void MainController::removePassword(const PassAccount &p, std::vector<PassAccount> &accounts) {
    std::erase_if(accounts,[&p](const PassAccount& pa)->bool{ return pa.password==p.password;});
    saveToFile(accounts);
}

void MainController::addNewCategory(const string &catName, std::vector<PassAccount> &accounts) {
    addNewPassword(PassAccount("","",catName,"",""),accounts);
    saveToFile(accounts);
}

void MainController::removeCategory(const string &catName, std::vector<PassAccount> &accounts) {
    std::erase_if(accounts,[&catName](const PassAccount& pa)->bool{ return pa.category==catName;});
    saveToFile(accounts);
}



