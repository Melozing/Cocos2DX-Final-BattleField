#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2015-2017 Chukong Technologies Inc.
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.6)

set(APP_NAME FinalBattleField)

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

# add cross-platforms source files and header files 
list(APPEND GAME_SOURCE
    Classes/AppDelegate.cpp
    Classes/HelloWorldScene.cpp
    Classes/Bullet/BulletPlayerGame3.cpp
    Classes/Button/BaseButton.cpp
    Classes/Button/ExitButton.cpp
    Classes/Button/ReplayButton.cpp
    Classes/Constants/Constants.cpp
    Classes/Controller/GameController.cpp
    Classes/Controller/SceneController.cpp
    Classes/Controller/SoundController.cpp
    Classes/Controller/SpriteController.cpp
    Classes/FX/Explodable.cpp
    Classes/Game1/Game1Scene.cpp
    Classes/Game1/Background/Background.cpp
    Classes/Game1/Effect/EffectObject.cpp
    Classes/Game1/Effect/EffectObjectPool.cpp
    Classes/Game1/Enemy/FallingObject.cpp
    Classes/Game1/Enemy/FallingRock.cpp
    Classes/Game1/Enemy/FallingTree.cpp
    Classes/Game1/Enemy/FanBullet.cpp
    Classes/Game1/Enemy/FlyingBullet.cpp
    Classes/Game1/Enemy/Landmine.cpp
    Classes/Game1/Enemy/RandomBoom.cpp
    Classes/Game1/Items/AmmoItem.cpp
    Classes/Game1/Items/CollectibleItem.cpp
    Classes/Game1/Items/HealthItem.cpp
    Classes/Game1/Player/HealthPlayerGame1.cpp
    Classes/Game1/Player/PlayerGame1.cpp
    Classes/Game1/Skills/ShieldSkill.cpp
    Classes/Game2/Game2Scene.cpp
    Classes/Game2/Cursor/Cursor.cpp
    Classes/Game2/Enemy/BunkerGame2.cpp
    Classes/Game2/Enemy/EnemyBase.cpp
    Classes/Game2/Enemy/EnemyUtils.cpp
    Classes/Game2/Enemy/MeleeEnemyGame2.cpp
    Classes/Game2/Enemy/SniperBulletGame2.cpp
    Classes/Game2/Enemy/SniperEnemyGame2.cpp
    Classes/Game2/Enemy/SuicideBomberEnemy.cpp
    Classes/Game2/Player/BulletGame2.cpp
    Classes/Game2/Player/Bullet_Petard.cpp
    Classes/Game2/Player/Cannon.cpp
    Classes/Game2/Player/Grenade.cpp
    Classes/Game2/Player/GroundCannonGame2.cpp
    Classes/Game2/Player/PlayerGame2.cpp
    Classes/Game3/Game3Scene.cpp
    Classes/Game3/enemy/BoomForEnemyPlane.cpp
    Classes/Game3/enemy/BulletForEnemyPlane.cpp
    Classes/Game3/enemy/EnemyPlaneBase.cpp
    Classes/Game3/enemy/EnemyPlaneBoom.cpp
    Classes/Game3/enemy/EnemyPlaneBoss.cpp
    Classes/Game3/enemy/EnemyPlaneBullet.cpp
    Classes/Game3/enemy/FinisherMissiles.cpp
    Classes/Game3/enemy/MissileForEnemyPlane.cpp
    Classes/Game3/Items/HealthRecoveryItemGame3.cpp
    Classes/Game3/Items/IncreaseBulletCountItemGame3.cpp
    Classes/Game3/Items/ItemBaseGame3.cpp
    Classes/Game3/Items/ItemPoolGane3.cpp
    Classes/Game3/Items/UpgradeBulletItemGame3.cpp
    Classes/Game3/Player/CityCollisionArea.cpp
    Classes/Game3/Player/PlayerGame3.cpp
    Classes/Library/ItemLibrary.cpp
    Classes/Library/ItemLibraryWindow.cpp
    Classes/Library/UILibrary.cpp
    Classes/LoadingBar/CustomLoadingBar.cpp
    Classes/Manager/BackgroundManager.cpp
    Classes/Manager/ObjectPoolGame1.cpp
    Classes/Manager/ObjectPoolGame2.cpp
    Classes/Manager/ObjectPoolGame3.cpp
    Classes/Manager/PhysicsManager.cpp
    Classes/Manager/PlayerMovementManager.cpp
    Classes/Manager/PositionManager.cpp
    Classes/Manager/SpriteBatchNodeManager.cpp
    Classes/Manager/SpriteFrameCacheManager.cpp
    Classes/Manager/TutorialLayer.cpp
    Classes/Panel/BasePanel.cpp
    Classes/Panel/GameOverPanel.cpp
    Classes/Panel/PausePanel.cpp
    Classes/Panel/VictoryPanel.cpp
    Classes/PlayerAttributes/PlayerAttributes.cpp
    Classes/Scene/BaseScene.cpp
    Classes/Scene/LoadingScene.cpp
    Classes/Scene/MainMenuScene.cpp
    Classes/Scene/SceneFinishGame.cpp
    Classes/UIGame/Badge.cpp
    Classes/utils/MathFunction.cpp
    Classes/utils/PhysicsShapeCache.cpp
     )
