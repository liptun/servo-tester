#define DISPLAY_HELPER_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DisplayHelper {
public:
  DisplayHelper();
  void init();
  void (*onRender)(Adafruit_SSD1306 &display) = nullptr;
  void update();

private:
  unsigned long lastMillis;
  Adafruit_SSD1306 display;
};
