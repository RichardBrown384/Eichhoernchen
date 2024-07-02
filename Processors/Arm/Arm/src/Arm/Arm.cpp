#include <cstdint>

#include "Common/Decode.h"
#include "Common/BitUtil.h"
#include "Common/Constants.h"
#include "Common/Shifts.h"

#include "Arm/Arm.h"

using namespace rbrown::arm;

constexpr auto PSR_FLAG_N = 0x80000000u;
constexpr auto PSR_FLAG_Z = 0x40000000u;
constexpr auto PSR_FLAG_C = 0x20000000u;
constexpr auto PSR_FLAG_V = 0x10000000u;
constexpr auto PSR_FLAG_I = 0x08000000u;
constexpr auto PSR_FLAG_F = 0x04000000u;

constexpr auto PSR_MASK_FLAGS = 0xFC000000u;
constexpr auto PSR_MASK_PC = 0x03FFFFFCu;
constexpr auto PSR_MASK_MODE = 0x00000003u;

constexpr auto VECTOR_RESET = 0x00u;
constexpr auto VECTOR_UNDEFINED_INSTRUCTION = 0x04u;
constexpr auto VECTOR_SOFTWARE_INTERRUPT = 0x08u;
constexpr auto VECTOR_PREFETCH_ABORT = 0x0Cu;
constexpr auto VECTOR_DATA_ABORT = 0x10u;
constexpr auto VECTOR_ADDRESS_EXCEPTION = 0x14u;
constexpr auto VECTOR_IRQ = 0x18u;
constexpr auto VECTOR_FIQ = 0x1Cu;

constexpr auto INSTRUCTION_NOOP = 0xE1A00000; // MOV R0, R0

constexpr auto MapRegisterByMode(uint32_t mode, uint32_t r) {
    if (r < 8u) {
        return r;
    } else if (r < 13u) {
        return 8u + 2u * (r - 8u) + (mode == PSR_MODE_FIQ);
    } else if (r < 15u) {
        return 18u + 4u * (r - 13u) + mode;
    }
    return 26u;
}

Arm::Arm(Memory& mem, Interrupts& interrupts) :
    memory { mem },
    interrupts { interrupts },
    decode { .instruction = INSTRUCTION_NOOP },
    fetch { .instruction = INSTRUCTION_NOOP },
    regs { 0 },
    pc { VECTOR_RESET },
    psrMode { PSR_MODE_SVC },
    psrN { false },
    psrZ { false },
    psrC { false },
    psrV { false },
    psrI { true },
    psrF { true } {}

auto Arm::SetRegister(uint32_t mode, uint32_t r, uint32_t v) -> void { regs[MapRegisterByMode(mode, r)] = v; }
auto Arm::SetPC(uint32_t v) -> void { pc = v & PSR_MASK_PC; }
auto Arm::SetMode(uint32_t m) -> void{
    psrMode = m & PSR_MASK_MODE;
    SetTransPin(psrMode != PSR_MODE_USR);
}

auto Arm::SetN(bool v) -> void { psrN = v; }
auto Arm::SetZ(bool v) -> void { psrZ = v; }
auto Arm::SetC(bool v) -> void { psrC = v; }
auto Arm::SetV(bool v) -> void { psrV = v; }
auto Arm::SetI(bool v) -> void { psrI = v; }
auto Arm::SetF(bool v) -> void { psrF = v; }

auto Arm::SetDecodedInstruction(uint32_t v) -> void { decode.instruction = v; decode.prefetchAbort = false; }
auto Arm::SetFetchedInstruction(uint32_t v) -> void { fetch.instruction = v; fetch.prefetchAbort = false; }

auto Arm::GetRegister(uint32_t mode, uint32_t r) const -> uint32_t { return regs[MapRegisterByMode(mode, r)]; }
auto Arm::GetPC() const -> uint32_t { return pc; }
auto Arm::GetMode() const -> uint32_t { return psrMode; }

auto Arm::GetRegisterWithoutPSR(uint32_t mode, uint32_t r) const -> uint32_t {
    if (IsPC(r)) {
        return this->GetPC();
    }
    return this->GetRegister(mode, r);
}

auto Arm::GetRegisterWithPSR(uint32_t mode, uint32_t r) const -> uint32_t {
    if (IsPC(r)) {
        return GetPC() + ReconstructPSR();
    }
    return GetRegister(mode, r);
}

auto Arm::GetN() const -> bool { return psrN; }
auto Arm::GetZ() const -> bool { return psrZ; }
auto Arm::GetC() const -> bool { return psrC; }
auto Arm::GetV() const -> bool { return psrV; }
auto Arm::GetI() const -> bool { return psrI; }
auto Arm::GetF() const -> bool { return psrF; }

auto Arm::GetDecodedInstruction() const -> uint32_t { return decode.instruction; }

