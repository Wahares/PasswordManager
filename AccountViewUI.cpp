//
// Created by Wahares on 22.05.2023.
//

#include "AccountViewUI.h"
#include "ViewTypes.h"
#include "UiStateMachine.h"
#include "MainController.h"
#include "MyCryptography.h"
#include "PassAccount.h"
#include "StringExpansion.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <sstream>

std::vector<PassAccount> *accounts;
std::vector<PassAccount> *currentAccountsView;

//ids of DataTypes for currently displayed records to be sorted on
int sortMethod1type = -1;
int sortMethod2type = -1;

#define backOnStartOption  KeyboardOption("b", "Back",[this](const std::string &answer) -> void { onStart(); })
#define cancelOperation  KeyboardOption("c", "Cancel",[](const std::string &answer) -> void {  })

//defines if currently displayed records where filtered by search action
bool searched = false;

AccountViewUI::AccountViewUI() {
    UiStateMachine::setupMe(this);
}

ViewTypes AccountViewUI::getType() {
    return ViewTypes::AccountView;
}

void AccountViewUI::onStart() {
    accounts = new std::vector<PassAccount>();
    MainController::loadFromFile(*accounts);
    resetViewAccounts();
    displayMain();
}
/**
 * displays commands and records without restarting view
 */
void AccountViewUI::displayMain() {

    std::vector<KeyboardOption> vec;
    sortCurrent();
    std::cout << "------------Passwords------------\n";
    printCurrentPasswords();
    std::cout << "------------Commands------------\n";

    getOptions(vec);
    UiStateMachine::CreateOptionUI(vec);
}

std::vector<KeyboardOption> AccountViewUI::getOptions(std::vector<KeyboardOption> &vec) {
    vec.push_back(KeyboardOption("l", "Logout",
                                 [this](const std::string &answer) -> void {
                                     logout();
                                 }));
    vec.push_back(KeyboardOption("ap", "Add Password",
                                 [this](const std::string &answer) -> void {
                                     addPasswordOption();
                                 }));
    vec.push_back(KeyboardOption("mp", "Modify Password",
                                 [this](const std::string &answer) -> void {
                                     modifyPassword();
                                 }));
    vec.push_back(KeyboardOption("dp", "Delete Password",
                                 [this](const std::string &answer) -> void {
                                     deleteDisplayed();
                                 }));
    vec.push_back(KeyboardOption("ac", "Add Category",
                                 [this](const std::string &answer) -> void {
                                     addCategory();
                                 }));
    vec.push_back(KeyboardOption("dc", "Delete Category",
                                 [this](const std::string &answer) -> void {
                                     deleteCategory();
                                 }));
    vec.push_back(KeyboardOption("se", "Search",
                                 [this](const std::string &answer) -> void {
                                     search();
                                 }));

    if (sortMethod1type == -1)
        vec.push_back(KeyboardOption("sb", "Sort By",
                                     [this](const std::string &answer) -> void {
                                         addSortBy();
                                     }));
    else {
        if (sortMethod2type == -1)
            vec.push_back(KeyboardOption("sb", "Add Second Sort By Method",
                                         [this](const std::string &answer) -> void {
                                             addSortBy();
                                         }));
        vec.push_back(KeyboardOption("rsb", "Reset Sort By",
                                     [this](const std::string &answer) -> void {
                                         sortMethod1type = -1;
                                         sortMethod2type = -1;
                                         displayMain();
                                     }));
    }
    if (searched)
        vec.push_back(KeyboardOption("rse", "Reset Search",
                                     [this](const std::string &answer) -> void {
                                         onStart();
                                     }));
    return vec;
}
/**
 * set variables to be ready on re-enter and transit to main UI
 */
void AccountViewUI::logout() {
    MainController::password = "";
    MainController::fileDirectory = "";
    delete accounts;
    UiStateMachine::fireTransition(FilePick);
}

void AccountViewUI::addPasswordOption() {
    UiStateMachine::CreateOptionUI({
                                           backOnStartOption,
                                           KeyboardOption("1", "Generate password",
                                                          [this](const std::string &answer) -> void {
                                                              addGenerated(0);
                                                          }),
                                           KeyboardOption("2", "Enter password manually",
                                                          [this](const std::string &answer) -> void { addPasswordManual(); })
                                   });
}
/**
 * UI for user to modify picked DataType field in picked record
 */
