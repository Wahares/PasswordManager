//
// Created by Wahares on 18.05.2023.
//

#ifndef PASSWORDMANAGER_STARTUI_H
#define PASSWORDMANAGER_STARTUI_H
#include "UiSingleView.h"
#include "UiStateMachine.h"
#include "ViewTypes.h"
#include <vector>

class startUI :  UiSingleView{
public:startUI();
     ViewTypes getType() override;
private:
     void onStart() override;
      std::vector<KeyboardOption> getOptions(std::vector<KeyboardOption> &vec) override;
      void pickFileUI();
      void createFileUI();
      void pickFromDirectoryUI();
      void goToPass(const std::string& directory);
     void goToPassNewFile(const std::string &directory);
};


#endif //PASSWORDMANAGER_STARTUI_H
