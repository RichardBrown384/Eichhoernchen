set(IOC_TEST_SOURCE
    src/Ioc/BankTest.cpp
    src/Ioc/InternalRegisterByteTest.cpp
    src/Ioc/InternalRegisterWordTest.cpp
    src/Ioc/InterruptEvents.cpp
    src/Ioc/IocTest.cpp
    )

set(MEMC_TEST_SOURCE
    src/Memc/MemcTest.cpp
    src/Memc/AccessTiming.cpp
    src/Memc/AddressTranslator.cpp
    src/Memc/ControlRegister.cpp
    src/Memc/Dma.cpp
    src/Memc/DmaAddressGenerator.cpp
    src/Memc/FlyBack.cpp
    src/Memc/InputOutputController.cpp
    src/Memc/HighRom.cpp
    src/Memc/LogicalMemoryFourMegabytes.cpp
    src/Memc/LogicalMemoryHalfMegabyte.cpp
    src/Memc/LogicalMemoryOneMegabyte.cpp
    src/Memc/LogicalMemoryTwoMegabytes.cpp
    src/Memc/LowRom.cpp
    src/Memc/PhysicalMemory.cpp
    src/Memc/Reset.cpp
    src/Memc/VideoController.cpp
    )

set(WDC1772_TEST_SOURCE
    src/WDC1772/ForceInterrupts.cpp
    src/WDC1772/ReadAddresses.cpp
    src/WDC1772/ReadSectors.cpp
    src/WDC1772/Registers.cpp
    src/WDC1772/Restores.cpp
    src/WDC1772/Seeks.cpp
    src/WDC1772/StepIns.cpp
    src/WDC1772/StepOuts.cpp
    src/WDC1772/Steps.cpp
    src/WDC1772/WDC1772Test.cpp
    src/WDC1772/WriteSectors.cpp
    )

add_executable(ArchimedesTest
    ${IOC_TEST_SOURCE}
    ${MEMC_TEST_SOURCE}
    ${WDC1772_TEST_SOURCE}
    )

target_include_directories(
    ArchimedesTest
    PRIVATE
    include
)

target_compile_options(
    ArchimedesTest
    PRIVATE
    ${EMULATOR_COMPILER_FLAGS}
)

target_link_libraries(ArchimedesTest PRIVATE archimedes common gmock gtest gtest_main)

add_test(NAME ArchimedesTest COMMAND ArchimedesTest)
