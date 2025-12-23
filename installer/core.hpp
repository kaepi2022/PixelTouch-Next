/*
----------------------------------------------------------------
            PixelTouch Installer Source code
            
            2025/MM/DD    by kaepi2022(MineKura)

            This Project Using STL and OpenGL and SFML.
    I have great appreciation and respect for the developers.
----------------------------------------------------------------
*/

#ifndef CORE_HPP
#define CORE_HPP

#include <iostream>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <atomic>
#include <thread>

#include <cstdlib>
#include <sstream>

#include "mini_tool.hpp"
#include "window.hpp"
#include "object.hpp"

constexpr u8 ID_TITLE           = 0;
constexpr u8 ID_INSTALL_MESSAGE = 1;
constexpr u8 ID_INSTALLING      = 2;
constexpr u8 ID_INSTALLING_ERROR = 3;
constexpr u8 ID_INSTALLING_SUCSESS = 4;


u8 now_scene = ID_TITLE;



sf::Texture global_texture_button_1_bg( path_gui_button_bg_1 );
sf::Texture global_texture_button_2_bg( path_gui_button_bg_2 );

/*

このコードの需要がないので座標とかは適当にマジックナンバーとして書いてるw
*/

object       title_object_pixeltouch_logo({10.f,20.f},{250.f,50.f},path_gui_pixeltouch_logo);
object       title_object_device_with_droid({480.f,125.f},{300.f,200.f},path_gui_device_with_droid);
button       title_button_install({480.f,390.f},{300.f,74.f},&global_texture_button_2_bg,path_gui_button_install);
button       title_button_rebootbl({300.f,400.f},{150.f,56.f},&global_texture_button_1_bg,path_gui_button_rebootbl);
button       title_button_blu({120.f,400.f},{150.f,56.f},&global_texture_button_1_bg,path_gui_button_blu);
button       title_button_update({600.f,20.f},{150.f,56.f},&global_texture_button_1_bg,path_gui_button_update);
text         title_text_welcome_message({20.f,150.f},40.f,gm_home_welcome);
text         title_text_version_copylight({20.f,480.f},15.f,gm_version_copylight);
text         title_text_device_name({490.f,100.f},25.f,"");
text         title_text_install_build({350.f,480.f},15.f,"");
text         title_text_command_message({80.f,330.f},20.f,"");     //再起動ボタンなどを押したときに下に出てくるメッセージ

object        install_object_message_obj({250.f,200.f},{300.f,200.f},path_gui_install_message_obj);
object        install_object_obj({450.f,180.f},{300.f,300.f},path_gui_installing_obj);
object        install_object_sucsess_obj({250.f,125.f},{300.f,300.f},path_gui_install_sucsess_obj);    
object        install_object_error_obj({250.f,200.f},{300.f,200.f},path_gui_install_error_obj);    
button        install_button_back({50.f,400.f},{150.f,56.f},&global_texture_button_1_bg,path_gui_button_back);
button        install_button_next({600.f,400.f},{150.f,56.f},&global_texture_button_1_bg,path_gui_button_next);
button        install_button_complete({600.f,400.f},{150.f,56.f},&global_texture_button_1_bg,path_gui_button_complete);
button        install_button_retry({600.f,400.f},{150.f,56.f},&global_texture_button_1_bg,path_gui_button_retry);

text          install_text_step_message({250.f,20.f},35.f,gm_install_step_message);
text          install_text_step_message_mini({150.f,80.f},20.f,gm_install_step_message_mini);
text          install_text_next_button_message({200.f,100.f},20.f,gm_install_next_button_message_defalut);

text          install_text_log_sucsess_message({150.f,20.f},35.f,gm_installing_log_sucsess);
text          install_text_log_sucsess_message_mini({80.f,80.f},20.f,gm_installing_log_sucsess_pls_message);
text          install_text_log_error_message({170.f,20.f},35.f,gm_installing_log_error);
text          install_text_log_error_message_mini({225.f,125.f},20.f,gm_installing_log_error_tips);
text          install_text_installing_log({40.f,130.f},35.f,"");