void AccountViewUI::modifyPassword() {

    auto options = std::vector<KeyboardOption>();

    options.push_back(cancelOperation);
    std::cout << "Pick password to modify:\n";

    PassAccount *tmp = pickDisplayedPassword();
    if (tmp == nullptr) {
        onStart();
        return;
    }
    std::cout << "What do You want to modify?\n";
    int dataType = pickDataType();
    if (dataType != -1)
        modifySingleData(dataType, *tmp);

    MainController::saveToFile(*accounts);
    onStart();
}

/**
 * simple UI for user to pick one record from currently displayed
 * @return picked record
 */
PassAccount* AccountViewUI::pickDisplayedPassword() {
    PassAccount *tmp = nullptr;
    auto options = std::vector<KeyboardOption>();
    options.push_back(cancelOperation);

    for (int i = 0; i < currentAccountsView->size(); ++i) {
        auto os = std::stringstream();
        os << (*currentAccountsView)[i];
        string pass = os.str();
        options.push_back(
                KeyboardOption(std::to_string(i), pass,
                               [i, &tmp](const string &answer) -> void { tmp = &(*currentAccountsView)[i]; }));
    }
    UiStateMachine::CreateOptionUI(options);
    return tmp;
}

 /**
  * continuation of modify - different option branches because of different DataTypes field requirements
  * @param type - ID of DataType field
  * @param p - record to be modified
  */
void AccountViewUI::modifySingleData(int type, PassAccount &p) {
    KeyboardOption backOption = KeyboardOption("b", "Back", [](const string &answer) -> void {});

    if (type == 2) //want to modify category
    {
        std::vector<string> categories = getCategories();
        auto options = std::vector<KeyboardOption>();
        options.push_back(backOption);
        std::cout << "Pick category:\n";
        options.push_back(KeyboardOption("0", "None",
                                         [this, &p](const string &answer) -> void {
                                             setSingularData(p, 2, "");
                                         }));
        std::erase_if(categories, [](const string &s) -> bool { return s.empty(); });
        for (int i = 0; i < categories.size(); ++i)
            options.push_back(
                    KeyboardOption(std::to_string(i + 1), categories[i],
                                   [this, &categories, &p, i](const string &answer) -> void {
                                       setSingularData(p, 2, categories[i]);
                                   }));
        UiStateMachine::CreateOptionUI(options);

    }
    else if (type < 2) //want to modify name or password
    {
        std::cout << "Enter new data:\n";

        UiStateMachine::CreateOpenOptionUI(backOption,
                                           [type](const string &answer) -> bool {
                                               if (type ==
                                                   1)//want to modify password - check if new password already exists
                                                   if (passAlreadyExist(answer)) {
                                                       std::cout << "Password already exist\n";
                                                       return false;
                                                   }
                                               bool tmp = answer.length() > 2;
                                               if (!tmp)
                                                   std::cout << "Too short\n";
                                               return tmp;
                                           }, [this, &type, &p](const string &answer) -> void {
                    setSingularData(p, type, answer);
                });
    }
    else //want to modify optional data
    {
        std::cout << "This position is optional - write \"-\" if You want to make it blank\n";
        std::cout << "Enter new data:\n";
        UiStateMachine::CreateOpenOptionUI(backOption,
                                           [](const string &answer) -> bool { return true; },
                                           [this, &type, &p](const string &answer) -> void {
                                               setSingularData(p, type, answer);
                                           });
    }
}




/**
 * iterated UI to pick length and additional parameters of generated password
 * @param iteration - (set 0 here)internally used int to make method recursive
 */

