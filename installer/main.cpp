/*
----------------------------------------------------------------
            PixelTouch Installer Source code v1.0
            
            2025/12/12  created by kaepi2022(MineKura)
                                ©2025 kaepi2022 

            This Project Using STL and OpenGL and SFML.
    I have great appreciation and respect for the developers.
----------------------------------------------------------------
*/

#include <iostream>

#include "core.hpp"

int main(){

    
    init_installer();           //device.configを読み込み

    init_window();              //Windowの初期化
    title_::title_init();

    //閉じるなと
    std::cout << command_prompt_caveat << std::endl;

    is_execution_completed.store(false);

    while(window.isOpen()){

        while(const std::optional events = window.pollEvent()){
            if(events->getIf<sf::Event::Closed>()){
                if(command_execute_thread != nullptr){
                    command_execute_thread->join();
                    delete command_execute_thread;
                    command_execute_thread = nullptr;
                } 
                window.close();
                exit(0);
                break;
            }
        }


        switch (now_scene)
        {
        case ID_TITLE:    
            title_::title_scene();
            break;
        case ID_INSTALL_MESSAGE:
            install_::install_message_scene();
            break;
        case ID_INSTALLING:
        install_::install_scene();
            //install_::install_scene();
            break;
        case ID_INSTALLING_ERROR:
            install_::install_error_scene();
            break;
        case ID_INSTALLING_SUCSESS:
            install_::install_sucsess_scene();
            break;
        default:
            break;
        }
        toolUpdate();
    }
    
    if(command_execute_thread != nullptr){
        command_execute_thread->join();
        delete command_execute_thread;
        command_execute_thread = nullptr;
    } 

    return 0;

}