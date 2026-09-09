cmake_minimum_required(VERSION 3.16)
project(QtLabs VERSION 1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    ssstl
    GIT_REPOSITORY https://github.com/Danik2395/ssstl.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(ssstl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent)

set(SOURCES
    main.cpp
    MainWindow.h
    labs/LabTestWidget.h
    labs/Lab1Widget.h
    labs/classes/Call.h
    labs/classes/Call.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Concurrent
    ssstl
)