void AccountViewUI::addGenerated(int iteration) {
    if (iteration == 0) //start - ask how many characters
    {
        std::cout << "How many characters? (min 6) :\n";
        UiStateMachine::CreateOpenOptionUI(backOnStartOption, [](const string &answer) -> bool {
            if (StringExpansion::parsableToInt(answer))return StringExpansion::parseToInt(answer) >= 6;
            return false;
        }, [this](const string &answer) -> void { addGenerated(StringExpansion::parseToInt(answer) << 3); });
        return;
    }
    else if ((iteration & 2) == 0) //middle iteration - ask if generated password have to have big and small letters
    {

        std::cout << "Do you wish your password to have big and small letters?\n";
        UiStateMachine::CreateOptionUI({backOnStartOption, KeyboardOption("y", "Yes", [this, &iteration](
                const std::string &ans) -> void { addGenerated(iteration | 3); }), KeyboardOption("n", "No",
                                                                                                  [this, &iteration](
                                                                                                          const std::string &ans) -> void {
                                                                                                      addGenerated(
                                                                                                              iteration |
                                                                                                              2);
                                                                                                  })});
        return;
    }
    //last iteration - ask if generated password have to have special characters, generate strong password and apply

    std::cout << "Do you wish your password to have special characters?\n";
    UiStateMachine::CreateOptionUI({backOnStartOption, KeyboardOption("y", "Yes", [this, &iteration](
            const std::string &ans) -> void { addGeneratedContinue(iteration | 4); }), KeyboardOption("n", "No",
                                                                                                      [this, &iteration](
                                                                                                              const std::string &ans) -> void {
                                                                                                          addGeneratedContinue(
                                                                                                                  iteration);
                                                                                                      })});


}


/**
 * continuation of adding generated password - generate strong password and go to name etc. entering
 * @param data - coded parameters for generator
 */
void AccountViewUI::addGeneratedContinue(int data) {
    int howManyLetters = data >> 3;
    int hasLetters = (data & 1) != 0;
    int hasSpecial = (data & 4) != 0;
    string password;
    do {
        password = MyCryptography::generatePassword(howManyLetters, hasLetters, hasSpecial);
    } while (MyCryptography::calcPasswordStrength(password) < 2);
    addPasswordProcedure(password);
}

/**
 * UI for user to manually enter password - checking strength and go to name etc. entering
 */
void AccountViewUI::addPasswordManual() {
    std::cout << "Enter Password:\n";
    UiStateMachine::CreateOpenOptionUI(backOnStartOption, [](const string &answer) -> bool {
        bool exist = passAlreadyExist(answer);
        if (exist) std::cout << "Password already in use\n";
        return !exist;
    }, [this](const string &answer) -> void {
        if (MyCryptography::calcPasswordStrength(answer) < 3) {

            std::cout << "This password is weak. Consider changing it.\n" << "Do you wish to change it?\n";
            UiStateMachine::CreateOptionUI(
                    {backOnStartOption, KeyboardOption("y", "yes", [this](const string &answer) -> void {
                        addPasswordManual();
                        return;
                    }), KeyboardOption("n", "no", [this](const string &answer) -> void {
                        addPasswordProcedure(answer);
                        return;
                    })});

        } else
            addPasswordProcedure(answer);
    });
}

/**
 * final procedure of record adding - picking name, category etc.
 * @param pass - given password
 */
void AccountViewUI::addPasswordProcedure(const string &pass) {

    string name;
    string login;
    string www;
    string category;

    std::cout << "Enter account name:\n";
    std::cin >> name;
    while (name.length() < 2) {
        std::cout << "Too short:\n";
        std::cin >> name;
    }

    std::cout << "Enter account login (optional - write \"-\" for blank):\n";
    std::cin >> login;
    if (login == "-")
        login = "";

    std::cout << "Enter account domain (optional - write \"-\" for blank):\n";
    std::cin >> www;
    if (www == "-")
        www = "";

    std::cout << "Select category:\n";
    std::vector<KeyboardOption> categories;
    std::vector<string> tmp = getCategories();
    categories.push_back(KeyboardOption("0", "None", [&category](const string &answer) -> void { category = ""; }));
    std::erase_if(tmp, [](const string& s)->bool{ return s.empty();});
    for (int i = 0; i < tmp.size(); ++i)
        categories.push_back(KeyboardOption(std::to_string(i + 1), tmp[i], [&category, tmp, i](
                const string &answer) -> void { category = tmp[i]; }));
    UiStateMachine::CreateOptionUI(categories);

    finalAddPassword(name, pass, category, login, www);
}

/**
 * finalize of record adding - pack data int struct, pack into main vector of records and save
 * @param name - name
 * @param pass - password
 * @param cat - category
 * @param log - login
 * @param dom - domain
 */
void AccountViewUI::finalAddPassword(const string &name, const string &pass, const string &cat, const string &log,
                                     const string &dom) {
    MainController::addNewPassword(PassAccount(name, pass, cat, log, dom), *accounts);
    onStart();
}

