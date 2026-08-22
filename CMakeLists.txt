cmake_minimum_required(VERSION 3.25)

project(newscraper VERSION 0.4.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

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

add_executable(newscraper
    ${SRC_FILES}
    ${HEADER_FILES}
    ${UI_FILES}
)
target_include_directories(newscraper
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/header
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(newscraper PRIVATE Qt6::Widgets Qt6::Network)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_custom_target(copy_config ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/config $<TARGET_FILE_DIR:newscraper>/config
)
add_dependencies(newscraper copy_config)

include(GNUInstallDirs)

set(APP_CONFIG_DIR "${CMAKE_INSTALL_FULL_SYSCONFDIR}/${PROJECT_NAME}")
target_compile_definitions(newscraper
    PRIVATE
    APP_CONFIG_DIR="${APP_CONFIG_DIR}"
    APP_NAME="${PROJECT_NAME}"
    APP_VERSION="${PROJECT_VERSION}"
)

install(TARGETS newscraper RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES config/config.json DESTINATION ${APP_CONFIG_DIR})
install(FILES newscraper.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
