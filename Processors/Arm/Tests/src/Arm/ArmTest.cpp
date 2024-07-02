#include <regex>
#include <unordered_map>

#include "Arm/ArmTest.h"
#include "Arm/Arm.h"
#include "Arm/Assembler.h"

using namespace rbrown::arm;

constexpr auto PSR_MODE_USR = 0u;
constexpr auto PSR_MODE_FIQ = 1u;
constexpr auto PSR_MODE_IRQ = 2u;
constexpr auto PSR_MODE_SVC = 3u;

class MemoryMock: public Memory {
public:
    MemoryMock();

    void Reset() final;

    void CycleS() final;
    void CycleN() final;
    void CycleI(uint32_t) final;

    bool ReadWord(uint32_t, uint32_t&) final;
    bool ReadByte(uint32_t, uint32_t&) final;
    bool WriteWord(uint32_t, uint32_t) final;
    bool WriteByte(uint32_t, uint32_t) final;

    bool GetSupervisor() const final;
    void SetSupervisor(bool) final;

    bool ReadWordInner(uint32_t, uint32_t&);
    bool ReadByteInner(uint32_t, uint32_t&);
    bool WriteWordInner(uint32_t, uint32_t);
    bool WriteByteInner(uint32_t, uint32_t);
    void SetAbortWord(uint32_t, bool);
    std::string GetObservedCycles() const;
    std::string GetObservedSupervisorStates() const;
private:
    [[nodiscard]] bool GetByte(uint32_t, uint8_t&) const;
    [[nodiscard]] bool SetByte(uint32_t, uint8_t);
    void RecordSupervisorState();
    std::stringstream cycleStream;
    std::stringstream supervisorStream;
    std::unordered_map<uint32_t, uint8_t> store;
    std::unordered_map<uint32_t, bool> abort;
    bool supervisor;
};

class InterruptsMock: public Interrupts {
public:
    InterruptsMock();
    void Reset() final;
    bool PendingIrq() final;
    bool PendingFiq() final;
    void SetIrq(bool);
    void SetFiq(bool);
private:
    bool irq, fiq;
};


MemoryMock::MemoryMock() : cycleStream {}, supervisorStream {}, store {}, abort {}, supervisor {true} {}

void MemoryMock::Reset() {}
void MemoryMock::CycleS() { cycleStream << 'S'; }
void MemoryMock::CycleN() { cycleStream << 'N'; }
void MemoryMock::CycleI(uint32_t n) { if (n != 1u) { cycleStream << n; } cycleStream << 'I'; }

bool MemoryMock::ReadWord(uint32_t address, uint32_t& value) {
    RecordSupervisorState();
    return ReadWordInner(address, value);
}

bool MemoryMock::ReadByte(uint32_t address, uint32_t& value) {
    RecordSupervisorState();
    return ReadByteInner(address, value);
}

bool MemoryMock::WriteWord(uint32_t address, uint32_t value) {
    RecordSupervisorState();
    return WriteWordInner(address, value);
}

bool MemoryMock::WriteByte(uint32_t address, uint32_t value) {
    RecordSupervisorState();
    return WriteByteInner(address, value);
}

bool MemoryMock::GetSupervisor() const { return supervisor; }
void MemoryMock::SetSupervisor(bool v) { supervisor = v; }

bool MemoryMock::ReadWordInner(uint32_t address, uint32_t& value) {
    const auto wordAddress = address & ~0x3u;
    uint8_t a, b, c, d;
    auto result = true;
    result &= GetByte(wordAddress + 0u, a);
    result &= GetByte(wordAddress + 1u, b);
    result &= GetByte(wordAddress + 2u, c);
    result &= GetByte(wordAddress + 3u, d);
    if (result) {
        value = (d << 24u) + (c << 16u) + (b << 8u) + a;
    }
    return result;
}

bool MemoryMock::ReadByteInner(uint32_t address, uint32_t& value) {
    uint8_t v;
    if (GetByte(address, v)) {
        value = v;
        return true;
    }
    return false;
}

bool MemoryMock::WriteWordInner(uint32_t address, uint32_t value) {
    const auto wordAddress = address & ~0x3u;
    bool result = true;
    result &= SetByte(wordAddress + 0u, uint8_t(value));
    result &= SetByte(wordAddress + 1u, uint8_t(value >> 8u));
    result &= SetByte(wordAddress + 2u, uint8_t(value >> 16u));
    result &= SetByte(wordAddress + 3u, uint8_t(value >> 24u));
    return result;
}

