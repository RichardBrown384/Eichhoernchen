#pragma once

#include <cstdint>
#include <array>

namespace rbrown::arm {

class Memory {
public:
    virtual ~Memory() = default;

    virtual auto Reset() -> void = 0;

    virtual auto CycleS() -> void = 0;
    virtual auto CycleN() -> void = 0;
    virtual auto CycleI(uint32_t) -> void = 0;

    virtual auto ReadWord(uint32_t, uint32_t&) -> bool = 0;
    virtual auto ReadByte(uint32_t, uint32_t&) -> bool = 0;
    virtual auto WriteWord(uint32_t, uint32_t) -> bool = 0;
    virtual auto WriteByte(uint32_t, uint32_t) -> bool = 0;

    [[nodiscard]] virtual auto GetSupervisor() const -> bool = 0;
    virtual auto SetSupervisor(bool) -> void = 0;
};

class Interrupts {
public:
    virtual ~Interrupts() = default;
    virtual auto Reset() -> void = 0;
    virtual auto PendingIrq() -> bool = 0;
    virtual auto PendingFiq() -> bool = 0;
};

class Arm {
public:
    Arm(Memory&, Interrupts&);

    auto Reset() -> void;
    auto Execute() -> void;

    auto SetRegister(uint32_t, uint32_t, uint32_t) -> void;
    auto SetPC(uint32_t) -> void;
    auto SetMode(uint32_t) -> void;
    auto SetN(bool) -> void;
    auto SetZ(bool) -> void;
    auto SetC(bool) -> void;
    auto SetV(bool) -> void;
    auto SetI(bool) -> void;
    auto SetF(bool) -> void;

    auto SetDecodedInstruction(uint32_t) -> void;
    auto SetFetchedInstruction(uint32_t) -> void;

