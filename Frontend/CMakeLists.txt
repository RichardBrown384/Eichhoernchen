SET(FRONTEND_SYSTEMS_SRC
    src/Factories/ArchimedesFactory.cpp
)

add_executable(frontend
    ${FRONTEND_SYSTEMS_SRC}
    src/Main.cpp
)

target_include_directories(
    frontend
    PRIVATE
    include
)

target_link_libraries(frontend
    toolkit
    archimedes
    argparse)

target_compile_options(
    frontend
    PRIVATE
    ${EMULATOR_COMPILER_FLAGS}
)

install(TARGETS frontend DESTINATION bin)
