//
// Created by Wahares on 18.05.2023.
//

#include "UiStateMachine.h"
#include "UiSingleView.h"
#include "startUI.h"
#include "passwordUI.h"
#include "AccountViewUI.h"

std::map<ViewTypes, UiSingleView *> UiStateMachine::views;

/**
 * method to execute by UIview to setup on main UiStateMachine to be used later in transitions
 * @param view class deriving UiSingleView
 */
void UiStateMachine::setupMe(UiSingleView *view) {
    views.insert({view->getType(), view});
}
/**
 * create objects of views
 */
void UiStateMachine::createViews() {
    new startUI();
    new passwordUI();
    new AccountViewUI();
}

void UiStateMachine::fireTransition(ViewTypes type) {
    views.at(type)->onStart();
}

/**
 * creates small portion of UI with limited options
 * @param options vector of keyboardOptions (first one have to be 'back' option)
 */
void UiStateMachine::CreateOptionUI(std::vector<KeyboardOption> options) {
    std::string answer;
    while (answer != options[0].key) {

        for (auto &i: options)
            std::cout << i;

        std::cin >> answer;
        auto iterator = std::find_if(options.begin(), options.end(),
                                     [answer](const KeyboardOption &option) { return option.key == answer; });
        if (iterator != options.end()) {
            iterator->action(answer);
            return;
        } else {
            std::cout << "Couldn't resolve an option" << std::endl;
        }
    }
}

/**
 * creates small portion of UI with open question
 * @param back - KeyboardOption that cancels procedure
 * @param answerValidation - predicate that validates answer
 * @param atAnswerValidated - action executed with answer after successful validation
 */
void
UiStateMachine::CreateOpenOptionUI(const KeyboardOption &back, const std::function<bool(std::string)> &answerValidation,
                                   const std::function<void(std::string)> &atAnswerValidated) {
    std::string answer;
    while (answer != back.key) {
        std::cout << back;
        std::cin >> answer;
        if (answer != back.key) {
            if (answerValidation(answer)) {
                atAnswerValidated(answer);
                return;
            }
        }
        else
            back.action(answer);
    }
}

/**
 * simplified version of CreateOptionUI with y/n options
 * @param cancel - option canceling question (can be nullptr)
 * @param yes - positive answer action
 * @param no - negative answer action
 */
void UiStateMachine::CreateYNUI(std::function<void()> cancel,std::function<void()> yes, std::function<void()> no) {
    std::vector<KeyboardOption> vec;
    if(cancel != nullptr)
        vec.push_back(KeyboardOption("c","Cancel",[&cancel](const string& answer)->void{cancel();}));
    vec.push_back(KeyboardOption("y","Yes",[&yes](const string& answer)->void{yes();}));
    vec.push_back(KeyboardOption("n","No",[&no](const string& answer)->void{no();}));
    CreateOptionUI(vec);
}



