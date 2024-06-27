#include "Arduino.h"
#include "const.h"
#include "misc/led.h"

Led led(LR_PIN, LG_PIN, LB_PIN);

void setup() {
    pinMode(BUTTON_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);


    led.changeColor(0, 0, 0);
}

enum class State {
    IDLE,
    PANIC,
    SILENT
};

void waitChanges() {
    if (digitalRead(PIR_PIN) == HIGH) {


        led.changeColor(255, 0, 0);
        tone(BUZZER_PIN, 800);
        delay(BUZZ_TIME);
        led.changeColor(0, 0, 0);
        noTone(BUZZER_PIN);
    }
}

static State state = State::IDLE;
static unsigned long last_time = 0;

void changeState(State next_state, unsigned long time) {
    if (next_state == state) return;

    if (state == State::PANIC) {
        noTone(BUZZER_PIN);
        led.changeColor(0, 0, 0);
    } else if (state == State::SILENT) {
        led.changeColor(0, 0, 0);
    }

    state = next_state;
    last_time = time;
}

void stateMachine(unsigned long time) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
        changeState(State::SILENT, time);
    }

    switch (state) {
        case State::IDLE:
            if (digitalRead(PIR_PIN) == HIGH) {
                changeState(State::PANIC, time);
            }
            break;
        case State::PANIC:
            if (time - last_time >= BUZZ_TIME) {
                changeState(State::IDLE, time);
            }
            break;
        case State::SILENT:
            if (time - last_time >= SILENT_TIME) {
                changeState(State::IDLE, time);
            }
            break;
    }

};

void loop() {
    auto time = millis();

    stateMachine(time);

    switch (state) {
        case State::IDLE:
            if (time - last_time >= IDLE_FLASH_TIME) {
                last_time = time;
                led.changeColor(64, 0, 0);
                delay(60);
                led.changeColor(0, 0, 0);
            }
            break;

        case State::PANIC:
            if (((time - last_time) / PANIC_COLOR_TIME) % 2) {
                led.changeColor(255, 0, 0);
            } else {
                led.changeColor(0, 0, 255);
            }

            tone(BUZZER_PIN, 800);
            break;

        case State::SILENT:
            led.changeColor(0, 4, 0);
            break;
    }
}

