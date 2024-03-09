//
// Created by Wahares on 18.05.2023.
//

#include "startUI.h"
#include "UiStateMachine.h"
#include "ViewTypes.h"
#include "KeyboardOption.h"
#include "MyCryptography.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include "MainController.h"

class KeyboardOption;

void startUI::onStart() {
    //make sure there is PasswordFiles directory
    if (!std::filesystem::exists("..\\PasswordFiles") || !std::filesystem::is_directory("..\\PasswordFiles"))
        std::filesystem::create_directory("..\\PasswordFiles");

    std::cout << "Welcome to password manager!\n";
    std::vector<KeyboardOption> vec;
    getOptions(vec);
    UiStateMachine::CreateOptionUI(vec);
}

ViewTypes startUI::getType() {
    return FilePick;
}

startUI::startUI() {
    UiStateMachine::setupMe(this);
}

std::vector<KeyboardOption> startUI::getOptions(std::vector<KeyboardOption> &vec) {
    vec.push_back(KeyboardOption("q", "Quit", [](const std::string &answer) -> void {
        std::cout << "Goodbye!";
        std::exit(0);
    }));
    vec.push_back(KeyboardOption("1", "Choose file", [this](const std::string &answer) -> void { pickFileUI(); }));
    vec.push_back(KeyboardOption("2", "Choose file from path",
                                 [this](const std::string &answer) -> void { pickFromDirectoryUI(); }));
    vec.push_back(KeyboardOption("3", "Create new file",
                                 [this](const std::string &answer) -> void { createFileUI(); }));
    return vec;
}

/**
 * small UI used to pick source file from default directory
 */
void startUI::pickFileUI() {
    std::vector<KeyboardOption> fileOptions;
    auto iter = std::filesystem::directory_iterator("..\\PasswordFiles");

    fileOptions.push_back(KeyboardOption("b", "Back", [](const std::string &answer) -> void {
        UiStateMachine::fireTransition(ViewTypes::FilePick);
    }));

    for (const auto &file: iter) {
        string path = file.path().string();
        if (file.is_regular_file())
            fileOptions.push_back(KeyboardOption(std::to_string(fileOptions.size()), file.path().string(),
                                                 [path, this](const std::string &answer) -> void { goToPass(path); }));
    }

    if (fileOptions.size() == 1) {
        std::cout << "couldn't find any files" << std::endl;
        UiStateMachine::fireTransition(ViewTypes::FilePick);
        return;
    }

    std::cout << "found " << fileOptions.size() - 1 << " files" << std::endl;
    std::cout << "Choose file: " << std::endl;
    UiStateMachine::CreateOptionUI(fileOptions);
}



void startUI::pickFromDirectoryUI() {

    std::vector<KeyboardOption> fileOptions;
    std::cout << "Enter absolute path: " << std::endl;
    fileOptions.push_back(KeyboardOption("b", "Back", [](const std::string &answer) -> void {
        UiStateMachine::fireTransition(ViewTypes::FilePick);
    }));
    fileOptions.push_back(KeyboardOption("", "", [this](const std::string &answer) -> void {
        if (std::filesystem::exists(answer) && std::filesystem::is_regular_file(answer)) {
            goToPass(answer);
        } else {
            std::cout << "unable to open file\n";
            UiStateMachine::fireTransition(ViewTypes::FilePick);
        }
    }));
    UiStateMachine::CreateOpenOptionUI(fileOptions[0], [](const std::string &) -> bool { return true; },
                                       fileOptions[1].action);
}


/**
 * small UI used to get name of new file from user
 */
void startUI::createFileUI() {
    std::cout << "Enter file name: " << std::endl;

    UiStateMachine::CreateOpenOptionUI(KeyboardOption("c", "Cancel", [](const std::string &answer) -> void {
                                           UiStateMachine::fireTransition(ViewTypes::FilePick);
                                       }),
                                       [](const std::string &answer) -> bool {
                                           bool tmp = !std::filesystem::exists("..\\PasswordFiles\\" + answer);
                                           if(!tmp)
                                               std::cout<<"File with this name already exists!\n";
                                           return tmp;
                                       }, [this](const std::string &answer) -> void { goToPassNewFile(answer); });
}
/**
 * continuation of createFileUI - gets password to new file created in default directory
 * @param fileName - name of new file
 */
void startUI::goToPassNewFile(const std::string &fileName) {

    std::cout << "Enter Password: " << std::endl;
    UiStateMachine::CreateOpenOptionUI(KeyboardOption("c", "cancel", [](const std::string &answer) -> void {
        UiStateMachine::fireTransition(ViewTypes::FilePick);
    }), [](const std::string &answer) -> bool {
        bool valid = MyCryptography::calcPasswordStrength(answer) > 1;
        if (!valid)
            std::cout << "Password too weak" << std::endl;
        return valid;
    }, [fileName](const std::string &answer) -> void {
        auto stream = std::ofstream("..\\PasswordFiles\\" + fileName);
        if (stream.is_open()) {
            string validationtext = MyCryptography::packData(
                    {}, answer);
            validationtext = MyCryptography::base16encode(validationtext);
            validationtext += "\n[TIM]";
            std::time_t timestamp = std::time(nullptr);
            validationtext += std::ctime(&timestamp);
            validationtext += "[ETI]";
            validationtext += "\n[success123!@#]";
            validationtext = MyCryptography::base16encode(validationtext);
            std::cout << "Created: " + fileName + '\n';
            stream << validationtext;
            stream.close();
            UiStateMachine::fireTransition(ViewTypes::FilePick);
        } else {
            std::cout << "unable to create file\n";
            UiStateMachine::fireTransition(ViewTypes::FilePick);
        }
    });
}
/**
 * go to password UI
 * @param directory - directory of file to open
 */
void startUI::goToPass(const std::string &directory) {
    MainController::pickFile(directory);
}

