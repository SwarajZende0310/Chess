#pragma once
#include<string>

std::string GetResourceDir()
{
    #ifdef NDEBUG //release build
        return "assets/";
    #else
        return "/home/swarajzende/Chess/ChessGame/assets/";
    #endif
}
