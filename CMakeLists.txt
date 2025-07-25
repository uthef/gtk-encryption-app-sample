cmake_minimum_required(VERSION 3.16)
project(gtk-encryption-app-sample LANGUAGES C)

find_package(PkgConfig REQUIRED)
find_package(OpenSSL REQUIRED)

pkg_check_modules(GTK4 REQUIRED gtk4)

set(SOURCE_LIST 
    main.c
    utils/gtk_helpers.c
    utils/hexparse.c
    main_window/mw_context.c
    main_window/mw_actions.c
    main_window/input_transformer.c
)

if (WIN32 AND NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_LIST})
else()
    add_executable(${PROJECT_NAME} ${SOURCE_LIST})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${GTK4_INCLUDE_DIRS}
)

if(DEFINED CUSTOM_LIB_PATH)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        -L${CUSTOM_LIB_PATH}
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GTK4_LIBRARIES}
    OpenSSL::Crypto
)
