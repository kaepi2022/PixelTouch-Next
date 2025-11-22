@echo off
set PATH=.\platform-tools\;%PATH%
color 0a
:main
title PixelTouch Installer
cls
echo.
echo.
echo.
echo    ==========================================================
echo          PixelTouch Installer v1.0
echo    ==========================================================
echo.
echo     Project : https://github.com/kaepi2022/pixeltouch
echo.
echo     Model :  TAB-A05-BD
echo     Build :  pixeltouch_catcafe-1.0.0_kaepi_release
echo     Author:  KAEPI2022 / MineKura
echo     ----------------------------------------------------------
echo.
echo    PixelTouch のインストーラーへようこそ！
echo.
echo     端末はチャレンジパッドNEO(TAB-A05-BD)であることを確認してください。
echo     進研ゼミの解約などを行っている前提で実行してください。
echo     万が一なにかが起こっても責任は一切取りませんのでご了承ください。
echo.
echo       モードのキーを入力してください↓  
echo           (i)端末にPixelTouchをインストール
echo           (f)AndroidシステムからFastbootを起動
echo           (b)Bootloaderのアンロック
echo           (n)インストーラー終了                   
echo.
set /p mode=モードの入力 ＞＞ 

if /I "%mode%" == "i" goto caveat_install_system
if /I "%mode%" == "f" goto caveat_reboot_boot_loader
if /I "%mode%" == "b" goto caveat_boot_loader_unlock
if /I "%mode%" == "n" goto exit_task

echo 無効なモードです。
goto main

:caveat_reboot_boot_loader
title fastbootの起動
cls
echo.
echo      ==========================================================
echo      PCと接続して、USBデバックがONの状態で作業を開始してください。
echo      ==========================================================
echo.
pause
goto reboot_boot_loader
echo.


:reboot_boot_loader
cls
adb reboot bootloader > nul 2>&1
if %errorlevel% neq 0 (
   echo  reboot to bootloader         FAILD :（
   goto error
) else (
   echo  reboot to bootloader         OK     :）
   echo.
   echo      ==========================================================
   echo            fastbootの起動に成功しました。
   echo      ==========================================================
   echo.
   pause
   goto main
)

:caveat_boot_loader_unlock
title bootloader アンロック
cls
echo.
echo      ==========================================================
echo      端末をfastboot モードで待機してください！！！！
echo      ==========================================================
echo.
echo   警告！　実行するとユーザーデータが削除されます。
echo   続ける場合は y を入力してください。
echo.
set /p sys_continue=入力してください ＞＞ 
if /I "%sys_continue%" == "y" goto bootloader_unlock
goto main

:bootloader_unlock
echo.
echo  画面に項目が出たらタブレットの音量ボタン+を押してください！！
echo.
fastboot flashing unlock > nul 2>&1
if %errorlevel% neq 0 (
   echo  boot loader unlock..         FAILD :（
   goto error
) else (
   echo  boot loader unlock..         OK     :）
   echo.
   echo      ==========================================================
   echo           bootloaderのアンロックは無事完了しました。
   echo      ==========================================================
   echo.
   pause
   goto main
)

:error
echo.
title 実行エラー :（
echo      ==========================================================
echo      実行中にエラーが発生しました。 再度お試しください。
echo      ==========================================================
echo.
pause
goto main

:exit_task
cls
echo.
echo  GoodBye :）
echo.
timeout /t 2 > nul
cls
color 07
title cmd.exe
cmd.exe

:caveat_install_system
title PixelTouch Nextのインストール
cls
echo.
echo   ==========================================================
echo      端末をfastboot モードで待機してください！！！！
echo   ==========================================================
echo.
echo   警告！　実行するとユーザーデータが削除されます。
echo   続ける場合は y を入力してください。
echo.
set /p sys_continue=入力してください ＞＞ 
if /I "%sys_continue%" == "y" goto install_system
goto main

:install_system
cls
echo.
echo.
echo.
echo   インストールを開始します！
echo   端末に触れないでください。
echo.

fastboot flash preloader imgs/preloader.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash preloader     FAILD :（
   goto error
) else (
   echo flash preloader     OK     :）
) 

fastboot flash partition imgs/pgpt.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash partition     FAILD :（
   goto error
) else (
   echo flash partition     OK     :）
) 

fastboot flash boot_para imgs/boot_para.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash boot_para     FAILD :（
   goto error
) else (
   echo flash boot_para     OK     :）
) 

