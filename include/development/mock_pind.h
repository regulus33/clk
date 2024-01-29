//
// Created by zachary rowden on 21.12.2023.
//

#ifndef CLK_MOCK_PIND_H
#define CLK_MOCK_PIND_H
#include <Arduino.h>

// To understand the need for this class, go to ButtonService and check the value of MY_PIND when TEST_BUILD is true
// we use to swap mocked values for the PIND register.
class MockPinD {
private:
    // here is that "int" i mentioned above. But it makes more sense to use arduino's directive thingies
    // i.e. b00000001
    uint8_t mockValue;
    // this is just C++ singleton stuff...
    static MockPinD *instance;

    // only the class can touch the constructor obviously, again cpp stuff
    MockPinD() : mockValue(0) {}

public:
    // get the instance
    static MockPinD *getInstance() {
        if (!instance)
            instance = new MockPinD();
        return instance;
    }

    // Method to set the mock value, this is used in tests.cpp
    void setMockValue(uint8_t value) {
        mockValue = value;
    }

    // Used inside ButtonService->readPin() when test build is true
    uint8_t getMockValue() const {
        return mockValue;
    }
};

//If we don't initialize it to nullptr, the instance pointer may have some random value at the start
// (since by default, pointers in C++ are not initialized to any specific value).
MockPinD *MockPinD::instance = nullptr;

#endif //CLK_MOCK_PIND_H