bool MemoryMock::WriteByteInner(uint32_t address, uint32_t value) {
    return SetByte(address, value);
}

void MemoryMock::SetAbortWord(uint32_t address, bool value) {
    const auto wordAddress = address & ~0x3u;
    abort[wordAddress + 0u] = value;
    abort[wordAddress + 1u] = value;
    abort[wordAddress + 2u] = value;
    abort[wordAddress + 3u] = value;
}

std::string MemoryMock::GetObservedCycles() const { return cycleStream.str(); }
std::string MemoryMock::GetObservedSupervisorStates() const { return supervisorStream.str(); }

bool MemoryMock::GetByte(uint32_t address, uint8_t& value) const {
    if (abort.contains(address) && abort.at(address)) {
        return false;
    } else if (store.contains(address)) {
        value = store.at(address);
    } else {
        value = 0xFFu;
    }
    return true;
}

bool MemoryMock::SetByte(uint32_t address, uint8_t value) {
    if (abort.contains(address) && abort.at(address)) {
        return false;
    }
    store[address] = value;
    return true;
}

void MemoryMock::RecordSupervisorState() { supervisorStream << (supervisor ? 'S' : 's'); }

InterruptsMock::InterruptsMock(): irq{}, fiq{} {}
void InterruptsMock::Reset() {}
bool InterruptsMock::PendingIrq() { return irq; }
bool InterruptsMock::PendingFiq() { return fiq; }
void InterruptsMock::SetIrq(bool v) { irq = v; }
void InterruptsMock::SetFiq(bool v) { fiq = v; }

struct ArmTest::ArmTestImpl {
    ArmTestImpl() : memory{}, interrupts{}, arm{ memory, interrupts } {}
    MemoryMock memory;
    InterruptsMock interrupts;
    Arm arm;
};

namespace {

struct Status {
    bool n, z, c, v, i, f;
    uint32_t mode;
};

auto MapModeStringToValue(const std::string& mode) {
    if (mode == "USR") return PSR_MODE_USR;
    if (mode == "FIQ") return PSR_MODE_FIQ;
    if (mode == "IRQ") return PSR_MODE_IRQ;
    if (mode == "SVC") return PSR_MODE_SVC;
    return PSR_MODE_USR;
}

auto MapModeValueToString(uint32_t value) {
    if (value == PSR_MODE_USR) return "USR";
    if (value == PSR_MODE_FIQ) return "FIQ";
    if (value == PSR_MODE_IRQ) return "IRQ";
    if (value == PSR_MODE_SVC) return "SVC";
    return "USR";
}

auto MatchPSR(const char* condition, Status& status) {
    static const std::regex regex {R"(^PSR is ([NZCVIF]+|0),(USR|FIQ|IRQ|SVC)$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        for (const auto& c: match.str(1)) {
            if (c == 'N') status.n = true;
            if (c == 'Z') status.z = true;
            if (c == 'C') status.c = true;
            if (c == 'V') status.v = true;
            if (c == 'I') status.i = true;
            if (c == 'F') status.f = true;
        }
        status.mode = MapModeStringToValue(match.str(2));
        return true;
    }
    return false;
}

auto MatchPC(const char* condition, uint32_t& value) {
    static const std::regex regex {R"(^PC is \$([0-9A-F]+)$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        value = std::stoul(match.str(1), nullptr, 16);
        return true;
    }
    return false;
}

auto MatchRegister(const char* condition, uint32_t& mode, uint32_t& r, uint32_t& v) {
    static const std::regex regex {R"(^R([0-9]|1[0-4])(?:_(USR|FIQ|IRQ|SVC))? is \$([0-9A-F]+)$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        mode = MapModeStringToValue(match.str(2));
        r = std::stoul(match.str(1), nullptr, 10);
        v = std::stoul(match.str(3), nullptr, 16);
        return true;
    }
    return false;
}

auto MatchInterrupts(const char* condition, bool& irq, bool& fiq) {
    static std::regex regex {R"(^IRQ is ([I|F]+|0)$)"};
    irq = fiq = false;
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        for (const auto& c: match.str(1)) {
            if (c == 'I') irq = true;
            if (c == 'F') fiq = true;
        }
        return true;
    }
    return false;
}

