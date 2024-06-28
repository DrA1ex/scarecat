#include <Arduino.h>

#include "const.h"

#include "misc/button.h"
#include "misc/led.h"
#include "misc/buzzer.h"

static Led led(LR_PIN, LG_PIN, LB_PIN);
static Button<BUTTON_PIN> button;

static Note melody[] = {
        {650,  250},
        {1350, 250},
};

static Buzzer buzzer(BUZZER_PIN, melody, sizeof(melody) / sizeof(melody[0]));

enum class State {
    IDLE,
    PANIC,
    SILENT
};

static State state = State::IDLE;
static unsigned long last_time = 0;

static unsigned long silence_time = 0;
static uint8_t silence_level = 0;

void change_state(State next_state);

void button_clicked(uint8_t count);
void button_hold(uint8_t);

void setup() {
    Serial.begin(9600);

    pinMode(PIR_PIN, INPUT);


    led.setColor(0, 0, 0);

    button.begin();
    button.set_on_click(button_clicked);
    button.set_on_hold(button_hold);

    Serial.println("Initialized");
}

void change_state(State next_state) {
    if (next_state == state) return;

    if (state == State::PANIC) {
        buzzer.stop();
    }

    if (next_state == State::PANIC) {
        buzzer.play();
        led.flash();
    } else if (next_state == State::SILENT) {
        led.setColor(0, LED_G_STEP, 0);
        led.flash(LED_BLINK_COOL_DOWN_DURATION);
    } else {
        led.turnOff();
    }

    Serial.print("Change state from ");
    Serial.print((uint8_t) state);
    Serial.print(" to ");
    Serial.println((uint8_t) next_state);

    state = next_state;
    last_time = millis();
}
void button_clicked(uint8_t count) {
    change_state(State::SILENT);

    if (count <= silence_level) return;

    silence_time = count * SILENT_TIME;
    silence_level = count - 1;

    Serial.print("Clicked: ");
    Serial.println(count);
}
void button_hold(uint8_t) {
    if (silence_time == 0) return;

    Serial.println("Reset");

    silence_time = 0;
    silence_level = 0;
}

void state_machine(unsigned long time) {
    auto delta = time - last_time;

    switch (state) {
        case State::IDLE:
            if (digitalRead(PIR_PIN) == HIGH) {
                change_state(State::PANIC);
            }
            break;
        case State::PANIC:
            if (delta >= BUZZ_TIME) {
                change_state(State::IDLE);
            }
            break;
        case State::SILENT:
            if (delta >= silence_time) {
                change_state(State::IDLE);

                silence_time = 0;
                silence_level = 0;
            }
            break;
    }
}

void loop() {
    auto time = millis();

    button.handle();
    state_machine(time);

    auto delta = time - last_time;
    switch (state) {
        case State::IDLE:
            if (delta >= IDLE_FLASH_TIME) {
                last_time = time;

                led.setColor(64, 0, 0);
                led.blink();
            }
            break;

        case State::PANIC:
            if ((delta / PANIC_COLOR_TIME) % 2) {
                led.setColor(255, 0, 0);
            } else {
                led.setColor(0, 0, 255);
            }
            break;

        case State::SILENT: {
            auto level = (int) ((silence_time - delta - 1) / SILENT_TIME) + 1;

            if (!led.active() || led.blinkCount() != level) {
                Serial.print("Silent mode level: ");
                Serial.println(level);

                led.blink(level, true);
            }
        }
            break;
    }

    buzzer.tick(time);
    led.tick(time);
}

