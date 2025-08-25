#define ENCODER_HELPER_H
#include "Encoder.h"

class EncoderHelper {

public:
  EncoderHelper(byte clkPin, byte dtPin, byte swPin, byte stepsPerClick);
  void init();
  void update();
  void (*onTurn)(int8_t direction, unsigned long deltaT) = nullptr;
  void (*onButtonClick)(unsigned long pressDuration) = nullptr;

private:
  Encoder encoder;
  byte swPin;
  byte stepsPerClick;
  long lastPosition;
  unsigned long lastStepTime;
  boolean lastButtonState;
  unsigned long lastDebounceMillis;
};