auto MatchMemoryWord(const char* condition, uint32_t& address, uint32_t& value) {
    static std::regex regex {R"(^\(\$([0-9A-F]{8})\).W is \$([0-9A-F]{8})$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        address = std::stoul(match.str(1), nullptr, 16);
        value = std::stoul(match.str(2), nullptr, 16);
        return true;
    }
    return false;
}

auto MatchMemoryByte(const char* condition, uint32_t& address, uint32_t& value) {
    static std::regex regex {R"(^\(\$([0-9A-F]{8})\).B is \$([0-9A-F]{2})$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        address = std::stoul(match.str(1), nullptr, 16);
        value = std::stoul(match.str(2), nullptr, 16);
        return true;
    }
    return false;
}

auto MatchMemoryWordAssembly(const char* condition, uint32_t& address, uint32_t& value) {
    static std::regex regex {R"(^\(\$([0-9A-F]{8})\).ASM is (.*)$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        address = std::stoul(match.str(1), nullptr, 16);
        return Assemble(match.str(2), value);
    }
    return false;
}

auto MatchMemoryWordAbort(const char* condition, uint32_t& address) {
    static std::regex regex {R"(^\(\$([0-9A-F]{8})\).W is ABORT$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        address = std::stoul(match.str(1), nullptr, 16);
        return true;
    }
    return false;
}

auto MatchCycles(const char* condition, std::stringstream& stream) {
    static const std::regex regex {R"(^CYCLES is ((?:(?:[1-9]|1[0-6])?[S|N|I])+)$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        stream << match.str(1);
        return true;
    }
    return false;
}

auto MatchSupervisor(const char* condition, std::stringstream& sequenceStream, std::stringstream& finalStream) {
    static const std::regex regex {R"(^SPMD is ([sS]+),(s|S)$)"};
    std::cmatch match;
    if (std::regex_match(condition, match, regex)) {
        sequenceStream << match.str(1);
        finalStream << match.str(2);
        return true;
    }
    return false;
}
}

ArmTest::ArmTest(): impl{std::make_unique<ArmTest::ArmTestImpl>()} {}
ArmTest::~ArmTest() = default;

void ArmTest::Given(const std::initializer_list<const char*>& conditions) {
    auto& [memory, interrupts, arm] = *impl;
    const auto size = conditions.size();
    const auto data = std::data(conditions);
    Status status {};
    uint32_t address {}, value {};
    uint32_t mode{}, reg{};
    bool irq{}, fiq{};
    ASSERT_GE(size, 2u);
    // Match the PSR
    ASSERT_TRUE(MatchPSR(data[0u], status)) << "Expected Status register in first position";
    arm.SetMode(status.mode);
    arm.SetN(status.n);
    arm.SetZ(status.z);
    arm.SetC(status.c);
    arm.SetV(status.v);
    arm.SetI(status.i);
    arm.SetF(status.f);
    // Match the Program counter
    ASSERT_TRUE(MatchPC(data[1u], value)) << "Expected Program counter in second position";
    arm.SetPC(value);
    // Match registers
    auto index = 2u;
    while (index < size && MatchRegister(data[index], mode, reg, value)) {
        arm.SetRegister(mode, reg, value);
        ++index;
    }
    // Match interrupts
    if (index < size && MatchInterrupts(data[index], irq, fiq)) {
        interrupts.SetIrq(irq);
        interrupts.SetFiq(fiq);
        ++index;
    }
    // Match memory locations
    for (; index < size; ++index) {
        const auto& condition = data[index];
        if (MatchMemoryWord(condition, address, value) || MatchMemoryWordAssembly(condition, address, value)) {
            ASSERT_EQ(0, address & 0x3u);
            ASSERT_TRUE(memory.WriteWordInner(address, value));
        } else if (MatchMemoryWordAbort(condition, address)) {
            ASSERT_EQ(0, address & 0x3u);
            memory.SetAbortWord(address, true);
        } else if (MatchMemoryByte(condition, address, value)) {
            ASSERT_TRUE(memory.WriteByteInner(address, value));
        } else {
            FAIL() << "Unmatched pre condition " << condition;
        }
    }
}

