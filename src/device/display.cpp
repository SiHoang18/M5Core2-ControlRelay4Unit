#include "display.h"

String relay_state[4] = {"OFF", "OFF", "OFF", "OFF"};

String last_state[4] = {"", "", "", ""};

int topOffset = 40;
int boxWidth;
int boxHeight;

void drawRelayFrame(int i) {
    int col = i % 2;
    int row = i / 2;

    int x = col * boxWidth;
    int y = topOffset + row * boxHeight;

    M5.Lcd.drawRect(x + 5, y + 5, boxWidth - 10, boxHeight - 10, WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setCursor(x + 15, y + 15 + row * 5);
    M5.Lcd.printf("Relay %d", i + 1);
}

// Hàm vẽ trạng thái ON/OFF cho từng relay
void drawRelayState(int i) {
    if (relay_state[i] == last_state[i]) {
        return; // Không đổi thì khỏi vẽ lại
    }
    last_state[i] = relay_state[i];

    int col = i % 2;
    int row = i / 2;

    int x = col * boxWidth;
    int y = topOffset + row * boxHeight;

    int stateBoxX = x + 15;
    int stateBoxY = y + 50;
    int stateBoxW = boxWidth - 30;
    int stateBoxH = 24;

    M5.Lcd.fillRect(stateBoxX, stateBoxY, stateBoxW, stateBoxH, BLACK);

    if (relay_state[i] == "ON") {
        M5.Lcd.setTextColor(GREEN, BLACK);
    } else {
        M5.Lcd.setTextColor(RED, BLACK);
    }
    M5.Lcd.setCursor(stateBoxX, stateBoxY);
    M5.Lcd.printf("%s", relay_state[i].c_str());
}

void initDisplay() {
    boxWidth = M5.Lcd.width() / 2;
    boxHeight = (M5.Lcd.height() - topOffset) / 2;

    M5.Lcd.fillScreen(BLACK);
    for (int i = 0; i < 4; i++) {
        drawRelayFrame(i);
        drawRelayState(i); // vẽ trạng thái ban đầu
    }
}