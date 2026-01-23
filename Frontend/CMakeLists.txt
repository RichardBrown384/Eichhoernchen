add_executable(frontend
    src/CommandLine/ArgumentParser.cpp
    src/Host/FrontendHost.cpp
    src/Host/FrontendInput.cpp
    src/FileSystem.cpp
    src/Main.cpp
    src/Toolkit.cpp
)

target_include_directories(
    frontend
    PRIVATE
    include
)

target_link_libraries(frontend
    PRIVATE
# 3rd party
    argparse
    SDL3::SDL3
    zip
# Internal
    common
    archimedes
)

target_compile_options(
    frontend
    PRIVATE
    ${EMULATOR_COMPILER_FLAGS}
)

install(TARGETS frontend DESTINATION bin)
