cmake_minimum_required(VERSION 3.24...3.30)
project(ICB_game VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# Platform detection
if(ANDROID)
    set(PLATFORM_ANDROID TRUE)
    add_definitions(-DPLATFORM_ANDROID -D__ANDROID_API__=29)
    
    # Android specific settings
    set(ANDROID_STL c++_shared)
    set(CMAKE_ANDROID_STL_TYPE c++_shared)
    
    # Assicurati che le librerie standard siano disponibili
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions -frtti")
    
elseif(WIN32)
    set(PLATFORM_DESKTOP TRUE)
elseif(APPLE)
    set(PLATFORM_DESKTOP TRUE)
elseif(UNIX)
    set(PLATFORM_DESKTOP TRUE)
endif()

# Dependencies
set(RAYLIB_VERSION 5.5)

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS
)

FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
)

FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
)

FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG develop
)

FetchContent_MakeAvailable(raylib raylib_cpp entt sol2)

# ============================================================================
# ENGINE LIBRARY
# ============================================================================

# Collect engine source files (now in src/)
file(GLOB_RECURSE ENGINE_SOURCES
    "src/engine/*.cpp"
    "src/engine/*.c"
)

file(GLOB_RECURSE ENGINE_HEADERS
    "src/engine/*.hpp"
    "src/engine/*.h"
)

# Create engine library
add_library(ICB_engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(ICB_engine 
    PUBLIC
        include/
        include/entt
        include/enet
        src/
        src/engine
        src/game
        src/screens
        src/utils
)

target_link_libraries(ICB_engine 
    PUBLIC
        raylib 
        raylib_cpp 
        sol2
)

# Platform-specific linking for engine
if(APPLE)
    target_link_libraries(ICB_engine PUBLIC
        "-framework IOKit"
        "-framework Cocoa" 
        "-framework OpenGL"
    )
endif()

if(ANDROID)
    target_link_libraries(ICB_engine PUBLIC
        log
        android
        EGL
        GLESv2
        OpenSLES
        atomic
        m
        dl
    )
    
    # Assicurati che le librerie C++ standard siano linkate
    target_link_libraries(ICB_engine PUBLIC
        c++_shared
    )
endif()

# ============================================================================
# GAME EXECUTABLE (Desktop/Mobile)
# ============================================================================

if(NOT ANDROID)
    # Game main source file
    set(GAME_SOURCES "src/main.cpp")

    # Collect additional game-specific files
    file(GLOB_RECURSE GAME_SCENES
        "src/scenes/*.hpp"
        "src/scenes/*.cpp"
    )

    # Create game executable
    add_executable(ICB_game ${GAME_SOURCES} ${GAME_SCENES})

    target_include_directories(ICB_game 
        PUBLIC
            src/game/*cpp
    )

    target_link_libraries(ICB_game 
        PRIVATE
            ICB_engine
    )

    # Set output directory
    set_target_properties(ICB_game PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/game
        VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:ICB_game>
    )

    # Copy resources to build directory
    add_custom_command(TARGET ICB_game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/resources
        $<TARGET_FILE_DIR:ICB_game>/resources
        COMMENT "Copying game resources"
    )

endif()

# ============================================================================
# ANDROID LIBRARY TARGET
# ============================================================================

if(ANDROID)
    # Collect all source files for Android (everything in src/)
    file(GLOB_RECURSE ANDROID_SOURCES
        "src/*.cpp"
        "src/*.c"
    )

    # Create Android shared library
    add_library(main SHARED ${ANDROID_SOURCES})

    target_include_directories(main 
        PRIVATE
            src/engine
            src/game
            src/screens
    )

    target_link_libraries(main 
        PRIVATE
            raylib
            raylib_cpp
            sol2
            log
            android
            EGL
            GLESv2
            OpenSLES
            atomic
            m
            dl
            c++_shared  # Importante per le librerie standard C++
    )

    # Android specific flags
    target_compile_definitions(main PRIVATE
        PLATFORM_ANDROID
        __ANDROID_API__=29
        _GNU_SOURCE
        _POSIX_C_SOURCE=200809L
    )

    target_compile_options(main PRIVATE
        -ffunction-sections
        -funwind-tables
        -fstack-protector-strong
        -fPIC
        -Wall
        -Wformat
        -Werror=format-security
        -no-canonical-prefixes
        -fexceptions
        -frtti
    )

    # Android Native Activity
    find_library(android-lib android)
    target_link_libraries(main PRIVATE ${android-lib})
    
    # Add native app glue
    add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
    )
    
    target_include_directories(native_app_glue PUBLIC
        ${ANDROID_NDK}/sources/android/native_app_glue
    )
    
    target_link_libraries(main PRIVATE native_app_glue)

endif()

# ============================================================================
# CUSTOM TARGETS AND COMMANDS
# ============================================================================

# Custom target for Android build usando CMake
if(NOT ANDROID)
    add_custom_target(android_build_cmake
        COMMAND ${CMAKE_SOURCE_DIR}/build_android_cmake.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Building Android APK using CMake"
        VERBATIM
    )
    
    # Mantieni anche il vecchio target per compatibilità
    add_custom_target(android_build
        COMMAND ${CMAKE_SOURCE_DIR}/build_android.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Building Android APK using build script"
        VERBATIM
    )
endif()

# Custom target for cleaning Android build
add_custom_target(android_clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build/android
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/game.apk
    COMMENT "Cleaning Android build files"
)

# Print configuration info
message(STATUS "ICB Game Engine Configuration:")
message(STATUS "  Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
if(ANDROID)
    message(STATUS "  Android ABI: ${ANDROID_ABI}")
    message(STATUS "  Android API Level: ${ANDROID_PLATFORM}")
    message(STATUS "  Android STL: ${CMAKE_ANDROID_STL_TYPE}")
endif()

# ============================================================================
# INSTALLATION (Optional)
# ============================================================================

if(NOT ANDROID)
    install(TARGETS ICB_game
        RUNTIME DESTINATION bin
    )
    
    install(DIRECTORY src/resources/
        DESTINATION bin/resources
    )
endif()