constexpr u8 DEVICE_INFO_COUNT = 4;     //デバイス情報の量

void init_installer(){
    std::ifstream device_info_file(path_config_device);
    std::string info_data;

    for(int i = 0; i < DEVICE_INFO_COUNT; i++){
        std::getline(device_info_file,info_data);
        if(info_data.substr(0,1) == "M") config_device_MODEL = info_data.substr(6);
        else if(info_data.substr(0,1) == "N") config_device_NAME = info_data.substr(5);
        else if(info_data.substr(0,1) == "B") config_build_id = info_data.substr(6);
        else if(info_data.substr(0,1) == "F") config_device_MODEL_FSBT = info_data.substr(11);
    }

    device_info_file.close();
}

std::atomic<bool> is_execution_completed;
std::atomic<int> execution_result;
const char*      cmd_hide_option = " > nul 2>&1";   //実行中にコマンドを非表示にする。

void execution(const std::string& cmd,bool log = false){
    if(is_execution_completed.load()) return;   //もし完了状態になっていれば処理禁止

    int result = std::system(
        (cmd + (!log ? cmd_hide_option : "") ).c_str() //非表示コマンドとして変換
    );

    is_execution_completed.store(true); //処理完了だと伝える
    execution_result.store(result);      //評価結果を書き込み
}

std::thread *command_execute_thread = nullptr;     //いろいろと実行するスレッドくん

namespace install_{
    std::ifstream install_prompt;

    std::string test;

    u8 device_situation = 0;
    constexpr u8 DEVICE_SITUATION_BOOTLOADER_OK = (1 << 0);
    constexpr u8 DEVICE_SITUATION_DEVICE_OK = (1 << 1);

    u8 install_scene_mode = 0;  //0はモードなし
    constexpr u8 INSTALL_SCENE_MODE_INSTALL = 1;
    constexpr u8 INSTALL_SCENE_MODE_UPDATE  = 2;

    bool install_message_next_button_clicked = false;

    void reset_install_messege_scene_next_button_text(){
        install_text_next_button_message.sf_getObject().setPosition({200.f,100.f});
        install_text_next_button_message.sf_getObject().setString( to_sfString(gm_install_next_button_message_defalut) );
        install_text_next_button_message.sf_getObject().setFillColor(sf::Color::Black);
    }

    void check_device_situation(){
        std::ifstream  device_prop;
        device_situation = 0;
        std::string check_;
        std::system((path_fastboot + std::string(" devices > .\\") + path_save_fsbt_logfile + std::string(" 2>&1")).c_str());
        device_prop.open(path_save_fsbt_logfile);
        std::getline(device_prop,check_);

        if(check_.length() > 0){
            device_situation |=  DEVICE_SITUATION_BOOTLOADER_OK;
            device_prop.close();

            std::system((path_fastboot + std::string("getvar product > .\\") + path_save_fsbt_logfile + std::string(" 2>&1")).c_str());
            device_prop.open(path_save_fsbt_logfile);
            std::getline(device_prop,check_);
            //9は「product: 」の9文字です...マジックナンバーですはい...
            if(check_.substr(9) == config_device_MODEL_FSBT) device_situation |=  DEVICE_SITUATION_DEVICE_OK ;
            test = check_.substr(9);
            device_prop.close();
        }else{
            device_prop.close();
        }       

        is_execution_completed.store(true);
    } 


    void install_sucsess_scene(){
        if(install_button_complete.is_selected()) install_button_complete.sf_getBgObject().setFillColor(gui_button_color_selected);
        else install_button_complete.sf_getBgObject().setFillColor(gui_button_color_normal);

        if(install_button_complete.is_selected() && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0 ){
            if(command_execute_thread != nullptr){
                command_execute_thread->join();
                delete command_execute_thread;
                command_execute_thread = nullptr;
            } 
            title_text_command_message.sf_getObject().setString("");
            now_scene = ID_TITLE;

            execution_result.store(0);
            is_execution_completed.store(false);
        }


        window.clear(sf::Color::White);
        install_text_log_sucsess_message.draw(window);
        install_text_log_sucsess_message_mini.draw(window);
        install_object_sucsess_obj.draw(window);
        install_button_complete.draw(window);
        window.display();
    }

