// кнопки-крутилки

void encTick() {
  enc.tick();
  if (enc.isTurn()) {
    timeoutReset();
    if (enc.isLeft()) thisVolume += 1;
    if (enc.isRight()) thisVolume -= 1;
    thisVolume = constrain(thisVolume, 1, 200);
    dispMode();
    EEPROM.put(0, thisVolume);
  }
}

void btnTick() {
  if (btn.holded()) {
    timeoutReset();
    workMode = !workMode;
    dispMode();
  }
  if (encBtn.holded()) {
    int8_t pumpingShot = -1;
    for (byte i = 0; i < NUM_SHOTS; i++) {    // поиск наличия рюмки
      if (!digitalRead(SW_pins[i])) {         // нашли рюмку
        servoON();
        servo.attach(SERVO_PIN, shotPos[i]);  // едем к ней
        pumpingShot = i;
      }
    }
    if (pumpingShot == -1) return; // нет рюмок -> нет прокачки, ищем заново ^
    delay(500); // дадим немного времени для серво
    pumpON(); // включаем помпу
    while (!digitalRead(SW_pins[pumpingShot]) && !digitalRead(ENC_SW)); // пока стоит рюмка и зажат энкодер, продолжаем наливать
    pumpOFF();
    delay(300);
    servo.write(HOME_POS);
    delay(300);
    servoOFF();
    timeoutReset();
    systemState = WAIT;
  }
}