/**
 * makes UI for user to enter name of new category
 */
void AccountViewUI::addCategory() {
    std::cout<<"Enter new category name:\n";
    UiStateMachine::CreateOpenOptionUI(
            cancelOperation,
            [](const string &answer) -> bool {
                bool valid = std::count_if(accounts->begin(), accounts->end(),
                                           [&answer](const PassAccount &p) -> bool { return p.category == answer; }) ==
                             0;
                if (!valid)
                    std::cout << "Category already exists\n";
                return valid;
            }, [](const string &answer) -> void { MainController::addNewCategory(answer, *accounts); }
    );
    onStart();
}

/**
 * makes UI for user to pick category to delete and ascertain if user wants to delete all records in given category
 */
void AccountViewUI::deleteCategory() {

    std::vector<string> categories = getCategories();
    auto options = std::vector<KeyboardOption>();
    options.push_back(cancelOperation);
    std::cout << "Pick category to delete:\n";

    for (int i = 0; i < categories.size(); ++i)
        options.push_back(KeyboardOption(std::to_string(i), categories[i], [&categories](const string &answer) -> void {
            //how many passwords uses this category?
            int count = (int) std::count_if(currentAccountsView->begin(), currentAccountsView->end(),
                                            [&answer, &categories]
                                                    (const PassAccount &p) -> bool {
                                                return p.category == categories[StringExpansion::parseToInt(answer)] &&
                                                       !p.password.empty();
                                            });

            std::cout << "Are you sure to delete this category with " << count << " passwords with it?\n";
            UiStateMachine::CreateYNUI(nullptr, [&categories, &answer]() -> void {
                MainController::removeCategory(categories[StringExpansion::parseToInt(answer)], *accounts);
            }, []() -> void {});
        }));


    UiStateMachine::CreateOptionUI(options);
    onStart();
}
/**
 * adds sorting method id (if there is first then it adds as second sub-sortMethod)
 */
void AccountViewUI::addSortBy() {
    std::cout << "By which value do you wish to sort passwords?\n";
    int *point;
    if (sortMethod1type == -1)
        point = &sortMethod1type;
    else
        point = &sortMethod2type;
    *point = pickDataType();
    displayMain();
}

/**
 * simple sort by two or one method set previously
 */
void AccountViewUI::sortCurrent() {
    if (sortMethod2type == -1) {
        if (sortMethod1type != -1)
            std::sort(currentAccountsView->begin(), currentAccountsView->end(),
                      [this](const PassAccount &l, const PassAccount &r) -> bool {
                          return getSingularData(l, sortMethod1type).compare(getSingularData(r, sortMethod1type)) < 0;
                      });
    } else {
        std::sort(currentAccountsView->begin(), currentAccountsView->end(),
                  [this](const PassAccount &l, const PassAccount &r) -> bool {
                      int tmp = getSingularData(l, sortMethod1type).compare(getSingularData(r, sortMethod1type));
                      if (tmp != 0)
                          return tmp < 0;
                      return getSingularData(l, sortMethod2type).compare(getSingularData(r, sortMethod2type)) < 0;
                  });
    }

}
/**
 *
 * @param p - PassAccount
 * @param type -  id of field DataType
 * @param data - data to be read from PassAccount at DataType field
 */
std::string AccountViewUI::getSingularData(const PassAccount &p, int type) {
    switch (type) {
        case 0://name
            return p.name;
        case 1://pass
            return p.password;
        case 2://cat
            return p.category;
        case 3://log
            return p.login;
        case 4://dom
            return p.www;
        default:
            return "";
    }
}
/**
 *
 * @param p - PassAccount
 * @param type -  id of field DataType
 * @param data - data to be written on PassAccount at DataType field
 */
void AccountViewUI::setSingularData(PassAccount &p, int type, const string &data) {
    PassAccount *actualPass = &*std::find_if(accounts->begin(), accounts->end(),
                                             [&p](const PassAccount &pa) -> bool { return pa.password == p.password; });
    switch (type) {
        case 0://name
            actualPass->name = data;
            break;
        case 1://pass
            actualPass->password = data;
            break;
        case 2://cat
            actualPass->category = data;
            break;
        case 3://log
            if (data == "-") //optional data handle
                actualPass->login = "";
            else
                actualPass->login = data;
            break;
        case 4://dom
            if (data == "-") //optional data handle
                actualPass->www = "";
            else
                actualPass->www = data;
            break;
        default:
            break;
    }
    MainController::saveToFile(*accounts);
}
/**
 * simple UI used to allow user to search records that include some phrase in picked DataType field
 */