    void install_error_scene(){
        if(install_button_retry.is_selected()) install_button_retry.sf_getBgObject().setFillColor(gui_button_color_selected);
        else install_button_retry.sf_getBgObject().setFillColor(gui_button_color_normal);

        if(install_button_complete.is_selected() && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0 ){
            if(command_execute_thread != nullptr){
                command_execute_thread->join();
                delete command_execute_thread;
                command_execute_thread = nullptr;
            } 

            execution_result.store(0);
            is_execution_completed.store(false);
            now_scene = ID_INSTALL_MESSAGE;
        }


        window.clear(sf::Color::White);
        install_object_error_obj.draw(window);
        install_text_log_error_message.draw(window);
        install_text_log_error_message_mini.draw(window);
        install_button_retry.draw(window);

        window.display();
    }

    void install_message_scene(){
        
        if(install_button_next.is_selected()) install_button_next.sf_getBgObject().setFillColor(gui_button_color_selected);
        else install_button_next.sf_getBgObject().setFillColor(gui_button_color_normal);

        if(install_button_back.is_selected()) install_button_back.sf_getBgObject().setFillColor(gui_button_color_selected);
        else install_button_back.sf_getBgObject().setFillColor(gui_button_color_normal);

        
        if(install_button_next.is_selected() && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0 && !install_message_next_button_clicked){
            if(command_execute_thread != nullptr) delete command_execute_thread;
            install_message_next_button_clicked = true;
            command_execute_thread = new std::thread(check_device_situation);

        }

        if(install_button_back.is_selected() && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0){
            if(command_execute_thread != nullptr) delete command_execute_thread;    //安全第一w
            
            reset_install_messege_scene_next_button_text();
            title_text_command_message.sf_getObject().setString("");
            now_scene = ID_TITLE;

            install_scene_mode = 0; //無にする。
        }

        if(install_message_next_button_clicked && is_execution_completed.load()){
            command_execute_thread->join();
            install_message_next_button_clicked = false;
            is_execution_completed.store(false);
            delete command_execute_thread;
            command_execute_thread = nullptr;

            if((device_situation & DEVICE_SITUATION_BOOTLOADER_OK) > 0 && (device_situation & DEVICE_SITUATION_DEVICE_OK) > 0){
                reset_install_messege_scene_next_button_text();
                command_execute_thread = new std::thread(execution,path_fastboot + std::string("devices"),false); //とりあえず
                setWindow_restrictions();  //バツボタン禁止
                now_scene = ID_INSTALLING;

                if(install_prompt.is_open()) install_prompt.close();    //もしファイルが開いたままだと閉じる。

                if(install_scene_mode == INSTALL_SCENE_MODE_INSTALL) install_prompt.open(path_config_install_system);       //システムインストールモードならinstall.config
                else if(install_scene_mode == INSTALL_SCENE_MODE_UPDATE) install_prompt.open(path_config_system_update);   //システムアップデートモードならupdate.config
                
            }
            
            if((device_situation & DEVICE_SITUATION_BOOTLOADER_OK) <= 0){
                install_text_next_button_message.sf_getObject().setFillColor(gui_button_color_using);
                install_text_next_button_message.sf_getObject().setString(to_sfString( gm_install_next_button_message_no_bootloader_mode ));
                install_text_next_button_message.sf_getObject().setPosition({180.f,100.f});

            }else if((device_situation & DEVICE_SITUATION_DEVICE_OK) <= 0){
                install_text_next_button_message.sf_getObject().setFillColor(gui_button_color_using);
                install_text_next_button_message.sf_getObject().setString(to_sfString( gm_install_next_button_message_diffirent_device_1 + config_device_NAME + std::string("(") + config_device_MODEL + std::string(")") +  gm_install_next_button_message_diffirent_device_2));
                install_text_next_button_message.sf_getObject().setPosition({150.f,100.f});
            }

        }



        window.clear(sf::Color::White);

        install_text_step_message.draw(window);
        install_text_step_message_mini.draw(window);
        install_object_message_obj.draw(window);
        install_button_back.draw(window);
        install_button_next.draw(window);
        install_text_next_button_message.draw(window);
        window.display();
    }

