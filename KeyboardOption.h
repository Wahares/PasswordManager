//
// Created by Wahares on 18.05.2023.
//

#ifndef PASSWORDMANAGER_KEYBOARDOPTION_H
#define PASSWORDMANAGER_KEYBOARDOPTION_H
#include <functional>
#include <iostream>

struct KeyboardOption{
    friend std::ostream& operator<<(std::ostream &os, const KeyboardOption& k) {
        {
            if (!k.key.empty() && !k.destription.empty())
                os<< "[" << k.key << "] - " << k.destription << std::endl;
            return os;
        }
    }
public:
    std::string key;
    std::string destription;
    std::function<void(std::string answer)> action;
};


#endif //PASSWORDMANAGER_KEYBOARDOPTION_H
