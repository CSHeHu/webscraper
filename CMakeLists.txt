cmake_minimum_required(VERSION 3.25)

project(webscraper)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets WebEngineWidgets Network REQUIRED)

set(SRC_FILES
    src/dataManager.cpp
    src/main.cpp
    src/mainwindow.cpp
)
set(HEADER_FILES
    header/dataManager.h
    header/mainwindow.h
)
set(UI_FILES
)

add_executable(webscraper
    ${SRC_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
)
target_include_directories(webscraper
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/header
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(webscraper PRIVATE Qt6::Widgets Qt6::WebEngineWidgets Qt6::Network)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
