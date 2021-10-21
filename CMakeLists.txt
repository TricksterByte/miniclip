cmake_minimum_required(VERSION 3.18)

project(puyopuyo
    LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SDL2_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2")
set(SDL2_IMAGE_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_image")
set(SDL2_MIXER_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_mixer")
set(SDL2_TTF_DIR "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_ttf")

if ("${CMAKE_CXX_STANDARD}" STREQUAL "")
    message("No explicit C++ version specified, assuming C++17.")
    set(CMAKE_CXX_STANDARD 17)
else ()
    message("Using ${CMAKE_CXX_STANDARD}.")
endif()

set(CXX_STANDARD_REQUIRED REQUIRED)

set(PUYO_LIB_TARGET libpuyo)
set(PUYO_EXE_TARGET exepuyo)

include(Utilities)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
find_package(SDL2_MIXER REQUIRED)
find_package(SDL2_TTF REQUIRED)

add_library(${PUYO_LIB_TARGET} INTERFACE)

target_include_directories(${PUYO_LIB_TARGET}
    PUBLIC INTERFACE
    include/

    SYSTEM PUBLIC INTERFACE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS})

target_sources(${PUYO_LIB_TARGET}
    PUBLIC INTERFACE
    "src/common/log.cpp"
    
    "src/game/core/game.cpp"
    "src/game/core/graphics.cpp")

target_link_libraries(${PUYO_LIB_TARGET}
    PUBLIC INTERFACE
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES})

add_executable(${PUYO_EXE_TARGET}
    main/main.cpp "include/puyo/game/components/gameplay/score.hpp" "include/puyo/game/systems/input/handle_general_input.hpp" "include/puyo/game/systems/gameplay/check_lose.hpp" "include/puyo/game/systems/general/reset_entities.hpp")

target_link_libraries(${PUYO_EXE_TARGET}
    PUBLIC ${PUYO_LIB_TARGET})

# FIX THIS
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2/lib/x64/SDL2.dll" "${PROJECT_BINARY_DIR}/Release/SDL2.dll")
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_image/lib/x64/SDL2_image.dll" "${PROJECT_BINARY_DIR}/Release/SDL2_image.dll")
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_image/lib/x64/libjpeg-9.dll" "${PROJECT_BINARY_DIR}/Release/libjpeg-9.dll")
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_image/lib/x64/libpng16-16.dll" "${PROJECT_BINARY_DIR}/Release/libjpeg-9.dll")
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_image/lib/x64/zlib1.dll" "${PROJECT_BINARY_DIR}/Release/zlib1.dll")
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_mixer/lib/x64/SDL2_mixer.dll" "${PROJECT_BINARY_DIR}/Release/SDL2_mixer.dll")
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_ttf/lib/x64/SDL2_ttf.dll" "${PROJECT_BINARY_DIR}/Release/SDL2_ttf.dll")
COPY_FILE_POST_BUILD(${PUYO_EXE_TARGET} "${CMAKE_CURRENT_LIST_DIR}/3rdparty/SDL2_ttf/lib/x64/libfreetype-6.dll" "${PROJECT_BINARY_DIR}/Release/libfreetype-6.dll")
