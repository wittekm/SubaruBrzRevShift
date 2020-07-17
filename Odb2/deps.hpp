#pragma once

struct Deps {
    Deps() : lcd(M5.Lcd) {
    }

    M5Display& lcd;

    const unsigned long curTimeMillis() {
        return millis();
    }

    const void sleepMs(int millis) {
        delay(millis);
    }

    const void freshScreen() {
        lcd.clear();
        lcd.setCursor(0, 0);
    }
};