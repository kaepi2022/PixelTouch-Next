/*
----------------------------------------------------------------
            PixelTouch Installer Source code v1.0
            
            2025/12/12  created by kaepi2022(MineKura)
                                ©2025 kaepi2022 

            This Project Using STL and OpenGL and SFML.
    I have great appreciation and respect for the developers.
----------------------------------------------------------------
*/

#ifndef MINITOOL_HPP
#define MINITOOL_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using u8 = unsigned char;

u8 mouse_clicked = 0;   //マウスのクリック
u8 mouse_clicked_one_frame_ago = 0;

void toolUpdate(){
    mouse_clicked_one_frame_ago = mouse_clicked;    //前フレームの状態を記録
    mouse_clicked = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ? 1 : 0;
}

//UTF-8(日本語など)に対応させるために変換するコード
sf::String to_sfString(const std::string& string_){
    return sf::String::fromUtf8(string_.begin(),string_.end());
}




#endif