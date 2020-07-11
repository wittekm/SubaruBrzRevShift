#pragma once

struct Deps {
    Deps() : lcd(M5.Lcd) {
    }

    M5Display& lcd;

    const unsigned long curTimeMillis() {
        return millis();
    }

    const void sleep(int millis) {
        delay(millis);
    }
};