cmake_minimum_required(VERSION 3.25)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 20)

add_executable(PasswordManager main.cpp
        MyCryptography.cpp MyCryptography.h
        UiStateMachine.cpp UiStateMachine.h
        UiSingleView.h
        startUI.cpp startUI.h
        ViewTypes.h
        KeyboardOption.h
        passwordUI.cpp passwordUI.h
        MainController.cpp MainController.h
        PassAccount.h
        StringExpansion.cpp StringExpansion.h
        AccountViewUI.cpp AccountViewUI.h)