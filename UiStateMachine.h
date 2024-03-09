//
// Created by Wahares on 18.05.2023.
//

#ifndef PASSWORDMANAGER_UIMACHINESTATE_H
#define PASSWORDMANAGER_UIMACHINESTATE_H

#include <map>
#include "UiStateMachine.h"
#include "UiSingleView.h"
#include "ViewTypes.h"
#include "KeyboardOption.h"
#include <vector>

class UiSingleView;

class UiStateMachine {
public:
    static std::map<ViewTypes, UiSingleView *> views;

    static void createViews();

    static void setupMe(UiSingleView *view);

    static void fireTransition(ViewTypes type);

    static void CreateOptionUI(std::vector<KeyboardOption> options);
    static void CreateYNUI(std::function<void()> cancel,std::function<void()> yes,std::function<void()> no);

    static void CreateOpenOptionUI(const KeyboardOption& back
    ,const std::function<bool(std::string)>& answerValidation
    ,const std::function<void(std::string)>& atAnswerValidated);
};


#endif //PASSWORDMANAGER_UIMACHINESTATE_H
