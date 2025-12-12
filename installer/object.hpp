/*
----------------------------------------------------------------
            PixelTouch Installer Source code v1.0
            
            2025/12/12  created by kaepi2022(MineKura)
                                ©2025 kaepi2022 

            This Project Using STL and OpenGL and SFML.
    I have great appreciation and respect for the developers.
----------------------------------------------------------------
*/

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "config.hpp"
#include "window.hpp"
#include "mini_tool.hpp"

sf::Font font( path_gui_item_font );   //フォント

class object{
    protected:
        sf::RectangleShape obj;
        sf::Texture texture;

    public:
        object(sf::Vector2f position,sf::Vector2f size,const std::string& texture_path) : obj(size){
            obj.setPosition(position);
            if(!texture.loadFromFile(texture_path) ) exit(0);

            obj.setTexture(&texture);
        }
        

        void draw(sf::RenderWindow &window){
            window.draw(obj);
        }

        sf::RectangleShape* sf_getObject(){
            return &obj;
        }
};

class button{
    private:
        bool lock_button = false;
        sf::RectangleShape button_bg;
        sf::RectangleShape button_text;

        sf::Texture text_texture;

    public:
        //DRY防止で先にコンストラクタで使う関数書く
        void setPosition(sf::Vector2f position){
            button_bg.setPosition(position);
            button_text.setPosition(position);
        }

        void setSize(sf::Vector2f size){
            button_bg.setSize(size);
            button_text.setSize(size);
        }

        button(sf::Vector2f position,sf::Vector2f size,sf::Texture* background_texture_sources,const std::string& text_texture_path){
            setSize(size);
            setPosition(position);
            if(!text_texture.loadFromFile(text_texture_path))  exit(0);

            button_bg.setTexture(background_texture_sources);
            button_text.setTexture(&text_texture);

            button_bg.setFillColor(gui_button_color_normal);
        }

        bool is_selected(){
            sf::Vector2f mouse_pos = {(float)sf::Mouse::getPosition(window).x,(float)sf::Mouse::getPosition(window).y};
            sf::Vector2f this_pos = button_bg.getPosition();
            sf::Vector2f this_size = button_bg.getSize();

            //軽AABB
            return {window.hasFocus() &&
                    mouse_pos.x >= this_pos.x  && mouse_pos.x <= this_pos.x + this_size.x
                &&  mouse_pos.y >= this_pos.y && mouse_pos.y <= this_pos.y + this_size.y
            };
        }

        sf::RectangleShape& sf_getBgObject(){
            return button_bg;
        }
        
        sf::RectangleShape& sf_getTextObject(){
            return button_text;
        }

        void draw(sf::RenderWindow &window){
            window.draw(button_bg);
            window.draw(button_text);
        }

};

class text{
    private:
        sf::Text obj;

    public:
        text(sf::Vector2f position,float scale,const std::string& init_ = std::string("Hello :)") ) : obj(font){
            obj.setPosition(position);
            obj.setCharacterSize(scale);
            obj.setString(to_sfString(init_));
            obj.setFillColor(sf::Color::Black);
        }

        void draw(sf::RenderWindow &window){
            window.draw(obj);
        }

        sf::Text& sf_getObject(){
            return obj;
        }
};

#endif