void ArmTest::When(const std::initializer_list<const char*>& assembly, uint32_t extraInvocations) {
    auto& [memory, interrupts, arm] = *impl;
    std::vector<uint32_t> instructions;
    for (const auto& line: assembly) {
        uint32_t instruction;
        ASSERT_TRUE(Assemble(line, instruction)) << "Assembly failed " << line;
        instructions.push_back(instruction);
    }
    const auto count = instructions.size();
    auto address = arm.GetPC() - 8u;
    for (const auto instruction: instructions) {
        ASSERT_TRUE(memory.WriteWordInner(address & 0x03FFFFFCu, instruction));
        address += 4u;
    }
    if (count > 0u) {
        arm.SetDecodedInstruction(instructions[0u]);
    }
    if (count > 1u) {
        arm.SetFetchedInstruction(instructions[1u]);
    }
    for (auto n = 0u; n < count + extraInvocations; n++) {
        arm.Execute();
    }
}

void ArmTest::When(const std::initializer_list<uint32_t>& instructions, uint32_t extraInvocations) {
    auto& [memory, interrupts, arm] = *impl;
    const auto count = instructions.size();
    const auto data = std::data(instructions);
    auto address = arm.GetPC() - 8u;
    for (const auto instruction: instructions) {
        ASSERT_TRUE(memory.WriteWordInner(address & 0x03FFFFFCu, instruction));
        address += 4u;
    }
    if (count > 0u) {
        arm.SetDecodedInstruction(data[0u]);
    }
    if (count > 1u) {
        arm.SetFetchedInstruction(data[1u]);
    }
    for (auto n = 0u; n < count + extraInvocations; n++) {
        arm.Execute();
    }
}

void ArmTest::Then(const std::initializer_list<const char*>& conditions) {
    auto&[memory, interrupts, arm] = *impl;
    const auto size = conditions.size();
    const auto data = std::data(conditions);
    std::stringstream cycles, supervisor, supervisorFinal;
    Status status{};
    uint32_t address{}, value{};
    uint32_t mode{}, reg{};
    ASSERT_GE(size, 3u);
    // Match the cycle counts
    ASSERT_TRUE(MatchCycles(data[0u], cycles)) << "Expected cycle counts in first position";
    ASSERT_EQ(cycles.str(), memory.GetObservedCycles()) << data[0u];
    // Match the PSR
    ASSERT_TRUE(MatchPSR(data[1u], status)) << "Expected PSR in first position";
    ASSERT_EQ(MapModeValueToString(status.mode),
              MapModeValueToString(arm.GetMode())) << data[1u];
    ASSERT_EQ(status.n, arm.GetN()) << data[1u];
    ASSERT_EQ(status.z, arm.GetZ()) << data[1u];
    ASSERT_EQ(status.c, arm.GetC()) << data[1u];
    ASSERT_EQ(status.v, arm.GetV()) << data[1u];
    ASSERT_EQ(status.i, arm.GetI()) << data[1u];
    ASSERT_EQ(status.f, arm.GetF()) << data[1u];
    // Match the program counter
    ASSERT_TRUE(MatchPC(data[2u], value)) << "Expected program counter in third position";
    ASSERT_EQ(value, arm.GetPC()) << data[2u];
    // Match the supervisor/trans pin activity
    auto index = 3u;
    if (index < size && MatchSupervisor(data[index], supervisor, supervisorFinal)) {
        ASSERT_EQ(supervisor.str(), memory.GetObservedSupervisorStates()) << data[index];
        ASSERT_EQ(supervisorFinal.str(),
                  memory.GetSupervisor() ? "S" : "s") << data[index];
        ++index;
    }
    // Match the registers
    while (index < size && MatchRegister(data[index], mode, reg, value)) {
        ASSERT_EQ(value, arm.GetRegister(mode, reg)) << data[index];
        ++index;
    }
    // Match memory locations
    for (; index < size; ++index) {
        const auto& condition = data[index];
        uint32_t observed;
        if (MatchMemoryWord(condition, address, value) || MatchMemoryWordAssembly(condition, address, value)) {
            ASSERT_EQ(0, address & 0x3u);
            ASSERT_TRUE(memory.ReadWordInner(address, observed)) << condition;
            ASSERT_EQ(value, observed) << condition;
        } else if (MatchMemoryByte(condition, address, value)) {
            ASSERT_TRUE(memory.ReadByteInner(address, observed)) << condition;
            ASSERT_EQ(value, observed) << condition;
        } else {
            FAIL() << "Unmatched post condition " << condition;
        }
    }
}