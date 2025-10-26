cmake_minimum_required(VERSION 3.31.9)

project(Eichhoernchen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

#set(FETCHCONTENT_FULLY_DISCONNECTED true)

# Google Test
FetchContent_Declare(googletest
  GIT_REPOSITORY     https://github.com/google/googletest.git
  GIT_TAG            v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# SDL 3
# Note: I was using prerelease tags to track SDL3 but those tags got deleted.
# Using a fixed commit hash for now so that we don't have to keep updating while
# SDL, I presume, prepare for an initial 3.0 release.
FetchContent_Declare(sdl3
  GIT_REPOSITORY     https://github.com/libsdl-org/SDL.git
  GIT_TAG            9651ca59187c16079846918483c40d6b5c2f454c # HEAD on 09.06.2024
)

# ZLib
FetchContent_Declare(zlib
  GIT_REPOSITORY     https://github.com/madler/zlib.git
  GIT_TAG            v1.3.1
)

# Libzip
FetchContent_Declare(libzip
  GIT_REPOSITORY     https://github.com/nih-at/libzip.git
  GIT_TAG            v1.11.4
)
set(ENABLE_COMMONCRYPTO OFF CACHE BOOL "" FORCE)
set(ENABLE_GNUTLS OFF CACHE BOOL "" FORCE)
set(ENABLE_MBEDTLS OFF CACHE BOOL "" FORCE)
set(ENABLE_OPENSSL OFF CACHE BOOL "" FORCE)
set(ENABLE_WINDOWS_CRYPTO OFF CACHE BOOL "" FORCE)
set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_REGRESS OFF CACHE BOOL "" FORCE)
set(BUILD_OSSFUZZ OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_DOC OFF CACHE BOOL "" FORCE)

# Agrparse
FetchContent_Declare(argparse
  GIT_REPOSITORY   https://github.com/p-ranav/argparse
  GIT_TAG          v3.2
)

# spdlog
FetchContent_Declare(spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG          v1.16.0
)

#fmtlib
FetchContent_Declare(fmtlib
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 12.0.0
    )

FetchContent_MakeAvailable(zlib libzip googletest sdl3 argparse spdlog fmtlib)

if(MSVC)
    set(EMULATOR_COMPILER_FLAGS
        /Wall
        /wd4100 /wd4514 /wd4710 /wd4820
        /experimental:external /external:W0
    )
else()
    set(EMULATOR_COMPILER_FLAGS
        -Wall -Wextra -pedantic -Werror
        -Wno-unused-parameter
    )
endif()

add_subdirectory(Common)
add_subdirectory(Frontend)
add_subdirectory(Processors)
add_subdirectory(Systems)
add_subdirectory(Tests)
add_subdirectory(Toolkit)
