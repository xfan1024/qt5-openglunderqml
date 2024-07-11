cmake_minimum_required(VERSION 3.5)

project(openglunderqml)

find_package(Qt5 COMPONENTS Core Qml Quick REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    squircle.cpp
    main.cpp
    openglunderqml.qrc
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Qml Qt5::Quick)
