#pragma once

#include <gtest/gtest.h>

class ArmTest : public ::testing::Test {
protected:
    ArmTest();
    ~ArmTest() override;
    void Given(const std::initializer_list<const char*>&);
    void When(const std::initializer_list<const char*>&, uint32_t = 0u);
    void When(const std::initializer_list<uint32_t>&, uint32_t = 0u);
    void Then(const std::initializer_list<const char*>&);
protected:
    struct ArmTestImpl;
    std::unique_ptr<ArmTestImpl> impl;
};