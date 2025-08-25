#include <Arduino.h>
#include <Config.h>
#include <DisplayHelper.h>
#include <Wire.h>

DisplayHelper::DisplayHelper()
    : lastMillis(0), display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

void DisplayHelper::init() {
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void DisplayHelper::update() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= RENDER_INTERVAL_DURATION) {
    lastMillis = currentMillis;
    display.clearDisplay();
    display.setCursor(0, 0);

    if (onRender) {
      onRender(display);
    }

    display.display();
  }
}
