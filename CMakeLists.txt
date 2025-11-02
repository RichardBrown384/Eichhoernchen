cmake_minimum_required(VERSION 3.31.9)

project(Eichhoernchen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

#set(FETCHCONTENT_FULLY_DISCONNECTED true)

# For whatever reason all the libs default to shared which on macOS is fine
# however under windows this causes problems when trying to link and then run
# the tests. For now force windows builds to use static linking.
# The results aren't entirely satisfactory since libzip ends up using
# zlib dynamically but for the sake of running tests in CI this is sufficient.
if(WIN32)
    # Force shared libs on Windows CI to avoid linker issues
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static libs on Windows" FORCE)
endif()

# Google Test
FetchContent_Declare(googletest
  GIT_REPOSITORY     https://github.com/google/googletest.git
  GIT_TAG            v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# SDL 3
FetchContent_Declare(sdl3
  GIT_REPOSITORY     https://github.com/libsdl-org/SDL.git
  GIT_TAG            release-3.2.24
)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

# ZLib
FetchContent_Declare(zlib
  GIT_REPOSITORY     https://github.com/madler/zlib.git
  GIT_TAG            v1.3.1
  OVERRIDE_FIND_PACKAGE
)
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(zlib)
if(NOT TARGET ZLIB::ZLIB)
    add_library(ZLIB::ZLIB ALIAS zlib)
endif()

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
set(ENABLE_BZIP2 OFF CACHE BOOL "" FORCE)
set(ENABLE_LZMA OFF CACHE BOOL "" FORCE)
set(ENABLE_ZSTD OFF CACHE BOOL "" FORCE)
set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_REGRESS OFF CACHE BOOL "" FORCE)
set(BUILD_OSSFUZZ OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_DOC OFF CACHE BOOL "" FORCE)
set(LIBZIP_DO_INSTALL OFF CACHE BOOL "" FORCE)

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

# fmtlib
FetchContent_Declare(fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 12.0.0
)

FetchContent_MakeAvailable(libzip googletest sdl3 argparse spdlog fmtlib)

if(MSVC)
  set(EMULATOR_COMPILER_FLAGS
    /W4                    # High warning level, realistic for production
    /permissive-           # Enforce strict ISO C++ (matches clang/gcc)
    /Zc:__cplusplus        # Set __cplusplus correctly (needed for some libs)
    /Zc:inline             # Better inline semantics
    /Zc:preprocessor       # Modern preprocessor
    /Zc:throwingNew        # Enforce throwing new semantics
    /EHsc                  # Standard C++ exception model
    /FC                    # Full path in diagnostics
    /external:anglebrackets
    /external:W0           # Silence warnings from system headers
    /WX                    # Treat warnings as errors (like -Werror)

    # Disable a few common, harmless ones:
    /wd4100    # Unreferenced formal parameter
    /wd4324    # Structure was padded due to alignment specifier
    /wd4505    # Unreferenced local function has been removed

    # Disable cases that occur because we haven't tried to compile this on windows for 10 years
    /wd4146    # Disable "unary minus on unsigned" warning
    /wd4244    # Truncation uint32_t to uint8_t (which is okay but we should fix it)
    /wd4267    # Truncation
    /wd4389    # Signed/unsigned
    /wd4805    # unsafe mix of bool/unsigned
  )
else()
  set(EMULATOR_COMPILER_FLAGS
    -Wall -Wextra -pedantic -Werror
    -Wno-unused-parameter -Wno-unused-function
    #-Wconversion -Wsign-conversion
  )
endif()

enable_testing()

add_subdirectory(Common)
add_subdirectory(Frontend)
add_subdirectory(Processors)
add_subdirectory(Systems)
add_subdirectory(Tests)
add_subdirectory(Toolkit)
