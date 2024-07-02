set(ARCHIMEDES_SRC
    src/Archimedes.cpp
    src/ArchimedesDiscImage.cpp
    src/ArchimedesIoc.cpp
    src/ArchimedesKeyboard.cpp
    src/ArchimedesKeyboardConfiguration.cpp
    src/ArchimedesMemc.cpp
    src/ArchimedesVidc.cpp
    src/ArchimedesVidc_Fifo.cpp
    src/I2CController.cpp
    src/RTC8583.cpp
    src/WDC1772.cpp
)

add_library(archimedes
    STATIC
    ${ARCHIMEDES_SRC}
)

target_link_libraries(archimedes common toolkit Arm spdlog)

target_include_directories(archimedes
    PUBLIC
    include
)

target_compile_options(
    archimedes
    PRIVATE
    ${EMULATOR_COMPILER_FLAGS}
)