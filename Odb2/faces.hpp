#pragma once
#include <M5Stack.h>

#define KEYBOARD_I2C_ADDR 0X08
#define KEYBOARD_INT 5

static xQueueHandle keyboard_queue;
static void IRAM_ATTR keyboard_callback()
{
    if (!digitalRead(KEYBOARD_INT) == LOW) {
        return;
    }
    // IRAM_ATTR = use or to be in the Internal RAM of the ESP32
    uint8_t key_val;
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1); // request 1 byte from keyboard
    key_val = Wire.read();                  // receive a byte as character
    if (key_val != 0)
    {
        Serial.println(key_val);
        //xQueueSendFromISR(keyboard_queue, &key_val, NULL);
    }
}

class KeyboardFace {
private:
    Deps& deps;

public:
    KeyboardFace(Deps &deps): deps(deps)
    {
        Wire.begin();
        deps.lcd.println("FACES Keyboard I2C Read Example");

        //create a queue to handle gpio event from isr
        keyboard_queue = xQueueCreate(10, sizeof(uint8_t));
        pinMode(KEYBOARD_INT, INPUT_PULLUP);
        attachInterrupt(KEYBOARD_INT, keyboard_callback, FALLING);
    }


    void loop()
    {
        uint8_t key_val;
        // xQueueReceive(keyboard_queue, &key_val, portMAX_DELAY) // block
        if (xQueueReceive(keyboard_queue, &key_val, 0))
        { // Non-blocking
            if (key_val >= 0x20 && key_val < 0x7F)
            { // ASCII String
                Serial.println("its this case");
                Serial.print((char)key_val);
                M5.Lcd.print((char)key_val);
            }
            else
            {
                Serial.println("its the other case" + key_val);
                Serial.printf("0x%02X ", key_val);
                M5.Lcd.printf("0x%02X ", key_val);
            }
        }
    }
};
