cmake_minimum_required(VERSION 3.25)

# Project name and C++ standard
project(webscraper)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Enable Qt's Meta-Object Compiler (MOC) and other tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Path to your Qt installation
set(CMAKE_PREFIX_PATH "~/Qt/6.8.1/gcc_64")


# Find packages package
find_package(CURL REQUIRED)  
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)

# Collect source and header files
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/header/*.h")
file(GLOB_RECURSE UI_FILES "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.ui")

# Define the executable target
add_executable(webscraper
    ${SRC_FILES}
    ${HEADER_FILES}
    ${UI_FILES} # Include .ui files
)
# Add include directories for headers and generated files
target_include_directories(webscraper 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/header    # Include project headers
    ${CMAKE_BINARY_DIR}                   # Include generated files like ui_mainwindow.h
    ${CURL_INCLUDE_DIRS}                  # Include curl headers
)

# Link libraries 
target_link_libraries(webscraper PRIVATE CURL::libcurl)
target_link_libraries(webscraper PRIVATE Qt6::Widgets Qt6::WebEngineWidgets)

# Set the output directory for the compiled binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
