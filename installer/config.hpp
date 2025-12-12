/*
----------------------------------------------------------------
            PixelTouch Installer Source code v1.0
            
            2025/12/12  created by kaepi2022(MineKura)
                                ©2025 kaepi2022 

            This Project Using STL and OpenGL and SFML.
    I have great appreciation and respect for the developers.
----------------------------------------------------------------
*/

#ifndef COMP_HPP
#define COMP_HPP

#include <iostream>
#include <SFML/Graphics.hpp>


//installer.configから取得
std::string config_device_MODEL;     //デバイス名
std::string config_device_NAME  ;     //デバイスの型番
std::string config_device_MODEL_FSBT   ;     //FASTBOOTで使われる型番
std::string config_build_id   ;     //ROMのビルドID44


//ビルド時に決定
//Windowサイズ
constexpr unsigned int window_size_x = 800u;
constexpr unsigned int window_size_y = 500u;

constexpr unsigned int PROGRAM_FPS = 60u;

//ボタンの選択時などのカラー、PixelBlueから取りました...
const sf::Color              gui_button_color_normal                  = sf::Color(0x42,0x85,0xF4);
const sf::Color              gui_button_color_selected                = sf::Color(0x52,0x69,0xD1);
const sf::Color              gui_button_color_disabled                = sf::Color(0x69,0x69,0x69);  //ボタンが無効化されているときのカラー
const sf::Color              gui_button_color_using                = sf::Color(0xEA,0x43,0x35 );  //ボタンのコマンドを実行中に変わる色

//画像のパッチ集
const char*            path_gui_item_font               = "files/gui/Font.ttf";
const char*            path_gui_pixeltouch_logo         = "files/gui/pixeltouch.png";
const char*            path_gui_installer_icon          = "files/gui/app_icon.png";

const char*            path_fastboot                    = ".\\files\\tools\\fastboot.exe ";
const char*            path_adb                         = ".\\files\\tools\\adb.exe ";
const char*            path_save_fsbt_logfile           = "device_cheak.tmp";


//以下の2つはチャレンジパッドのシステム時に変わる
const char*            path_config_device            = "files/device.config";
const char*            path_config_install_system       = "files/install.config";
const char*            path_gui_device_with_droid       = "files/device/device_w_droid.png";
const char*            path_gui_install_error_obj       = "files/device/device_error.png";
const char*            path_gui_install_sucsess_obj     = "files/device/device_sucsess.png";  

const char*            path_gui_button_bg_1             = "files/gui/button_bg_1.png";      
const char*            path_gui_button_bg_2             = "files/gui/button_bg_2.png";
const char*            path_gui_button_install          = "files/gui/gui_button_install_text.png";          
const char*            path_gui_button_blu              = "files/gui/gui_button_bootloaderunlock_text.png";
const char*            path_gui_button_rebootbl         = "files/gui/gui_button_rebootbootloader_text.png";      
const char*            path_gui_button_next             = "files/gui/gui_button_next_text.png";
const char*            path_gui_button_retry            = "files/gui/gui_button_retry_text.png";  
const char*            path_gui_button_complete         = "files/gui/gui_button_complete_text.png";
const char*            path_gui_button_back             = "files/gui/gui_button_back_text.png";
const char*            path_gui_install_message_obj     = "files/gui/install_message_obj.png";     
const char*            path_gui_installing_obj          = "files/gui/installing_obj.png"; 

const     char* command_prompt_caveat                   = "Please don't close command prompt!!";    //要するにコマンドプロンプトを閉じたら死にますよってこと

//GUIメッセージ
const     char* gm_window_name        = "PixelTouch インストーラー  for ";
const     char* gm_home_welcome       = "ようこそ";


const     char* gm_version_copylight  = "Version 1.0 ©2025 Kaepi2022(MineKura)";
const     char* gm_install_build      = "ROMビルド番号 : ";

const     char* gm_install_caveat     = "絶対にデバイスを触らないでください。";



const     char* gm_install_step_message                               = "インストールを始める前に";
const     char* gm_install_step_message_mini                          = "USBを繋ぎ、Bootloaderモードで待機してください。";
const     char* gm_install_next_button_message_defalut                = "「次へ」を押すとインストールを始めます。";
const     char* gm_install_next_button_message_diffirent_device_1     = "お使いのデバイスが";
const     char* gm_install_next_button_message_diffirent_device_2     = "\n　　　　　であることを確認できませんでした。";
const     char* gm_install_next_button_message_no_bootloader_mode     = "Bootloaderモードを検出できませんでした。";
const     char* gm_installing_log_flash                               = "をインストールしています。";
const     char* gm_installing_log_erase                               = "を初期化しています。";
const     char* gm_installing_log_custom                              = "システムを調整しています。";

const     char* gm_installing_log_error                               = "　　問題が発生したため、\nインストールを終了しました。";
const     char* gm_installing_log_error_tips                          = "USBなどの接続を確認してください。";

const     char* gm_installing_log_sucsess                             = "インストールに成功しました！";
const     char* gm_installing_log_sucsess_pls_message                 = "正しくPixelTouchを起動させるにはリカバリーからの初期化が必要です。";

const     char* gm_reboot_bootloader_started_message                  = "ブートローダーモードに再起動しています...";
const     char* gm_reboot_bootloader_sucsess                          = "ブートローダーモードへの再起動に成功しました。";

const     char* gm_bl_error                                           = "エラーが発生しました。デバイスの接続を確認してください。";

const     char* gm_blu_started_message                                = "ブートローダーをアンロックします。\n何かしらの項目がタブレットにでたら[+]を押してください。";
const     char* gm_blu_sucsess                                        = "ブートローダーアンロックに成功しました。";

#endif