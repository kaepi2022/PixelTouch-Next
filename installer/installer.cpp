#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cstdint>

const char *android_imgs    = "./imgs/";
const char *use_file_format = ".img";
const char *config_file     = "setup_config.cfg";

const char *ELEMENT_DEVICE_NAME            = "_DN";
const char *ELEMENT_INSTALL_SERVICE_NAME   = "_IS";
const char *ELEMENT_TITLE_NAME             = "_TN";
const char *ELEMENT_BUILD_NAME             = "_BN";
const char *ELEMENT_AUTHOR_NAME            = "_AN";
const char *ELEMENT_COLOR_ID               = "_CI";

const char *FASTBOOT_TYPE_FLASH            = "f_";
const char *FASTBOOT_TYPE_SYSTEM           = "s_";
const char *FASTBOOT_TYPE_ERASE            = "e_";

const char *MESSAGE_FASTBOOT_LOG_FLASH_1   = " flashing ";
const char *MESSAGE_FASTBOOT_LOG_FLASH_2   = ".....  ";
const char *MESSAGE_FASTBOOT_LOG_REBOOT    = " started to reboot...  ";
const char *MESSAGE_FASTBOOT_LOG_ERASE_1   = " erasing ";
const char *MESSAGE_FASTBOOT_LOG_ERASE_2   = ".....  ";
const char *MESSAGE_FASTBOOT_LOG_SUCCESS   = "  OK :)\n";
const char *MESSAGE_FASTBOOT_LOG_FAILED    = "  FAILED X(\n";
const char *MESSAGE_ERROR                  = "\nHappened error! Please try again. :(";
const char *MESSAGE_COMPLETED              = "\n     Installation Complete! \n    Your device is rebooting...";

const char *GUI_BANNER_1                   = "==========================================================";
const char *GUI_BANNER_2                   = "----------------------------------------------------------";
const char *GUI_BUILD_NAME                 = "  Build  : ";
const char *GUI_AUTHOR_NAME                = "  Author : ";
const char *GUI_DEVICE_MODEL               = "  Model  : ";
const char *GUI_USER_INPUT                 = "INPUT >> ";

const char *MESSAGE_HELLO_TITLE_MESSAGE_1  = "Welcome to ";
const char *MESSAGE_HELLO_TITLE_MESSAGE_2  = "!!!! :)";
const char *MESSAGE_HELLO_TITLE_MESSAGE_3  =
    "WHEN YOU COMPLETED TO CONNECT USB PLUG\n"
    "WITH ANDROID DEVICE, PLEASE INPUT MODE KEY.\n\n"
    "    Install System in your device : Input (y)\n"
    "    Exit this program             : other keys\n";

const char *MESSAGE_EXECUTING_CAVEAT       = "CAVEAT --- DO NOT PULL USB PLUG OR DEVICE!!!";

int main() {
    std::ifstream config_settings_file(config_file);

    if (!config_settings_file) {
        std::cout << "Not found setup_config.cfg file! :(";
        return -1;
    }

    std::string file_config;
    std::string text_title_name;
    std::string text_device_name;
    std::string text_install_service_name;
    std::string text_build_name;
    std::string text_author_name;
    std::string element_name;

    // 設定ファイルからヘッダー情報を取得
    for (int i = 0; i < 6 && std::getline(config_settings_file, file_config); i++) {
        if (file_config.length() < 3) continue;
        element_name = file_config.substr(0, 3);

        if (element_name == ELEMENT_DEVICE_NAME)
            text_device_name = file_config.substr(4);
        else if (element_name == ELEMENT_TITLE_NAME)
            text_title_name = file_config.substr(4);
        else if (element_name == ELEMENT_INSTALL_SERVICE_NAME)
            text_install_service_name = file_config.substr(4);
        else if (element_name == ELEMENT_BUILD_NAME)
            text_build_name = file_config.substr(4);
        else if (element_name == ELEMENT_AUTHOR_NAME)
            text_author_name = file_config.substr(4);
        else if (element_name == ELEMENT_COLOR_ID)
            std::system(("color " + file_config.substr(4, 2)).c_str());
    }

    if (text_device_name.empty() || text_install_service_name.empty() ||
        text_build_name.empty() || text_author_name.empty() || text_title_name.empty()) {
        std::cout << "Missing setup parameters in config file!";
        return -1;
    }

    // GUIバナー表示
    std::cout << "\n"
              << GUI_BANNER_1 << "\n"
              << text_title_name << "\n"
              << GUI_BANNER_1 << "\n\n"
              << GUI_DEVICE_MODEL << text_device_name << "\n"
              << GUI_BUILD_NAME << text_build_name << "\n"
              << GUI_AUTHOR_NAME << text_author_name << "\n"
              << GUI_BANNER_2 << "\n\n"
              << MESSAGE_HELLO_TITLE_MESSAGE_1 << text_install_service_name
              << MESSAGE_HELLO_TITLE_MESSAGE_2 << "\n\n"
              << MESSAGE_HELLO_TITLE_MESSAGE_3 << "\n"
              << GUI_USER_INPUT;

    // ユーザー入力処理
    char y_or_n;
    std::cin >> y_or_n;
    if (y_or_n != 'y' && y_or_n != 'Y') return 0;

    std::cout << "\n\n" << MESSAGE_EXECUTING_CAVEAT << std::endl;

    std::string fastboot_id;
    int return_code = 0;

    // 実際のフラッシュ処理
    while (std::getline(config_settings_file, file_config)) {
        if (file_config.size() < 3) continue;

        element_name = file_config.substr(0, 2);
        fastboot_id = file_config.substr(2);

        if (element_name == FASTBOOT_TYPE_FLASH) {
            std::cout << MESSAGE_FASTBOOT_LOG_FLASH_1 << fastboot_id << MESSAGE_FASTBOOT_LOG_FLASH_2;
            return_code = std::system(("fastboot flash " + fastboot_id + " " +
                                       std::string(android_imgs) + fastboot_id + use_file_format +
                                       " > nul 2>&1").c_str());
            std::cout <<  (return_code == 0 ? MESSAGE_FASTBOOT_LOG_SUCCESS : MESSAGE_FASTBOOT_LOG_FAILED);  
        } else if (element_name == FASTBOOT_TYPE_ERASE) {
            std::cout << MESSAGE_FASTBOOT_LOG_ERASE_1 << fastboot_id << MESSAGE_FASTBOOT_LOG_ERASE_2;
            return_code = std::system(("fastboot erase " + fastboot_id + " > nul 2>&1").c_str());
            std::cout <<  (return_code == 0 ? MESSAGE_FASTBOOT_LOG_SUCCESS : MESSAGE_FASTBOOT_LOG_FAILED);  
        } else if (element_name == FASTBOOT_TYPE_SYSTEM) {
            return_code = std::system(("fastboot " + fastboot_id + " > nul 2>&1").c_str());
            std::cout <<  (return_code == 0 ? MESSAGE_FASTBOOT_LOG_SUCCESS : MESSAGE_FASTBOOT_LOG_FAILED);  
        }

        if (return_code != 0) {
            std::cout << "\n" << GUI_BANNER_1 << "\n" << MESSAGE_ERROR << "\n" << GUI_BANNER_1 << std::endl;
            config_settings_file.close();
            return -1;
        }
    }

    std::cout << "\n" << GUI_BANNER_1 << "\n" << MESSAGE_COMPLETED << "\n" << GUI_BANNER_1 << std::endl;
            config_settings_file.close();
    config_settings_file.close();
    return 0;
}