unsigned long readVccPreviousTime = 0;




void setup()
{
  readVcc();
  Serial.begin(115200);
}

void loop()
{
  Serial.println(readVcc());
  delay(100);
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

  //only use this block if using an Arduino MEGA
#if defined(__AVR_ATmega2560__)
  ADCSRB &= ~_BV(MUX5); // Without this the function always returns -1 on the ATmega2560
#endif

  readVccPreviousTime = millis();

  if ((millis() - readVccPreviousTime) >= 2L) {
//    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC)); // measuring

    uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
    uint8_t high = ADCH; // unlocks both

    long result = (high << 8) | low;

    //  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000  1116237L,
    //  result = 1116237L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    result = 1108800L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
    return result; // Vcc in millivolts
  }
}