list(APPEND GAME_HEADER
    Classes/AppDelegate.h
    Classes/HelloWorldScene.h
    Classes/Bullet/BulletPlayerGame3.h
    Classes/Button/BaseButton.h
    Classes/Button/ExitButton.h
    Classes/Button/ReplayButton.h
    Classes/Constants/Constants.h
    Classes/Controller/GameController.h
    Classes/Controller/SceneController.h
    Classes/Controller/SoundController.h
    Classes/Controller/SpriteController.h
    Classes/FX/Explodable.h
    Classes/Game1/Game1Scene.h
    Classes/Game1/Background/Background.h
    Classes/Game1/Effect/EffectObject.h
    Classes/Game1/Effect/EffectObjectPool.h
    Classes/Game1/Enemy/FallingObject.h
    Classes/Game1/Enemy/FallingRock.h
    Classes/Game1/Enemy/FallingTree.h
    Classes/Game1/Enemy/FanBullet.h
    Classes/Game1/Enemy/FlyingBullet.h
    Classes/Game1/Enemy/Landmine.h
    Classes/Game1/Enemy/RandomBoom.h
    Classes/Game1/Items/AmmoItem.h
    Classes/Game1/Items/CollectibleItem.h
    Classes/Game1/Items/HealthItem.h
    Classes/Game1/Player/HealthPlayerGame1.h
    Classes/Game1/Player/PlayerGame1.h
    Classes/Game1/Skills/ShieldSkill.h
    Classes/Game2/Game2Scene.h
    Classes/Game2/Cursor/Cursor.h
    Classes/Game2/Enemy/BunkerGame2.h
    Classes/Game2/Enemy/EnemyBase.h
    Classes/Game2/Enemy/EnemyUtils.h
    Classes/Game2/Enemy/MeleeEnemyGame2.h
    Classes/Game2/Enemy/SniperBulletGame2.h
    Classes/Game2/Enemy/SniperEnemyGame2.h
    Classes/Game2/Enemy/SuicideBomberEnemy.h
    Classes/Game2/Player/BulletGame2.h
    Classes/Game2/Player/Bullet_Petard.h
    Classes/Game2/Player/Cannon.h
    Classes/Game2/Player/Grenade.h
    Classes/Game2/Player/GroundCannonGame2.h
    Classes/Game2/Player/PlayerGame2.h
    Classes/Game3/Game3Scene.h
    Classes/Game3/enemy/BoomForEnemyPlane.h
    Classes/Game3/enemy/BulletForEnemyPlane.h
    Classes/Game3/enemy/EnemyPlaneBase.h
    Classes/Game3/enemy/EnemyPlaneBoom.h
    Classes/Game3/enemy/EnemyPlaneBoss.h
    Classes/Game3/enemy/EnemyPlaneBullet.h
    Classes/Game3/enemy/FinisherMissiles.h
    Classes/Game3/enemy/MissileForEnemyPlane.h
    Classes/Game3/Items/HealthRecoveryItemGame3.h
    Classes/Game3/Items/IncreaseBulletCountItemGame3.h
    Classes/Game3/Items/ItemBaseGame3.h
    Classes/Game3/Items/ItemPoolGane3.h
    Classes/Game3/Items/UpgradeBulletItemGame3.h
    Classes/Game3/Player/CityCollisionArea.h
    Classes/Game3/Player/PlayerGame3.h
    Classes/Library/ItemLibrary.h
    Classes/Library/ItemLibraryWindow.h
    Classes/Library/UILibrary.h
    Classes/LoadingBar/CustomLoadingBar.h
    Classes/Manager/BackgroundManager.h
    Classes/Manager/ObjectPoolGame1.h
    Classes/Manager/ObjectPoolGame2.h
    Classes/Manager/ObjectPoolGame3.h
    Classes/Manager/PhysicsManager.h
    Classes/Manager/PlayerMovementManager.h
    Classes/Manager/PositionManager.h
    Classes/Manager/SpriteBatchNodeManager.h
    Classes/Manager/SpriteFrameCacheManager.h
    Classes/Manager/TutorialLayer.h
    Classes/Panel/BasePanel.h
    Classes/Panel/GameOverPanel.h
    Classes/Panel/PausePanel.h
    Classes/Panel/VictoryPanel.h
    Classes/PlayerAttributes/PlayerAttributes.h
    Classes/Scene/BaseScene.h
    Classes/Scene/LoadingScene.h
    Classes/Scene/MainMenuScene.h
    Classes/Scene/SceneFinishGame.h
    Classes/UIGame/Badge.h
    Classes/utils/MathFunction.h
    Classes/utils/PhysicsShapeCache.h
    )

if(ANDROID)
    # change APP_NAME to the share library name for Android, it's value depend on AndroidManifest.xml
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
elseif(LINUX)
    list(APPEND GAME_SOURCE
         proj.linux/main.cpp
         )
elseif(WINDOWS)
    list(APPEND GAME_HEADER
         proj.win32/main.h
         proj.win32/resource.h
         )
    list(APPEND GAME_SOURCE
         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
             proj.ios_mac/ios/AppController.h
             proj.ios_mac/ios/RootViewController.h
             )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/ios/main.m
             proj.ios_mac/ios/AppController.mm
             proj.ios_mac/ios/RootViewController.mm
             proj.ios_mac/ios/Prefix.pch
             ${APP_UI_RES}
             )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.ios_mac/mac/Icon.icns
            proj.ios_mac/mac/Info.plist
            )
        list(APPEND GAME_SOURCE
             proj.ios_mac/mac/main.cpp
             proj.ios_mac/mac/Prefix.pch
             ${APP_UI_RES}
             )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

# mark app complie info and libs info
set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )
if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive)
endif()

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES
                              MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/mac/Info.plist"
                              )
    elseif(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME} COPY_TO ${APP_RES_DIR}/..)
endif()

if(LINUX OR WINDOWS)
    cocos_copy_res(COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
