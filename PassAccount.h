//
// Created by Wahares on 22.05.2023.
//

#ifndef PASSWORDMANAGER_PASSACCOUNT_H
#define PASSWORDMANAGER_PASSACCOUNT_H
#include<iostream>
using std::string;
struct PassAccount {
    friend std::ostream& operator<<(std::ostream &os, const PassAccount& pass) {
        {
            os<<"["<<pass.category<<"] "<<"Account: "<<pass.name<<", Password: "<<pass.password;
            if(!pass.login.empty())
                os<<", Login: "<<pass.login;
            if(!pass.www.empty())
                os<<", Domain: "<<pass.www;
            return os;
        }
    }
public:
    string name;
    string password;
    string category;
    string login;
    string www;
};


#endif //PASSWORDMANAGER_PASSACCOUNT_H
