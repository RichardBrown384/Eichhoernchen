set(TOOLKIT_SRC
    src/Toolkit.cpp
    src/Toolkit_File.cpp
    src/Toolkit_PresentationContext.cpp
    src/Toolkit_ZipFile.cpp
    )

add_library(
    toolkit
    STATIC
    ${TOOLKIT_SRC}
)

target_include_directories(
    toolkit
    PUBLIC
    include
)

target_link_libraries(
    toolkit
    PRIVATE
    SDL3::SDL3
    zip
)

target_compile_options(
    toolkit
    PRIVATE
    ${EMULATOR_COMPILER_FLAGS}
)