auto Arm::TransferAddressIsInvalid(uint32_t v) -> bool { return v & PSR_MASK_FLAGS; }
auto Arm::IsPC(uint32_t v) -> bool { return v == REGISTER_R15; }

auto Arm::GetMemory() -> Memory& { return memory; }
auto Arm::GetInterrupts() -> Interrupts& { return interrupts; }

auto Arm::GetPipelineDecode() -> PipelineEntry& { return decode; }

auto Arm::GetTransPin() -> bool { return GetMemory().GetSupervisor(); }
auto Arm::SetTransPin(bool v) -> void { GetMemory().SetSupervisor(v); }

auto Arm::CycleS() -> void { return GetMemory().CycleS(); }
auto Arm::CycleN() -> void { return GetMemory().CycleN(); }
auto Arm::CycleI() -> void { return GetMemory().CycleI(1u); }
auto Arm::CycleI(uint32_t n) -> void { return GetMemory().CycleI(n); }

auto Arm::IncrementPC() -> void { SetPC(GetPC() + 4u); }

auto Arm::DeconstructPSR(uint32_t v) -> void {
    SetN(v & PSR_FLAG_N);
    SetZ(v & PSR_FLAG_Z);
    SetC(v & PSR_FLAG_C);
    SetV(v & PSR_FLAG_V);
    if (GetMode() != PSR_MODE_USR) {
        SetI(v & PSR_FLAG_I);
        SetF(v & PSR_FLAG_F);
        SetMode(v);
    }
}

auto Arm::ReconstructPSR() const -> uint32_t {
    auto psr = 0u;
    if (GetN()) psr |= PSR_FLAG_N;
    if (GetZ()) psr |= PSR_FLAG_Z;
    if (GetC()) psr |= PSR_FLAG_C;
    if (GetV()) psr |= PSR_FLAG_V;
    if (GetI()) psr |= PSR_FLAG_I;
    if (GetF()) psr |= PSR_FLAG_F;
    psr |= GetMode();
    return psr;
}

auto Arm::TestCondition(uint32_t instruction) const -> bool {
    const auto n = GetN();
    const auto z = GetZ();
    const auto c = GetC();
    const auto v = GetV();
    switch (InstructionConditionCode(instruction)) {
        case CONDITION_CODE_EQ: return z;
        case CONDITION_CODE_NE: return !z;
        case CONDITION_CODE_CS: return c;
        case CONDITION_CODE_CC: return !c;
        case CONDITION_CODE_MI: return n;
        case CONDITION_CODE_PL: return !n;
        case CONDITION_CODE_VS: return v;
        case CONDITION_CODE_VC: return !v;
        case CONDITION_CODE_HI: return (c && !z);
        case CONDITION_CODE_LS: return !(c && !z);
        case CONDITION_CODE_GE: return n == v;
        case CONDITION_CODE_LT: return n != v;
        case CONDITION_CODE_GT: return (!z && (n == v));
        case CONDITION_CODE_LE: return !(!z && (n == v));
        case CONDITION_CODE_AL: return true;
            [[unlikely]] default: return false;
    }
}

auto Arm::ReadByte(uint32_t address, uint32_t& value) -> bool {
    return GetMemory().ReadByte(address, value);
}

auto Arm::ReadWord(uint32_t address, uint32_t& value) -> bool {
    return GetMemory().ReadWord(address, value);
}

auto Arm::ReadWordUnaligned(uint32_t address, uint32_t& value) -> bool {
    if (GetMemory().ReadWord(address, value)) {
        const auto shift = LogicalShiftLeft(address & 0x3u, 3u);
        value = RotateRight(value, shift);
        return true;
    }
    return false;
}

auto Arm::WriteWord(uint32_t address, uint32_t value) -> bool {
    return GetMemory().WriteWord(address, value);
}

auto Arm::WriteByte(uint32_t address, uint32_t value) -> bool {
    return GetMemory().WriteByte(address, ReplicateByteInWord(value));
}

auto Arm::PrefetchInstruction() -> void {
    decode = fetch;
    fetch.prefetchAbort = !ReadWord(GetPC(), fetch.instruction);
}

auto Arm::Branch(uint32_t target) -> void {
    CycleN();
    SetPC(target);
    PrefetchInstruction();
    CycleS();
    SetPC(target + 4u);
    PrefetchInstruction();
    SetPC(target + 8u);
}

auto Arm::GetBranchLinkValue() const -> uint32_t {
    const auto address = (GetPC() - 4u) & PSR_MASK_PC;
    const auto psr = ReconstructPSR();
    return address + psr;
}

auto Arm::GetBranchTarget(uint32_t instruction) const -> uint32_t {
    const auto offset = InstructionBranchOffset(instruction);
    return GetPC() + LogicalShiftLeft(offset, 2);
}

