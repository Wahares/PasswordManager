//
// Created by Wahares on 18.05.2023.
//

#ifndef PASSWORDMANAGER_PASSWORDUI_H
#define PASSWORDMANAGER_PASSWORDUI_H
#include "UiSingleView.h"

class passwordUI :  UiSingleView{
public:passwordUI();
    ViewTypes getType() override;
private:
    void onStart() override;
    std::vector<KeyboardOption> getOptions(std::vector<KeyboardOption> &vec) override;
};


#endif //PASSWORDMANAGER_PASSWORDUI_H
