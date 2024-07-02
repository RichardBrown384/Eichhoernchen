set(ARM_INCLUDES
    include/Assembler.h
    include/Disassembler.h
    include/Arm.h
)

set(ARM_ASSEMBLER_SOURCE
    src/Assembler/Assembler.cpp
    src/Assembler/Assembler_BlockDataTransfer.cpp
    src/Assembler/Assembler_Branch.cpp
    src/Assembler/Assembler_ConditionCode.cpp
    src/Assembler/Assembler_Coprocessor.cpp
    src/Assembler/Assembler_CoprocessorDataOperations.cpp
    src/Assembler/Assembler_CoprocessorDataTransfer.cpp
    src/Assembler/Assembler_CoprocessorRegisterTransfers.cpp
    src/Assembler/Assembler_DataProcessing.cpp
    src/Assembler/Assembler_Impl.cpp
    src/Assembler/Assembler_Multiply.cpp
    src/Assembler/Assembler_Number.cpp
    src/Assembler/Assembler_Operand2.cpp
    src/Assembler/Assembler_SingleDataTransfer.cpp
    src/Assembler/Assembler_SoftwareInterrupt.cpp
    src/Assembler/Assembler_Swap.cpp
    src/Assembler/SourceLine.cpp
)

set(ARM_DISASSEMBLER_SOURCE
    src/Disassembler/Disassembler.cpp
)

set(ARM_CPU_SOURCE
    src/Arm/Arm.cpp
    src/Arm/Arm_BlockDataTransferStore.cpp
    src/Arm/Arm_BlockDataTransferLoad.cpp
    src/Arm/Arm_Branch.cpp
    src/Arm/Arm_CoprocessorDataOperation.cpp
    src/Arm/Arm_CoprocessorDataTransfer.cpp
    src/Arm/Arm_CoprocessorRegisterTransfer.cpp
    src/Arm/Arm_DataProcessing.cpp
    src/Arm/Arm_DataProcessingImmediate.cpp
    src/Arm/Arm_DataProcessingInstructionShift.cpp
    src/Arm/Arm_DataProcessingRegisterShift.cpp
    src/Arm/Arm_SingleDataTransferImmediateLoad.cpp
    src/Arm/Arm_SingleDataTransferImmediateStore.cpp
    src/Arm/Arm_SingleDataTransferRegisterLoad.cpp
    src/Arm/Arm_SingleDataTransferRegisterStore.cpp
)

set(ARM_SOURCE
    ${ARM_ASSEMBLER_SOURCE}
    ${ARM_DISASSEMBLER_SOURCE}
    ${ARM_CPU_SOURCE}
    )

add_library(Arm STATIC ${ARM_SOURCE})

target_include_directories(
    Arm
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_compile_options(
    Arm
    PRIVATE
    ${EMULATOR_COMPILER_FLAGS}
)

target_link_libraries(
    Arm
    PRIVATE
    fmt::fmt
)

install(TARGETS Arm EXPORT ArmConfig DESTINATION lib)
install(FILES ${ARM_INCLUDES} DESTINATION include/Arm)

export(TARGETS Arm
    NAMESPACE Arm::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/ArmConfig.cmake"
)

install(EXPORT ArmConfig
    NAMESPACE Arm::
    DESTINATION lib/cmake/Arm
)