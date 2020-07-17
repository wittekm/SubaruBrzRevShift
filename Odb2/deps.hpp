#pragma once

struct Deps {
    Deps() : lcd(M5.Lcd) {
    }

    M5Display& lcd;

    const unsigned long curTimeMillis() {
        return millis();
    }

    const void sleepMs(int millis = 2000) {
        delay(millis);
    }

    const void freshScreen() {
        lcd.clear();
        lcd.setCursor(0, 0);
    }

    const void text(char* input, int size=4, int color=YELLOW) {
        lcd.setTextSize(size);
        lcd.setTextColor(color, BLACK);
        lcd.println(input);
    }
};