    std::string command;

    void install_scene(){
        if( is_execution_completed.load() && !install_prompt.eof()){

            if(command_execute_thread != nullptr) command_execute_thread->join();

            if(execution_result.load() > 0){
                install_text_installing_log.sf_getObject().setString(to_sfString(
                    ("エラー")
                ));
                install_prompt.close();
                execution_result.store(0);
                is_execution_completed.store(false);
                if(command_execute_thread != nullptr){
                    delete command_execute_thread;
                    command_execute_thread = nullptr;
                }
                init_window();
                now_scene = ID_INSTALLING_ERROR;

                if(install_scene_mode == INSTALL_SCENE_MODE_INSTALL){
                    install_text_log_error_message.sf_getObject().setString(to_sfString(gm_installing_log_error));
                    install_text_step_message.sf_getObject().setString( to_sfString(gm_install_step_message) );
                }else if(install_scene_mode == INSTALL_SCENE_MODE_UPDATE){
                    install_text_log_error_message.sf_getObject().setString(to_sfString(gm_installing_update_log_error));
                    install_text_step_message.sf_getObject().setString( to_sfString(gm_install_update_step_message) );
                }

                return; //終了させる
            }

            is_execution_completed.store(false);

            std::getline(install_prompt,command);

            std::istringstream getinfo(command);

            std::string rabel;
            std::string path;
            std::string custom;     //語尾にある追加機能

            /*
                ラベル パッチの順に見る
                例:F_system .\\files\\imgs\\system.img
            */
            getinfo >> rabel >> path >> custom;

            std::string verfily   = rabel.substr(0,1);
            std::string partition = rabel.substr(2);
            
            if(verfily == "F"){
                if(command_execute_thread != nullptr) delete command_execute_thread;
                
                command_execute_thread = new std::thread(execution,
                    std::string(path_fastboot + std::string("flash ") + partition + " " + path + custom),false
                );

                install_text_installing_log.sf_getObject().setString(to_sfString(
                    (partition + (install_scene_mode == INSTALL_SCENE_MODE_INSTALL ? gm_installing_log_flash : gm_installing_update_log_flash))
                ));
            }
            else if(verfily == "E"){
                if(command_execute_thread != nullptr) delete command_execute_thread;
                command_execute_thread = new std::thread(execution,
                    std::string(path_fastboot + std::string("erase ") + partition),false
                );
                install_text_installing_log.sf_getObject().setString(to_sfString(
                    (partition + gm_installing_log_erase)
                ));
            }
            else if(verfily == "C"){
                if(command_execute_thread != nullptr) delete command_execute_thread;
                command_execute_thread = new std::thread(execution,
                    path_fastboot + partition,false //partitionってなってるけど、カスタムのやつね
                );

                install_text_installing_log.sf_getObject().setString(to_sfString(
                    (gm_installing_log_custom)
                ));
            }
        }

        if(install_prompt.eof()){
            execution_result.store(0);
            is_execution_completed.store(false);
            init_window();
            install_prompt.close();
            now_scene = ID_INSTALLING_SUCSESS;

            if(install_scene_mode == INSTALL_SCENE_MODE_INSTALL){
                install_text_log_sucsess_message.sf_getObject().setString(to_sfString(gm_installing_log_sucsess));
                install_text_log_sucsess_message_mini.sf_getObject().setString(to_sfString(gm_installing_log_sucsess_pls_message));
                install_text_log_sucsess_message_mini.sf_getObject().setPosition({80.f,80.f});
            }else if(install_scene_mode == INSTALL_SCENE_MODE_UPDATE){
                install_text_log_sucsess_message.sf_getObject().setString(to_sfString(gm_installing_update_log_sucsess));
                install_text_log_sucsess_message_mini.sf_getObject().setString(to_sfString(gm_installing_update_log_sucsess_pls_message));
                install_text_log_sucsess_message_mini.sf_getObject().setPosition({165.f,80.f});
            }

            install_scene_mode = 0; //無
        }

        window.clear(sf::Color::White);
        install_text_installing_log.draw(window);
        install_object_obj.draw(window);
        window.display();
    }
}

