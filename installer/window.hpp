/*
----------------------------------------------------------------
            PixelTouch Installer Source code v1.0
            
            2025/12/12  created by kaepi2022(MineKura)
                                ©2025 kaepi2022 

            This Project Using STL and OpenGL and SFML.
    I have great appreciation and respect for the developers.
----------------------------------------------------------------
*/

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "mini_tool.hpp"
#include "config.hpp"

sf::Image window_icon(path_gui_installer_icon);

sf::RenderWindow window;
std::string window_name;

//ウィンドウの制限
void setWindow_restrictions(){
    if(window.isOpen()) window.close();
    window.create( 
    sf::VideoMode( sf::Vector2u(window_size_x,window_size_y)),  //ウィンドウ生成
    to_sfString( window_name ),sf::Style::Titlebar);
    window.setIcon(window_icon);
    window.setFramerateLimit(PROGRAM_FPS);
}

void init_window(){
    if(window.isOpen()) window.close();
    window_name = (gm_window_name + config_device_NAME + "(" + config_device_MODEL + ")");
    window.create( 
    sf::VideoMode( sf::Vector2u(window_size_x,window_size_y)),  //ウィンドウ生成
    to_sfString(window_name)
    ,sf::Style::Titlebar | sf::Style::Close                       //ウィンドウ名
    );
    window.setFramerateLimit(PROGRAM_FPS);
    window.setIcon(window_icon);
}

#endif