    [[nodiscard]] auto GetRegister(uint32_t, uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetPC() const -> uint32_t;
    [[nodiscard]] auto GetMode() const -> uint32_t;

    [[nodiscard]] auto GetN() const -> bool;
    [[nodiscard]] auto GetZ() const -> bool;
    [[nodiscard]] auto GetC() const -> bool;
    [[nodiscard]] auto GetV() const -> bool;
    [[nodiscard]] auto GetI() const -> bool;
    [[nodiscard]] auto GetF() const -> bool;

    [[nodiscard]] auto GetDecodedInstruction() const -> uint32_t;

private:
    struct PipelineEntry {
        uint32_t instruction;
        bool prefetchAbort;
    };
    struct DataProcessingOperands {
        uint32_t op1;
        uint32_t op2;
        uint32_t carry;
    };
    struct MultiplyAccumulateOperands {
        uint32_t op1;
        uint32_t op2;
        uint32_t accumulator;
    };

    static auto TransferAddressIsInvalid(uint32_t) -> bool;
    static auto IsPC(uint32_t) -> bool;

    auto GetMemory() -> Memory&;
    auto GetInterrupts() -> Interrupts&;

    auto GetPipelineDecode() -> PipelineEntry&;

    [[nodiscard]] auto GetRegisterWithoutPSR(uint32_t, uint32_t) const -> uint32_t;
    [[nodiscard]] auto GetRegisterWithPSR(uint32_t, uint32_t) const -> uint32_t;

    auto GetTransPin() -> bool;
    auto SetTransPin(bool) -> void;

    auto CycleS() -> void;
    auto CycleN() -> void;
    auto CycleI() -> void;
    auto CycleI(uint32_t n) -> void;

    auto IncrementPC() -> void;

    auto DeconstructPSR(uint32_t) -> void;
    [[nodiscard]] auto ReconstructPSR() const -> uint32_t;

    [[nodiscard]] auto TestCondition(uint32_t instruction) const -> bool;

    auto ReadByte(uint32_t, uint32_t&) -> bool;
    auto ReadWord(uint32_t , uint32_t&) -> bool;
    auto ReadWordUnaligned(uint32_t, uint32_t&) -> bool;

    auto WriteWord(uint32_t, uint32_t) -> bool;
    auto WriteByte(uint32_t, uint32_t) -> bool;

    auto PrefetchInstruction() -> void;

    auto Branch(uint32_t) -> void;

    [[nodiscard]] auto GetBranchLinkValue() const -> uint32_t;
    [[nodiscard]] auto GetBranchTarget(uint32_t) const -> uint32_t;

    auto Execute(uint32_t) -> void;

    auto ExceptionReset() -> void;
    auto ExceptionDisableIrq(uint32_t) -> void;
    auto ExceptionUndefinedInstruction() -> void;
    auto ExceptionSoftwareInterrupt() -> void;
    auto ExceptionPrefetchAbort() -> void;
    auto ExceptionDataAbort() -> void;
    auto ExceptionAddressException() -> void;
    auto ExceptionInterrupt() -> void;
    auto ExceptionFastInterrupt() -> void;

    auto ExecuteDataProcessingRegisterShift(uint32_t) -> void;
    auto ReadOperandsMultiplyAccumulate(uint32_t) -> MultiplyAccumulateOperands;
    auto ReadOperandsRegisterShift(uint32_t) -> DataProcessingOperands;
    auto ReadOperand2RegisterShift(uint32_t, uint32_t, uint32_t, uint32_t&, uint32_t&) const -> void;

    auto ExecuteDataProcessingInstructionShift(uint32_t) -> void;
    auto ReadOperandsInstructionShift(uint32_t) -> DataProcessingOperands;
    auto ReadOperand2InstructionShift(uint32_t, uint32_t, uint32_t, uint32_t&, uint32_t&) const -> void;

    auto ExecuteDataProcessingImmediate(uint32_t) -> void;
    auto ReadOperandsImmediate(uint32_t) -> DataProcessingOperands;
    auto ReadOperand2Immediate(uint32_t, uint32_t, uint32_t&, uint32_t&) const -> void;

    auto ApplyArithmeticFlags(uint32_t, uint32_t, uint32_t) -> void;
    auto ApplyLogicalFlags(uint32_t, uint32_t) -> void;
    auto ApplyMultiplyAccumulateFlags(uint32_t) -> void;

    auto ApplyArithmeticOperation(uint32_t, uint32_t, uint32_t, uint32_t) -> void;
    auto ApplyArithmeticCompareOperation(uint32_t, uint32_t, uint32_t, uint32_t) -> void;
    auto ApplyLogicalOperation(uint32_t, uint32_t, uint32_t) -> void;
    auto ApplyLogicalCompareOperation(uint32_t, uint32_t, uint32_t) -> void;
    auto ApplyMultiplyAccumulateOperation(uint32_t, uint32_t) -> void;

    auto And(uint32_t, const DataProcessingOperands&) -> void;
    auto Eor(uint32_t, const DataProcessingOperands&) -> void;
    auto Sub(uint32_t, const DataProcessingOperands&) -> void;
    auto Rsb(uint32_t, const DataProcessingOperands&) -> void;
    auto Add(uint32_t, const DataProcessingOperands&) -> void;
    auto Adc(uint32_t, const DataProcessingOperands&) -> void;
    auto Sbc(uint32_t, const DataProcessingOperands&) -> void;
    auto Rsc(uint32_t, const DataProcessingOperands&) -> void;
    auto Tst(uint32_t, const DataProcessingOperands&) -> void;
    auto Teq(uint32_t, const DataProcessingOperands&) -> void;
    auto Cmp(uint32_t, const DataProcessingOperands&) -> void;
    auto Cmn(uint32_t, const DataProcessingOperands&) -> void;
    auto Orr(uint32_t, const DataProcessingOperands&) -> void;
    auto Mov(uint32_t, const DataProcessingOperands&) -> void;
    auto Bic(uint32_t, const DataProcessingOperands&) -> void;
    auto Mvn(uint32_t, const DataProcessingOperands&) -> void;
    auto Mul(uint32_t, const MultiplyAccumulateOperands&) -> void;
    auto Mla(uint32_t, const MultiplyAccumulateOperands&) -> void;

    auto ExecuteSingleDataTransferImmediateLoad(uint32_t) -> void;
    auto ExecuteSingleDataTransferImmediateStore(uint32_t) -> void;

    auto ExecuteSingleDataTransferRegisterLoad(uint32_t) -> void;
    auto ExecuteSingleDataTransferRegisterStore(uint32_t) -> void;

    void ExecuteSingleDataTransferLoad(uint32_t, uint32_t, uint32_t);
    void ExecuteSingleDataTransferStore(uint32_t, uint32_t, uint32_t);

    auto ExecuteBlockDataTransferLoad(uint32_t) -> void;
    auto ExecuteBlockDataTransferStore(uint32_t) -> void;

    auto ExecuteBranch(uint32_t) -> void;

    auto ExecuteCoprocessorDataOperation(uint32_t) -> void;
    auto ExecuteCoprocessorDataTransfer(uint32_t) -> void;
    auto ExecuteCoprocessorRegisterTransfer(uint32_t) -> void;

private:
    Memory& memory;
    Interrupts& interrupts;

    PipelineEntry decode;
    PipelineEntry fetch;

    std::array<uint32_t, 27> regs;

    uint32_t pc;
    uint32_t psrMode;
    bool psrN;
    bool psrZ;
    bool psrC;
    bool psrV;
    bool psrI;
    bool psrF;
};

}