fastboot flash cam_vpu1 imgs/cam_vpu1.img > nul 2>&1
fastboot flash cam_vpu2 imgs/cam_vpu2.img > nul 2>&1
fastboot flash cam_vpu3 imgs/cam_vpu3.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash cam_vpu       FAILD :（
   goto error
) else (
   echo flash cam_vpu       OK     :）
)

fastboot erase nvram > nul 2>&1
if %errorlevel% neq 0 (
   echo erase nvram         FAILD :（
   goto error
) else (
   echo erase nvram         OK     :）
)

fastboot erase protect1 > nul 2>&1
fastboot erase protect2 > nul 2>&1
if %errorlevel% neq 0 (
   echo erase protect       FAILD :（
   goto error
) else (
   echo erase protect       OK     :）
)

fastboot erase nvcfg > nul 2>&1
if %errorlevel% neq 0 (
   echo erase nvcfg         FAILD :（
   goto error
) else (
   echo erase nvcfg         OK     :）
)

fastboot flash lk imgs/lk.img > nul 2>&1
fastboot flash lk2 imgs/lk.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash lk            FAILD :（
   goto error
) else (
   echo flash lk            OK     :）
)

fastboot flash boot imgs/boot.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash boot          FAILD :（
   goto error
) else (
   echo flash boot          OK     :）
)

fastboot flash recovery imgs/recovery.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash recovery      FAILD :（
   goto error
) else (
   echo flash recovery      OK     :）
)

fastboot flash logo imgs/logo.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash logo          FAILD :（
   goto error
) else (
   echo flash logo          OK     :）
)

fastboot flash dtbo imgs/dtbo.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash dtbo          FAILD :（
   goto error
) else (
   echo flash dtbo          OK     :）
)

fastboot erase expdb > nul 2>&1
if %errorlevel% neq 0 (
   echo erase expdb         FAILD :（
   goto error
) else (
   echo erase expdb         OK     :）
)

fastboot flash frp imgs/frp.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash frp           FAILD :（
   goto error
) else (
   echo flash frp           OK     :）
)

fastboot erase nvdata > nul 2>&1
if %errorlevel% neq 0 (
   echo erase nvdata        FAILD :（
   goto error
) else (
   echo erase nvdata        OK     :）
)

fastboot flash tee1 imgs/tee.img > nul 2>&1
fastboot flash tee2 imgs/tee.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash tee           FAILD :（
   goto error
) else (
   echo flash tee           OK     :）
)

fastboot erase kb > nul 2>&1
if %errorlevel% neq 0 (
   echo erase kb            FAILD :（
   goto error
) else (
   echo erase kb            OK     :）
)

fastboot erase dkb > nul 2>&1
if %errorlevel% neq 0 (
   echo erase dkb           FAILD :（
   goto error
) else (
   echo erase dkb           OK     :）
)

fastboot erase metadata > nul 2>&1
if %errorlevel% neq 0 (
   echo erase metadata      FAILD :（
   goto error
) else (
   echo erase metadata      OK     :）
)

fastboot --disable-verity flash vbmeta imgs/vbmeta.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash vbmeta        FAILD :（
   goto error
) else (
   echo flash vbmeta        OK     :）
)

fastboot flash system imgs/system.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash system        FAILD :（
   goto error
) else (
   echo flash system        OK     :）
)

fastboot flash vendor imgs/vendor.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash vendor        FAILD :（
   goto error
) else (
   echo flash vendor        OK     :）
)

fastboot flash factory imgs/factory.img > nul 2>&1
if %errorlevel% neq 0 (
   echo flash factory       FAILD :（
   goto error
) else (
   echo flash factory       OK     :）
)

fastboot erase cache > nul 2>&1
if %errorlevel% neq 0 (
   echo erase cache         FAILD :（
   goto error
) else (
   echo erase cache         OK     :）
)

fastboot erase userdata > nul 2>&1
if %errorlevel% neq 0 (
   echo erase userdata      FAILD :（
   goto error
) else (
   echo erase userdata      OK     :）
)

fastboot erase cache > nul 2>&1
fastboot erase userdata > nul 2>&1
if %errorlevel% neq 0 (
   echo wipe final          FAILD :（
   goto error
) else (
   echo wipe final          OK     :）
)

fastboot reboot > nul 2>&1
if %errorlevel% neq 0 (
   echo reboot              FAILD :（
) else (
   echo reboot              OK     :）
)

echo.
echo      ==========================================================
echo            PixelTouchのインストールに成功しました！ :）
echo      念の為、再度リカバリーから初期化してから再起動してください。
echo      ==========================================================
echo.
pause
goto main