//
// Created by Wahares on 18.05.2023.
//

#ifndef PASSWORDMANAGER_UISINGLEVIEW_H
#define PASSWORDMANAGER_UISINGLEVIEW_H
#include <functional>
#include <vector>
#include <string>
#include "ViewTypes.h"
#include "KeyboardOption.h"

class KeyboardOption;
class UiSingleView {
public:
    //unique ViewTypes type used in UiStateMachine transitions
    virtual ViewTypes getType()=0;
    //executed at start of UiView
    virtual void onStart()=0;
    //options given at start of this UiView
    virtual std::vector<KeyboardOption> getOptions(std::vector<KeyboardOption> &vec)=0;
};


#endif //PASSWORDMANAGER_UISINGLEVIEW_H
