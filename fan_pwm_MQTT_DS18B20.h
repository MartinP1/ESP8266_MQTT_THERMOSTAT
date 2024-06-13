
void setSpeed(uint8_t uiSpeed) {
  uiSpeed = 255 - uiSpeed; // circuitry is inverting, ensure 255 = max, 0 = off
#if defined(ARDUINO_D1_MINI32) || defined(ARDUINO_LOLIN_S2_MINI)
  ledcWrite( pwmGpio, uiSpeed);
#else
  analogWrite(pwmGpio, uiSpeed);
#endif
}

void setValve (bool bState) {
  digitalWrite(valveGpio, bState);
}