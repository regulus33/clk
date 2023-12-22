//
// Created by zachary rowden on 21.12.2023.
//

#ifndef CLK_MOCK_PIND_H
#define CLK_MOCK_PIND_H
#include <Arduino.h>

class MockPinD {
private:
    uint8_t mockValue; // Variable to hold the mock PIND value
    static MockPinD *instance;

    // Private constructor
    MockPinD() : mockValue(0) {}

public:
    // Singleton access method
    static MockPinD *getInstance() {
        if (!instance)
            instance = new MockPinD();
        return instance;
    }

    // Method to set the mock value
    void setMockValue(uint8_t value) {
        mockValue = value;
    }

    // Method to get the mock value
    uint8_t getMockValue() const {
        return mockValue;
    }
};

// Initialize the static instance pointer
MockPinD *MockPinD::instance = nullptr;

#endif //CLK_MOCK_PIND_H