void AccountViewUI::search() {
    std::cout << "By which data would You like to search?\n";
    int dataType = pickDataType();
    if (dataType == -1) {
        onStart();
        return;
    }
    std::cout << "Enter phrase You want the found passwords to have: \n";
    searched = true;
    UiStateMachine::CreateOpenOptionUI(backOnStartOption, [](const string &answer) -> bool { return true; },
                                       [&dataType, this](const string &answer) -> void {
                                           delete currentAccountsView;
                                           currentAccountsView = new std::vector<PassAccount>(*accounts);
                                           std::erase_if(*currentAccountsView, [&dataType, this, &answer](
                                                   const PassAccount &p) -> bool {
                                               return getSingularData(p, dataType).find(answer) == std::string::npos;
                                           });
                                       });
    if(searched)
    displayMain();
}
/**
 * small UI with question which data does user selects
 * @return ID of dataType of PassAccount
 */
int AccountViewUI::pickDataType() {
    int tmp = -1;
    std::vector<KeyboardOption> options;
    options.push_back(cancelOperation);
    options.push_back(KeyboardOption("0", "Name", [&tmp](const string &answer) -> void {
        tmp = 0;
    }));
    options.push_back(KeyboardOption("1", "Password", [&tmp](const string &answer) -> void {
        tmp = 1;
    }));
    options.push_back(KeyboardOption("2", "Category", [&tmp](const string &answer) -> void {
        tmp = 2;
    }));
    options.push_back(KeyboardOption("3", "Login", [&tmp](const string &answer) -> void {
        tmp = 3;
    }));
    options.push_back(KeyboardOption("4", "Domain", [&tmp](const string &answer) -> void {
        tmp = 4;
    }));
    UiStateMachine::CreateOptionUI(options);
    return tmp;
}
/**
 * simple UI with question if user wants to delete all displayed passwords or pick one of them
 */
void AccountViewUI::deleteDisplayed() {
    std::cout << "Would you like to pick one password to be deleted or delete all of currently displayed records?\n";
    std::vector<KeyboardOption> options;
    options.push_back(cancelOperation);

    options.push_back(KeyboardOption("s", "Pick single password", [this](const string &answer) -> void {
        PassAccount *tmp = pickDisplayedPassword();
        if (tmp != nullptr) {
            std::cout << "Do you really want to delete this record?\n";
            UiStateMachine::CreateYNUI(nullptr, [&tmp]() -> void {
                MainController::removePassword(*tmp, *accounts);
            }, []() -> void {});
        }

    }));

    options.push_back(KeyboardOption("a", "Delete all currently displayed", [](const string &answer) -> void {
        std::cout << "Do you really want to delete " << currentAccountsView->size() << " records?\n";
        UiStateMachine::CreateYNUI(nullptr, []() -> void {
            for (const PassAccount &p: *currentAccountsView)
                MainController::removePassword(p, *accounts);
        }, []() -> void {});
    }));

    UiStateMachine::CreateOptionUI(options);
    onStart();
}

/**
 * copy data from 'accounts' vector and deletes blank category records
 */
void AccountViewUI::resetViewAccounts() {
    delete currentAccountsView;
    currentAccountsView = new std::vector<PassAccount>(*accounts);
    std::erase_if(*currentAccountsView, [](const PassAccount &p) -> bool { return p.password.empty(); });
    searched = false;
}

void AccountViewUI::printCurrentPasswords() {
    for (const PassAccount &p: *currentAccountsView)
        std::cout << p << std::endl;
}

std::vector<string> AccountViewUI::getCategories() {
    std::unordered_set<std::string> categories;
    for (const PassAccount &p: *accounts)
        categories.insert(p.category);
    auto v = std::vector<string>(categories.begin(), categories.end());
    return v;
}

bool AccountViewUI::passAlreadyExist(const string &pass) {
    return std::count_if(accounts->begin(), accounts->end(),
                         [&pass](const PassAccount &p) -> bool { return p.password == pass; }) != 0;
}
