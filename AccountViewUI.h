//
// Created by Wahares on 22.05.2023.
//

#ifndef PASSWORDMANAGER_ACCOUNTVIEW_H
#define PASSWORDMANAGER_ACCOUNTVIEW_H

#include "UiSingleView.h"
#include "PassAccount.h"

class AccountViewUI : UiSingleView {

public:AccountViewUI();
    ViewTypes getType() override;
private:
    void onStart() override;
    std::vector<KeyboardOption> getOptions(std::vector<KeyboardOption> &vec) override;
    void logout();
    void displayMain();
    void addPasswordOption();
    void modifyPassword();
    void modifySingleData(int type, PassAccount& p);
    void search();
    void addSortBy();
    void sortCurrent();
    void addCategory();
    void deleteCategory();
    void addGenerated(int iteration);
    void addGeneratedContinue(int data);
    void addPasswordManual();
    void addPasswordProcedure(const string& pass);
    static bool passAlreadyExist(const string& pass);
    void resetViewAccounts();
    void printCurrentPasswords();
    int pickDataType();
    PassAccount* pickDisplayedPassword();
    void finalAddPassword(const string& name, const string& pass,const string& category,const string& login,const string& domain);
    std::vector<string> getCategories();
    std::string getSingularData(const PassAccount& p,int type);
    void setSingularData(PassAccount& p,int type,const string& data);
    void deleteDisplayed();
};


#endif //PASSWORDMANAGER_ACCOUNTVIEW_H