auto Arm::ExceptionDisableIrq(uint32_t vector) -> void {
    SetRegister(PSR_MODE_SVC, REGISTER_R14, GetBranchLinkValue());
    SetMode(PSR_MODE_SVC);
    SetI(true);
    Branch(vector);
}

auto Arm::ExceptionReset() -> void {
    GetMemory().Reset();
    GetInterrupts().Reset();
    SetRegister(PSR_MODE_SVC, REGISTER_R14, GetBranchLinkValue());
    SetMode(PSR_MODE_SVC);
    SetI(true);
    SetF(true);
    Branch(VECTOR_RESET);
}

auto Arm::ExceptionUndefinedInstruction() -> void {
    CycleS();
    PrefetchInstruction();
    CycleI();
    ExceptionDisableIrq(VECTOR_UNDEFINED_INSTRUCTION);
}

auto Arm::ExceptionSoftwareInterrupt() -> void {
    CycleS();
    PrefetchInstruction();
    ExceptionDisableIrq(VECTOR_SOFTWARE_INTERRUPT);
}

auto Arm::ExceptionPrefetchAbort() -> void {
    ExceptionDisableIrq(VECTOR_PREFETCH_ABORT);
}

auto Arm::ExceptionDataAbort() -> void {
    ExceptionDisableIrq(VECTOR_DATA_ABORT);
}

auto Arm::ExceptionAddressException() -> void {
    ExceptionDisableIrq(VECTOR_ADDRESS_EXCEPTION);
}

auto Arm::ExceptionInterrupt() -> void {
    SetRegister(PSR_MODE_IRQ, REGISTER_R14, GetBranchLinkValue());
    SetMode(PSR_MODE_IRQ);
    SetI(true);
    Branch(VECTOR_IRQ);
}

auto Arm::ExceptionFastInterrupt() -> void {
    SetRegister(PSR_MODE_FIQ, REGISTER_R14, GetBranchLinkValue());
    SetMode(PSR_MODE_FIQ);
    SetI(true);
    SetF(true);
    Branch(VECTOR_FIQ);
}

auto Arm::Execute(uint32_t instruction) -> void {
    switch (InstructionType(instruction)) {
        case 0x0u:
            if (InstructionDataProcessingRegisterShiftBit(instruction)) {
                return ExecuteDataProcessingRegisterShift(instruction);
            } else {
                return ExecuteDataProcessingInstructionShift(instruction);
            }
        case 0x1u: return ExecuteDataProcessingImmediate(instruction);
        case 0x2u:
            if (InstructionSingleDataTransferLoadBit(instruction)) {
                return ExecuteSingleDataTransferImmediateLoad(instruction);
            } else {
                return ExecuteSingleDataTransferImmediateStore(instruction);
            }
        case 0x3u:
            if (InstructionSingleDataTransferRegisterInvalidBit(instruction)) [[unlikely]] {
                return ExceptionUndefinedInstruction();
            } else if (InstructionSingleDataTransferLoadBit(instruction)) {
                return ExecuteSingleDataTransferRegisterLoad(instruction);
            } else {
                return ExecuteSingleDataTransferRegisterStore(instruction);
            }
        case 0x4u:
            if (InstructionSingleDataTransferLoadBit(instruction)) {
                return ExecuteBlockDataTransferLoad(instruction);
            } else {
                return ExecuteBlockDataTransferStore(instruction);
            }
        case 0x5u: return ExecuteBranch(instruction);
        case 0x6u: return ExecuteCoprocessorDataTransfer(instruction);
        case 0x7u:
            if (InstructionCoprocessorSwiBit(instruction)) {
                return ExceptionSoftwareInterrupt();
            } else if (InstructionCoprocessorRegisterTransferBit(instruction)) {
                return ExecuteCoprocessorRegisterTransfer(instruction);
            } else {
                return ExecuteCoprocessorDataOperation(instruction);
            }
        default: return;
    }
}

auto Arm::Reset() -> void { ExceptionReset(); }

auto Arm::Execute() -> void {
    // The underlying assumption of this method is that the program counter is
    // always 8 bytes ahead of the current instruction
    // Exception priorities according to the VLSI manual
    // Reset > Address Exception and Data Aborts > FIQ > IRQ > Prefetch abort > SWI

    // Check for interrupts
    if (!GetF() && GetInterrupts().PendingFiq()) {
        ExceptionFastInterrupt();
        return;
    }
    if (!GetI() && GetInterrupts().PendingIrq()) {
        ExceptionInterrupt();
        return;
    }

    const auto& [instruction, prefetchAbort] = GetPipelineDecode();

    if (prefetchAbort) {
        ExceptionPrefetchAbort();
    } else if (TestCondition(instruction)) {
        Execute(instruction);
    } else {
        CycleS();
        PrefetchInstruction();
        IncrementPC();
    }
}