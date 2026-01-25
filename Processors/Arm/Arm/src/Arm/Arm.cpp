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
constexpr auto PSR_FLAG_SHIFT = 28u;

constexpr auto PSR_MASK_PC = 0x03FFFFFCu;
constexpr auto PSR_MASK_MODE = 0x00000003u;
constexpr auto PSR_MASK_USER = 0xF0000000u;
constexpr auto PSR_MASK_PRIV = 0xFC000000u;

constexpr auto VECTOR_RESET = 0x00u;
constexpr auto VECTOR_UNDEFINED_INSTRUCTION = 0x04u;
constexpr auto VECTOR_SOFTWARE_INTERRUPT = 0x08u;
constexpr auto VECTOR_PREFETCH_ABORT = 0x0Cu;
constexpr auto VECTOR_DATA_ABORT = 0x10u;
constexpr auto VECTOR_ADDRESS_EXCEPTION = 0x14u;
constexpr auto VECTOR_IRQ = 0x18u;
constexpr auto VECTOR_FIQ = 0x1Cu;

constexpr auto INSTRUCTION_NOOP = 0xE1A00000; // MOV R0, R0

constexpr auto TestCondition(const uint32_t psr, const uint32_t condition) -> bool {
    const auto n = (psr & PSR_FLAG_N) != 0u;
    const auto z = (psr & PSR_FLAG_Z) != 0u;
    const auto c = (psr & PSR_FLAG_C) != 0u;
    const auto v = (psr & PSR_FLAG_V) != 0u;
    switch (condition) {
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

consteval auto GenerateConditionTable() -> std::array<uint16_t, 8> {
    std::array<uint16_t, 8> table{};
    for (auto condition = 0u; condition < 16u; condition += 2) {
        auto entry = 0u;
        for (auto psr = 0u; psr < 16u; ++psr) {
            if (TestCondition(psr << PSR_FLAG_SHIFT, condition)) {
                entry += 1u << psr;
            }
        }
        table[condition >> 1u] = entry;
    }
    return table;
}

constexpr auto CONDITION_LOOKUP_TABLE = GenerateConditionTable();

constexpr auto TestConditionWithLookup(const uint32_t psr, const uint32_t condition) -> bool {
    const auto entry = CONDITION_LOOKUP_TABLE[condition >> 1u];
    return ((entry >> psr) ^ condition) & 1u;
}

constexpr auto MapRegisterByMode(const uint32_t mode, const uint32_t r) -> uint32_t {
    if (r < 8u) {
        return r;
    }
    if (r < 13u) {
        return r + 5u * (mode == PSR_MODE_FIQ);
    }
    if (r < 15u) {
        return r + 5u + 2u * mode;
    }
    return 26u;
}

Arm::Arm(Memory& mem, Interrupts& interrupts) :
    memory { mem },
    interrupts { interrupts },
    decode { .instruction = INSTRUCTION_NOOP, .prefetchAbort = false },
    fetch { .instruction = INSTRUCTION_NOOP, .prefetchAbort = false },
    regs { },
    pc { VECTOR_RESET },
    psr { PSR_FLAG_I + PSR_FLAG_F + PSR_MODE_SVC } {}

auto Arm::SetRegister(uint32_t mode, uint32_t r, uint32_t v) -> void { regs[MapRegisterByMode(mode, r)] = v; }
auto Arm::SetPC(uint32_t v) -> void { pc = v & PSR_MASK_PC; }
auto Arm::SetMode(uint32_t m) -> void{
    psr &= ~PSR_MASK_MODE;
    psr |= m & PSR_MASK_MODE;
    SetTransPin(GetMode() != PSR_MODE_USR);
}

auto Arm::SetN(const bool v) -> void { if (v) { psr |= PSR_FLAG_N; } else { psr &= ~PSR_FLAG_N; } }
auto Arm::SetZ(const bool v) -> void { if (v) { psr |= PSR_FLAG_Z; } else { psr &= ~PSR_FLAG_Z; } }
auto Arm::SetC(const bool v) -> void { if (v) { psr |= PSR_FLAG_C; } else { psr &= ~PSR_FLAG_C; } }
auto Arm::SetV(const bool v) -> void { if (v) { psr |= PSR_FLAG_V; } else { psr &= ~PSR_FLAG_V; } }
auto Arm::SetI(const bool v) -> void { if (v) { psr |= PSR_FLAG_I; } else { psr &= ~PSR_FLAG_I; } }
auto Arm::SetF(const bool v) -> void { if (v) { psr |= PSR_FLAG_F; } else { psr &= ~PSR_FLAG_F; } }

auto Arm::SetDecodedInstruction(uint32_t v) -> void { decode.instruction = v; decode.prefetchAbort = false; }
auto Arm::SetFetchedInstruction(uint32_t v) -> void { fetch.instruction = v; fetch.prefetchAbort = false; }

auto Arm::GetRegister(uint32_t mode, uint32_t r) const -> uint32_t { return regs[MapRegisterByMode(mode, r)]; }
auto Arm::GetPC() const -> uint32_t { return pc; }
auto Arm::GetMode() const -> uint32_t { return psr & PSR_MASK_MODE; }

auto Arm::GetRegisterWithoutPSR(uint32_t mode, uint32_t r) const -> uint32_t {
    if (IsPC(r)) {
        return this->GetPC();
    }
    return this->GetRegister(mode, r);
}

auto Arm::GetRegisterWithPSR(uint32_t mode, uint32_t r) const -> uint32_t {
    if (IsPC(r)) {
        return GetPC() + GetPSR();
    }
    return GetRegister(mode, r);
}

auto Arm::GetN() const -> bool { return psr & PSR_FLAG_N; }
auto Arm::GetZ() const -> bool { return psr & PSR_FLAG_Z; }
auto Arm::GetC() const -> bool { return psr & PSR_FLAG_C; }
auto Arm::GetV() const -> bool { return psr & PSR_FLAG_V; }
auto Arm::GetI() const -> bool { return psr & PSR_FLAG_I; }
auto Arm::GetF() const -> bool { return psr & PSR_FLAG_F; }

auto Arm::GetDecodedInstruction() const -> uint32_t { return decode.instruction; }

auto Arm::TransferAddressIsInvalid(uint32_t v) -> bool { return v & PSR_MASK_PRIV; }
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

auto Arm::SetPSR(uint32_t v) -> void {
    if (GetMode() == PSR_MODE_USR) {
        psr &= ~PSR_MASK_USER;
        psr |= v & PSR_MASK_USER;
        return;
    }
    psr &= ~PSR_MASK_PRIV;
    psr |= v & PSR_MASK_PRIV;
    SetMode(v);
}

auto Arm::GetPSR() const -> uint32_t {
    return psr;
}

auto Arm::TestCondition(const uint32_t instruction) const -> bool {
    return TestConditionWithLookup(GetPSR() >> PSR_FLAG_SHIFT, InstructionConditionCode(instruction));
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
    return address + GetPSR();
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