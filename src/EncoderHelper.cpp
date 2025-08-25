#include <Arduino.h>
#include <Config.h>
#include <EncoderHelper.h>

EncoderHelper::EncoderHelper(byte clkPin, byte dtPin, byte swPin,
                             byte stepsPerClick)
    : encoder(clkPin, dtPin), swPin(swPin), stepsPerClick(stepsPerClick),
      lastPosition(0), lastStepTime(0), lastButtonState(HIGH),
      lastDebounceMillis(0) {}

void EncoderHelper::init() {}

void EncoderHelper::update() {
  unsigned long currentMillis = millis();

  long newPosition = encoder.read() / stepsPerClick;
  if (newPosition != lastPosition) {
    int direction = (newPosition < lastPosition) ? ENCODER_DIRECTION_CW
                                                 : ENCODER_DIRECTION_CC;
    unsigned long deltaT = currentMillis - lastStepTime;
    lastStepTime = currentMillis;
    lastPosition = newPosition;

    if (onTurn) {
      onTurn(direction, deltaT);
    }
  }

  boolean buttonState = digitalRead(swPin);

  if (buttonState == LOW && lastButtonState == HIGH) {
    lastDebounceMillis = currentMillis;
  } else if (buttonState == HIGH && lastButtonState == LOW &&
             currentMillis - lastDebounceMillis >= BUTTON_DEBOUNCE_DURATION) {
    unsigned long pressedDuration = currentMillis - lastDebounceMillis;
    if (onButtonClick) {
      onButtonClick(pressedDuration);
    }
  }
  lastButtonState = buttonState;
}
