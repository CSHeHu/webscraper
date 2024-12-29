cmake_minimum_required(VERSION 3.25)

# Project name and C++ standard
project(webscraper)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find libcurl package
find_package(CURL REQUIRED)  

# Collect all source files in the 'src' directory
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Define the executable target
add_executable(webscraper ${SRC_FILES})

# Add include directories for headers
target_include_directories(webscraper 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/header    # Include project headers
    ${CURL_INCLUDE_DIRS}                  # Include curl headers
)

# Link the curl library
target_link_libraries(webscraper PRIVATE CURL::libcurl)

# Set the output directory for the compiled binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
