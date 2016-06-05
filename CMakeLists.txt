cmake_policy(SET CMP0017 NEW)
cmake_minimum_required(VERSION 2.8)
set(APP_NAME ball)
project (${APP_NAME})
#set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/cocos2d)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${COCOS2D_ROOT}/cmake/Modules/")
# include(CocosBuildHelpers)
# libcocos2d
set(BUILD_CPP_TESTS OFF CACHE BOOL "turn off build cpp-tests")

# add_subdirectory(${COCOS2D_ROOT})
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(PLATFORM_SPECIFIC_SRC)
set(PLATFORM_SPECIFIC_HEADERS)
set(PLATFORM_SPECIFIC_SRC
        #proj.ios_mac/ios/main.m
        #proj.ios_mac/ios/RootViewController.mm
        #proj.ios_mac/ios/AppController.mm
        proj.ios_mac/mac/main.cpp
        )
set(PLATFORM_SPECIFIC_HEADERS
        #proj.ios_mac/ios/RootViewController.h
        #proj.ios_mac/ios/AppController.h
        proj.ios_mac/mac/prefix.pch
        )
set(PLATFORM_SPECIFIC_HEADERS
        proj.win32/main.h
        proj.win32/resource.h
        )
include_directories(GLOB_RECURSE
        /usr/local/include/GLFW
        /usr/include/GLFW
        /Applications/Cocos/Cocos2d-x/cocos2d-x-3.10/cocos/platform/mac
        /Applications/Cocos/Cocos2d-x/cocos2d-x-3.10/external/glfw3/include/mac
        Classes
        /Applications/Cocos/Cocos2d-x/cocos2d-x-3.10/cocos/
)
set(GAME_SRC
        Classes/AppDelegate.cpp
        Classes/HelloWorldScene.cpp
        ${PLATFORM_SPECIFIC_SRC}
        )
set(GAME_HEADERS
        Classes/AppDelegate.h
        Classes/HelloWorldScene.h
        ${PLATFORM_SPECIFIC_HEADERS}
        )
if(GAME_HEADERS)
    if ( WIN32 )
        add_executable(${APP_NAME} WIN32 ${GAME_SRC} ${GAME_HEADERS})
    else()
        add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})
    endif ( WIN32 )
else()
    if ( WIN32 )
        add_executable(${APP_NAME} WIN32 ${GAME_SRC})
    else()
        add_executable(${APP_NAME} ${GAME_SRC})
    endif ( WIN32 )
endif()
target_link_libraries(${APP_NAME} /Applications/Cocos/Cocos2d-x/cocos2d-x-3.10/prebuilt/mac)
set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set_target_properties(${APP_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")
#pre_build(${APP_NAME}
#        COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
#        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
#        )
