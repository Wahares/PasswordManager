//
// Created by Wahares on 18.05.2023.
//

#include "passwordUI.h"
#include "UiStateMachine.h"
#include "MainController.h"
#include "MyCryptography.h"
#include "StringExpansion.h"

passwordUI::passwordUI() {
    UiStateMachine::setupMe(this);
}

ViewTypes passwordUI::getType() {
    return FilePassword;
}

void passwordUI::onStart() {
    std::cout << "Password: \n";
    std::vector<KeyboardOption> vec;
    getOptions(vec);
    //small UI checking correctness of given password (if incorrect - update timestamp)
    UiStateMachine::CreateOpenOptionUI(vec[0],
                                       [](const std::string &answer) -> bool {
                                           bool success = MainController::passwordTry(answer);
                                           if (success)
                                               std::cout << "Correct Password!\n";
                                           else {
                                               std::cout << "Incorrect Password!\n";
                                               MainController::touchFile();
                                           }
                                           return success;

                                       }, [](const std::string &answer) -> void {
                std::cout << "Last touch of file: "
                          << StringExpansion::dataBetween(MyCryptography::base16decode(MainController::readedData), "[TIM]", "\n[ETI]")
                          << std::endl;
                MainController::touchFile();
                UiStateMachine::fireTransition(ViewTypes::AccountView);
            });
}

std::vector<KeyboardOption> passwordUI::getOptions(std::vector<KeyboardOption> &vec) {
    vec.push_back(KeyboardOption("b", "Back", [](const std::string &answer) -> void {
        UiStateMachine::fireTransition(ViewTypes::FilePick);
    }));
    return vec;
}

