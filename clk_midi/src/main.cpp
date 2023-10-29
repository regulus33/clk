#include <Arduino.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();
#define INTERRUPT_PIN 2

// PPQN     = 24
// 1 bar is 4 quarter notes
// pulses in a bar = 24 * 4 = 96
// 2 bars = 192
// 4 bars = 384

volatile bool sendMidiClock = false;

void setSendMidiClock() {
    sendMidiClock = true;
}

void setup() {
    MIDI.begin();
    pinMode(INTERRUPT_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);// Set the pin as Input with Pull-Up resistor
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), setSendMidiClock, HIGH); // Trigger when the pin goes from HIGH to LOW
}

void loop() {
    if(sendMidiClock) {
        MIDI.sendRealTime(midi::MidiType::Clock);
        sendMidiClock = false;
    }
}



