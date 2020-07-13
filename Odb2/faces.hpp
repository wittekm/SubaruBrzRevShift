#pragma once
#include <M5Stack.h>
#include "evts.hpp"

#define KEYBOARD_I2C_ADDR 0X08
#define KEYBOARD_INT 5

void kbSetup()
{
    Wire.begin();
    pinMode(KEYBOARD_INT, INPUT_PULLUP);
}

void kbLoop(EventQueue& eq)
{
    if (digitalRead(KEYBOARD_INT) == LOW)
    {
        Wire.requestFrom(KEYBOARD_I2C_ADDR, 1); // request 1 byte from keyboard
        while (Wire.available()) // num bytes remaining to be read
        {
            uint8_t key_val = Wire.read(); // receive a byte as character
            if (key_val != 0)
            {
                if (key_val >= 0x20 && key_val < 0x7F)
                { // ASCII String
                    Serial.print((char)key_val);
                    M5.Lcd.print((char)key_val);
                    Event event(EventType::KB, (char)key_val);
                    eq.add(event);
                }
                else
                {
                    Serial.printf("0x%02X ", key_val);
                    M5.Lcd.printf("0x%02X ", key_val);
                }
            }
        }
    }
}

void btnLoop(EventQueue& eq)
{
    char whichBtn = 255;
    if(M5.BtnA.wasPressed()) {
        whichBtn = 'a';
    } else if(M5.BtnB.wasPressed()) {
        whichBtn = 'b';
    } else if (M5.BtnC.wasPressed()) {
        whichBtn = 'c';
    }
    if (whichBtn != 255) {
        Event event(EventType::BTN, whichBtn);
        eq.add(event);
    }
}