namespace title_{
    u8   lock_button_status = 0; //ロックされているボタンを表示する。　ロック対象は灰色にする。
    //ボタンロック 
    constexpr u8   LOCK_INSTALL_BUTTON = (1 << 0);
    constexpr u8   LOCK_BLU_BUTTON = (1 << 1);
    constexpr u8   LOCK_REBOOTBL_BUTTON = (1 << 2);
    
    
    constexpr u8 BUTTON_MODE_NONE  = 0;
    constexpr u8 BUTTON_MODE_REBOOT_BL  = 1;
    constexpr u8 BUTTON_MODE_BLU  = 2;
    u8   button_mode = BUTTON_MODE_NONE;   //ボタンモード


    void title_init(){
        title_button_blu.sf_getBgObject().setFillColor(gui_button_color_normal);
        title_button_rebootbl.sf_getBgObject().setFillColor(gui_button_color_normal);
        title_text_install_build.sf_getObject().setString( to_sfString(( gm_install_build + config_build_id )) );
        title_text_device_name.sf_getObject().setString( to_sfString(( config_device_NAME )) );
        
    }

    void title_scene(){

        if(title_button_install.is_selected() && (lock_button_status & LOCK_INSTALL_BUTTON) <= 0) title_button_install.sf_getBgObject().setFillColor(gui_button_color_selected);
        else if((lock_button_status & LOCK_INSTALL_BUTTON) <= 0) title_button_install.sf_getBgObject().setFillColor(gui_button_color_normal);
        else title_button_install.sf_getBgObject().setFillColor(gui_button_color_disabled);

        if(title_button_update.is_selected() && (lock_button_status & LOCK_INSTALL_BUTTON) <= 0) title_button_update.sf_getBgObject().setFillColor(gui_button_color_selected);
        else if((lock_button_status & LOCK_INSTALL_BUTTON) <= 0) title_button_update.sf_getBgObject().setFillColor(gui_button_color_normal);
        else title_button_update.sf_getBgObject().setFillColor(gui_button_color_disabled);

        if(title_button_blu.is_selected() && (lock_button_status & LOCK_BLU_BUTTON) <= 0) title_button_blu.sf_getBgObject().setFillColor(gui_button_color_selected);
        else if((lock_button_status & LOCK_BLU_BUTTON) <= 0) title_button_blu.sf_getBgObject().setFillColor(gui_button_color_normal);
        else if( button_mode != BUTTON_MODE_BLU ) title_button_blu.sf_getBgObject().setFillColor(gui_button_color_disabled);

        if(title_button_rebootbl.is_selected() && (lock_button_status & LOCK_REBOOTBL_BUTTON) <= 0) title_button_rebootbl.sf_getBgObject().setFillColor(gui_button_color_selected);
        else if((lock_button_status & LOCK_REBOOTBL_BUTTON) <= 0) title_button_rebootbl.sf_getBgObject().setFillColor(gui_button_color_normal);
        else if( button_mode != BUTTON_MODE_REBOOT_BL )  title_button_rebootbl.sf_getBgObject().setFillColor(gui_button_color_disabled);

        if(title_button_rebootbl.is_selected() && (lock_button_status & LOCK_REBOOTBL_BUTTON) <= 0 && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0 && button_mode == BUTTON_MODE_NONE){
            lock_button_status = LOCK_INSTALL_BUTTON | LOCK_BLU_BUTTON | ~LOCK_REBOOTBL_BUTTON; 
            setWindow_restrictions();
            button_mode = BUTTON_MODE_REBOOT_BL;
        }

        if(title_button_blu.is_selected() && (lock_button_status & LOCK_BLU_BUTTON) <= 0 && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0 && button_mode == BUTTON_MODE_NONE){
            lock_button_status = LOCK_INSTALL_BUTTON | ~LOCK_BLU_BUTTON | LOCK_REBOOTBL_BUTTON; 
            setWindow_restrictions();
            button_mode = BUTTON_MODE_BLU;
        }

        if(title_button_install.is_selected() && (lock_button_status & LOCK_INSTALL_BUTTON) <= 0 && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0 && button_mode == BUTTON_MODE_NONE){
            now_scene = ID_INSTALL_MESSAGE;
            install_::install_scene_mode =  install_::INSTALL_SCENE_MODE_INSTALL;
            install_text_step_message.sf_getObject().setString( to_sfString(gm_install_step_message) );
        }

        if(title_button_update.is_selected() && (lock_button_status & LOCK_INSTALL_BUTTON) <= 0 && (~mouse_clicked & mouse_clicked_one_frame_ago) > 0 && button_mode == BUTTON_MODE_NONE){
            now_scene = ID_INSTALL_MESSAGE;
            install_::install_scene_mode =  install_::INSTALL_SCENE_MODE_UPDATE;
            install_text_step_message.sf_getObject().setString( to_sfString(gm_install_update_step_message) );
        }

        switch (button_mode)
        {
        case BUTTON_MODE_REBOOT_BL:
            title_button_rebootbl.sf_getBgObject().setFillColor(gui_button_color_using);
            if(command_execute_thread == nullptr){
                title_text_command_message.sf_getObject().setString(to_sfString(gm_reboot_bootloader_started_message));
                command_execute_thread = new std::thread(execution,std::string(path_adb + std::string("reboot bootloader")),false);
            } 

            if(is_execution_completed.load()){
                command_execute_thread->join();
                
                int result =  execution_result.load();
                if(result == 0) title_text_command_message.sf_getObject().setString(to_sfString(gm_reboot_bootloader_sucsess));
                else if(result > 0) title_text_command_message.sf_getObject().setString(to_sfString(gm_bl_error));

                is_execution_completed.store(false);
                execution_result.store(0);
                delete command_execute_thread;
                command_execute_thread = nullptr;

                button_mode = BUTTON_MODE_NONE;
                lock_button_status = 0;
                init_window();
            }
            break;

        case BUTTON_MODE_BLU:
            title_button_blu.sf_getBgObject().setFillColor(gui_button_color_using);
            if(command_execute_thread == nullptr){
                title_text_command_message.sf_getObject().setString(to_sfString(gm_blu_started_message));
                command_execute_thread = new std::thread([](){
                    execution(path_fastboot + std::string("flashing unlock"));
                    is_execution_completed.store(false);
                    execution_result.store(0);
                    execution(path_fastboot + std::string("reboot bootloader"));
                    
                });
            } 

            if(is_execution_completed.load()){
                command_execute_thread->join();
                int result =  execution_result.load();
                title_text_command_message.sf_getObject().setString(to_sfString(gm_blu_sucsess));
                
                is_execution_completed.store(false);
                execution_result.store(0);
                delete command_execute_thread;
                command_execute_thread = nullptr;

                button_mode = BUTTON_MODE_NONE;
                lock_button_status = 0;
                init_window();
            }
            break;
        
        default:
            break;
        }


        window.clear(sf::Color::White);

        title_object_device_with_droid.draw(window);

        title_text_welcome_message.draw(window);
        title_object_pixeltouch_logo.draw(window);
        title_object_device_with_droid.draw(window);
        title_button_install.draw(window);
        title_button_blu.draw(window);
        title_button_update.draw(window);
        title_button_rebootbl.draw(window);
        title_text_version_copylight.draw(window);
        title_text_command_message.draw(window);
        title_text_install_build.draw(window);
        title_text_device_name.draw(window);

        window.display();


    }
}




#endif