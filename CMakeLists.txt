cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME application) 

project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(Qt5_DIR "C:\\Qt\\5.15.2\\msvc2019_64\\lib\\cmake\\Qt5")

set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}\\opencv\\build")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED)

include_directories(ui-desktop/include)

add_executable(${PROJECT_NAME}
    ui-desktop/src/main.cpp
    ui-desktop/src/mainwindow.cpp 
    ui-desktop/src/mainwindow.ui 
    ui-desktop/include/mainwindow.h
    # src/resources.qrc
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OpenCV_LIBS})

