
void setSpeed(uint8_t uiSpeed) {
#if defined(ARDUINO_D1_MINI32) || defined(ARDUINO_LOLIN_S2_MINI)
  ledcWrite( 0, uiSpeed);
#else
  uiSpeed = 255 - uiSpeed; // circuitry is inverting, ensure 255 = max, 0 = off
  analogWrite(pwmGpio, uiSpeed);
#endif
}

void setValve (bool bState) {
  digitalWrite(valveGpio, bState);
}