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


// --- [ Config 定義 ] ---

// installer.configから取得
std::string config_device_MODEL;        // デバイス名
std::string config_device_NAME;         // デバイスの型番
std::string config_device_MODEL_FSBT;   // FASTBOOTで使用するデバイスモデル名
std::string config_build_id;            // ROMのビルドID (44文字など)


// ビルド時に決定
// Windowサイズ
constexpr unsigned int window_size_x = 800u;
constexpr unsigned int window_size_y = 500u;

constexpr unsigned int PROGRAM_FPS = 60u;

// ボタンの選択時などのカラー (PixelBlueから引用)
const sf::Color              gui_button_color_normal                  = sf::Color(0x42,0x85,0xF4);
const sf::Color              gui_button_color_selected                = sf::Color(0x52,0x69,0xD1);
const sf::Color              gui_button_color_disabled                = sf::Color(0x69,0x69,0x69);  // ボタンが無効化されているときのカラー
const sf::Color              gui_button_color_using                   = sf::Color(0xEA,0x43,0x35 );  // コマンド実行中に変わるカラー

// --- [ パス定義 ] ---

// GUIリソースのパス
const char* path_gui_item_font               = "files/gui/Font.ttf";
const char* path_gui_pixeltouch_logo         = "files/gui/pixeltouch.png";
const char* path_gui_installer_icon          = "files/gui/app_icon.png";

// ツール・ログのパス
const char* path_fastboot                    = ".\\files\\tools\\fastboot.exe ";
const char* path_adb                         = ".\\files\\tools\\adb.exe ";
const char* path_save_fsbt_logfile           = "device_check.tmp";


// デバイス・インストール設定（チャレンジパッドのシステムにより変更される）
const char* path_config_device               = "files/device.config";
const char* path_config_install_system       = "files/install.config";
const char* path_config_system_update       = "files/update.config";

// デバイス状態画像のパス
const char* path_gui_device_with_droid       = "files/device/device_w_droid.png";
const char* path_gui_install_error_obj       = "files/device/device_error.png";
const char* path_gui_install_sucsess_obj     = "files/device/device_sucsess.png";  

// ボタン画像のパス
const char* path_gui_button_bg_1             = "files/gui/button_bg_1.png";      
const char* path_gui_button_bg_2             = "files/gui/button_bg_2.png";
const char* path_gui_button_install          = "files/gui/gui_button_install_text.png";          
const char* path_gui_button_blu              = "files/gui/gui_button_bootloaderunlock_text.png";
const char* path_gui_button_rebootbl         = "files/gui/gui_button_rebootbootloader_text.png";      
const char* path_gui_button_next             = "files/gui/gui_button_next_text.png";
const char* path_gui_button_retry            = "files/gui/gui_button_retry_text.png";  
const char* path_gui_button_complete         = "files/gui/gui_button_complete_text.png";
const char* path_gui_button_back             = "files/gui/gui_button_back_text.png";
const char* path_gui_button_update           = "files/gui/gui_button_update_text.png"; 
const char* path_gui_install_message_obj     = "files/gui/install_message_obj.png";     
const char* path_gui_installing_obj          = "files/gui/installing_obj.png"; 

const     char* command_prompt_caveat                   = "Please don't touch and close command window!";    // 意図が明確になるよう修正

// --- [ GUIメッセージ定数 ] ---

const     char* gm_window_name        = "PixelTouch インストーラー  for ";
const     char* gm_home_welcome       = "ようこそ";


const     char* gm_version_copylight  = "Version 2.0 ©2025 Kaepi2022(MineKura)";
const     char* gm_install_build      = "ROMビルド番号: ";

const     char* gm_install_caveat     = "インストール中は、絶対にデバイスに触れないでください。";



const     char* gm_install_step_message                               = "インストール準備";
const     char* gm_install_update_step_message                        = "アップデート準備";
const     char* gm_install_step_message_mini                          = "USB接続後、Bootloaderモードで待機させてください。";
const     char* gm_install_next_button_message_defalut                = "「次へ」を押すとインストールを開始します。";
const     char* gm_install_next_button_message_diffirent_device_1     = "接続されたデバイスが";
const     char* gm_install_next_button_message_diffirent_device_2     = "\n　　　　　であることを確認できませんでした。";
const     char* gm_install_next_button_message_no_bootloader_mode     = "Bootloaderモードのデバイスを検出できませんでした。";
const     char* gm_installing_log_flash                               = "を書き込んでいます。";
const     char* gm_installing_update_log_flash                        = "を更新しています。";
const     char* gm_installing_log_erase                               = "を初期化しています。";
const     char* gm_installing_log_custom                              = "システム設定を調整しています。";
const     char* gm_installing_log_error                               = "　　問題が発生しました。\nインストールを終了します。";
const     char* gm_installing_update_log_error                        = "　　問題が発生しました。\nアップデートを終了します。";
const     char* gm_installing_log_error_tips                          = "USBなどの接続状態を確認してください。";

const     char* gm_installing_log_sucsess                             = "インストールが完了しました。"; // 成功しました -> 完了しました (より自然な表現)
const     char* gm_installing_update_log_sucsess                      = "アップデートが完了しました。"; // 成功しました -> 完了しました (より自然な表現)
const     char* gm_installing_log_sucsess_pls_message                 = "PixelTouchがお使いのデバイスに正常にインストールされました！"; // 読点を追加し、自然な表現に
const     char* gm_installing_update_log_sucsess_pls_message          = "PixelTouchが正常にアップデートされました！"; // 読点を追加し、自然な表現に

const     char* gm_reboot_bootloader_started_message                  = "デバイスをブートローダーモードに再起動中です..."; // 再起動中です... に修正
const     char* gm_reboot_bootloader_sucsess                          = "ブートローダーモードへの再起動に成功しました。"; // 問題なし

const     char* gm_bl_error                                           = "エラーが発生しました。デバイスの接続状態を確認してください。"; // 接続状態に修正

const     char* gm_blu_started_message                                = "ブートローダーアンロックを開始します。\nタブレット画面に項目が表示されたら、[+]を押してください。"; // 表現をより具体的、かつ丁寧な指示に
const     char* gm_blu_sucsess                                        = "ブートローダーアンロックに成功しました。"; // 問題なし

#endif
