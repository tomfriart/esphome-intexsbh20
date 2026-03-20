/*
 * project:  Intex PureSpa SB-H20 WiFi Controller
 *
 * file:     SBH20IO.cpp
 *
 * encoding: UTF-8
 * created:  14th March 2021
 *
 * Copyright (C) 2021 Jens B.
 *
 * SPDX-License-Identifier: CC-BY-NC-SA-4.0
 */

#include "SBH20IO.h"

static uint8_t s_data_pin = 0;
static uint8_t s_latch_pin = 0;

namespace FRAME_LED
{
  const uint16_t POWER = 0x0001;
  const uint16_t HEATER_ON = 0x0080;
  const uint16_t NO_BEEP = 0x0100;
  const uint16_t HEATER_STANDBY = 0x0200;
  const uint16_t BUBBLE = 0x0400;
  const uint16_t FILTER = 0x1000;
}

namespace FRAME_DIGIT
{
  const uint16_t POS_1 = 0x0040;
  const uint16_t POS_2 = 0x0020;
  const uint16_t POS_3 = 0x0800;
  const uint16_t POS_4 = 0x0004;
  const uint16_t POS_ALL = POS_1 | POS_2 | POS_3 | POS_4;

  const uint16_t SEGMENT_A = 0x2000;
  const uint16_t SEGMENT_B = 0x1000;
  const uint16_t SEGMENT_C = 0x0200;
  const uint16_t SEGMENT_D = 0x0400;
  const uint16_t SEGMENT_E = 0x0080;
  const uint16_t SEGMENT_F = 0x0008;
  const uint16_t SEGMENT_G = 0x0010;
  const uint16_t SEGMENT_DP = 0x8000;
  const uint16_t SEGMENTS = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G;

  const uint16_t OFF = 0x0000;
  const uint16_t NUM_0 = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F;
  const uint16_t NUM_1 = SEGMENT_B | SEGMENT_C;
  const uint16_t NUM_2 = SEGMENT_A | SEGMENT_B | SEGMENT_G | SEGMENT_E | SEGMENT_D;
  const uint16_t NUM_3 = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G;
  const uint16_t NUM_4 = SEGMENT_F | SEGMENT_G | SEGMENT_B | SEGMENT_C;
  const uint16_t NUM_5 = SEGMENT_A | SEGMENT_F | SEGMENT_G | SEGMENT_C | SEGMENT_D;
  const uint16_t NUM_6 = SEGMENT_A | SEGMENT_F | SEGMENT_E | SEGMENT_D | SEGMENT_C | SEGMENT_G;
  const uint16_t NUM_7 = SEGMENT_A | SEGMENT_B | SEGMENT_C;
  const uint16_t NUM_8 = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G;
  const uint16_t NUM_9 = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G;
  const uint16_t LET_A = SEGMENT_E | SEGMENT_F | SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_G;
  const uint16_t LET_C = SEGMENT_A | SEGMENT_F | SEGMENT_E | SEGMENT_D;
  const uint16_t LET_D = SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_G;
  const uint16_t LET_E = SEGMENT_A | SEGMENT_F | SEGMENT_E | SEGMENT_D | SEGMENT_G;
  const uint16_t LET_F = SEGMENT_E | SEGMENT_F | SEGMENT_A | SEGMENT_G;
  const uint16_t LET_H = SEGMENT_B | SEGMENT_C | SEGMENT_E | SEGMENT_F | SEGMENT_G;
  const uint16_t LET_N = SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_E | SEGMENT_F;
}

namespace FRAME_BUTTON
{
  const uint16_t POWER = 0x0400;
  const uint16_t FILTER = 0x0002;
  const uint16_t HEATER = 0x8000;
  const uint16_t BUBBLE = 0x0008;
  const uint16_t TEMP_UP = 0x1000;
  const uint16_t TEMP_DOWN = 0x0080;
  const uint16_t TEMP_UNIT = 0x2000;
  const uint16_t ALL = POWER | FILTER | HEATER | BUBBLE | TEMP_UP | TEMP_DOWN | TEMP_UNIT;
}

namespace FRAME_TYPE
{
  const uint16_t CUE = 0x0100;
  const uint16_t LED = 0x4000;
  const uint16_t DIGIT = FRAME_DIGIT::POS_ALL;
  const uint16_t BUTTON = CUE | FRAME_BUTTON::ALL;
}

namespace DIGIT
{
  const uint8_t POS_1 = 0x8;
  const uint8_t POS_2 = 0x4;
  const uint8_t POS_3 = 0x2;
  const uint8_t POS_4 = 0x1;
  const uint8_t POS_1_2 = POS_1 | POS_2;
  const uint8_t POS_1_2_3 = POS_1 | POS_2 | POS_3;
  const uint8_t POS_ALL = POS_1 | POS_2 | POS_3 | POS_4;

  const uint8_t LET_C = 0xC;
  const uint8_t LET_D = 0xD;
  const uint8_t LET_E = 0xE;
  const uint8_t LET_F = 0xF;
  const uint8_t LET_N = 0xA;
  const uint8_t OFF = 0xB;
};

namespace ERROR
{
  const uint16_t NONE = 0;
  const uint16_t NO_WATER_FLOW = 0xE90;
  const uint16_t WATER_TEMP_LOW = 0xE94;
  const uint16_t WATER_TEMP_HIGH = 0xE95;
  const uint16_t SYSTEM = 0xE96;
  const uint16_t DRY_FIRE_PROTECT = 0xE97;
  const uint16_t TEMP_SENSOR = 0xE99;
  const uint16_t HEATING_ABORTED = 0xEAD;

  const uint16_t VALUES[] = {NO_WATER_FLOW, WATER_TEMP_LOW, WATER_TEMP_HIGH, WATER_TEMP_HIGH, SYSTEM, DRY_FIRE_PROTECT, TEMP_SENSOR, HEATING_ABORTED};
  const unsigned int COUNT = sizeof(VALUES) / sizeof(uint16_t);

  const char CODE_90[] PROGMEM = "E90";
  const char CODE_94[] PROGMEM = "E94";
  const char CODE_95[] PROGMEM = "E95";
  const char CODE_96[] PROGMEM = "E96";
  const char CODE_97[] PROGMEM = "E97";
  const char CODE_99[] PROGMEM = "E99";
  const char CODE_END[] PROGMEM = "END";
  const char CODE_OTHER[] PROGMEM = "EXX";

  const char EN_90[] PROGMEM = "no water flow";
  const char EN_94[] PROGMEM = "water temp too low";
  const char EN_95[] PROGMEM = "water temp too high";
  const char EN_96[] PROGMEM = "system error";
  const char EN_97[] PROGMEM = "dry fire protection";
  const char EN_99[] PROGMEM = "water temp sensor error";
  const char EN_END[] PROGMEM = "heating aborted after 72h";
  const char EN_OTHER[] PROGMEM = "error";

  const char DE_90[] PROGMEM = "kein Wasserdurchfluss";
  const char DE_94[] PROGMEM = "Wassertemperatur zu niedrig";
  const char DE_95[] PROGMEM = "Wassertemperatur zu hoch";
  const char DE_96[] PROGMEM = "Systemfehler";
  const char DE_97[] PROGMEM = "Trocken-Brandschutz";
  const char DE_99[] PROGMEM = "Wassertemperatursensor defekt";
  const char DE_END[] PROGMEM = "Heizbetrieb nach 72 h deaktiviert";
  const char DE_OTHER[] PROGMEM = "Störung";

  const char *const TEXT[3][COUNT + 1] PROGMEM = {
      {CODE_90, CODE_94, CODE_95, CODE_96, CODE_97, CODE_99, CODE_END, CODE_OTHER},
      {EN_90, EN_94, EN_95, EN_96, EN_97, EN_99, EN_END, EN_OTHER},
      {DE_90, DE_94, DE_95, DE_96, DE_97, DE_99, DE_END, DE_OTHER}};
}

inline uint16_t display2Num(uint16_t v) { return (((v >> 12) & 0x000F) * 100) + (((v >> 8) & 0x000F) * 10) + ((v >> 4) & 0x000F); }
inline uint16_t display2Error(uint16_t v) { return (v >> 4) & 0x0FFF; }
inline bool displayIsTemp(uint16_t v) { return (v & 0x000F) == DIGIT::LET_C || (v & 0x000F) == DIGIT::LET_F; }
inline bool displayIsError(uint16_t v) { return (v & 0xF000) == 0xE000; }
inline bool displayIsBlank(uint16_t v) { return (v & 0xFFF0) == ((DIGIT::OFF << 12) + (DIGIT::OFF << 8) + (DIGIT::OFF << 4)); }

volatile SBH20IO::State SBH20IO::state;
volatile SBH20IO::Buttons SBH20IO::buttons;

void SBH20IO::setup(LANG language, uint8_t data_pin, uint8_t clock_pin, uint8_t latch_pin)
{
  this->language = language;
  this->pin_data_ = data_pin;
  this->pin_clock_ = clock_pin;
  this->pin_latch_ = latch_pin;

  pinMode(pin_clock_, INPUT);
  pinMode(pin_data_, INPUT);
  pinMode(pin_latch_, INPUT);

  s_data_pin = data_pin;
  s_latch_pin = latch_pin;
  attachInterruptArg(digitalPinToInterrupt(pin_latch_), SBH20IO::latchFallingISR, this, FALLING);
  attachInterruptArg(digitalPinToInterrupt(pin_clock_), SBH20IO::clockRisingISR, this, RISING);
}

void SBH20IO::loop()
{
  unsigned long now = millis();
  if (state.stateUpdated)
  {
    lastStateUpdateTime = now;
    state.online = true;
    state.stateUpdated = false;
  }
  else if (timeDiff(now, lastStateUpdateTime) > CYCLE::RECEIVE_TIMEOUT)
  {
    state.online = false;
  }
}

bool SBH20IO::isOnline() const { return state.online; }
unsigned int SBH20IO::getTotalFrames() const { return state.frameCounter; }
unsigned int SBH20IO::getDroppedFrames() const { return state.frameDropped; }

int SBH20IO::getCurrentTemperature() const
{
  return (state.currentTemperature != UNDEF::USHORT) ? convertDisplayToCelsius(state.currentTemperature) : UNDEF::USHORT;
}

int SBH20IO::getTargetTemperature() const
{
  return (state.targetTemperature != UNDEF::USHORT) ? convertDisplayToCelsius(state.targetTemperature) : UNDEF::USHORT;
}

void SBH20IO::forceReadTargetTemperature() { changeTargetTemperature(-1); }
unsigned int SBH20IO::getErrorValue() const { return state.error; }

String SBH20IO::getErrorMessage(unsigned int errorValue) const
{
  if (errorValue)
  {
    unsigned int i;
    for (i = 0; i < ERROR::COUNT; i++)
      if (ERROR::VALUES[i] == errorValue) break;
    return FPSTR(ERROR::TEXT[(unsigned int)language][i]);
  }
  return "";
}

unsigned int SBH20IO::getRawLedValue() const { return (state.ledStatus != UNDEF::USHORT) ? state.ledStatus : UNDEF::USHORT; }
uint8_t SBH20IO::isPowerOn() const { return (state.ledStatus != UNDEF::USHORT) ? ((state.ledStatus & FRAME_LED::POWER) != 0) : UNDEF::BOOL; }
uint8_t SBH20IO::isFilterOn() const { return (state.ledStatus != UNDEF::USHORT) ? ((state.ledStatus & FRAME_LED::FILTER) != 0) : UNDEF::BOOL; }
uint8_t SBH20IO::isBubbleOn() const { return (state.ledStatus != UNDEF::USHORT) ? ((state.ledStatus & FRAME_LED::BUBBLE) != 0) : UNDEF::BOOL; }
uint8_t SBH20IO::isHeaterOn() const { return (state.ledStatus != UNDEF::USHORT) ? ((state.ledStatus & (FRAME_LED::HEATER_ON | FRAME_LED::HEATER_STANDBY)) != 0) : UNDEF::BOOL; }
uint8_t SBH20IO::isHeaterStandby() const { return (state.ledStatus != UNDEF::USHORT) ? ((state.ledStatus & FRAME_LED::HEATER_STANDBY) != 0) : UNDEF::BOOL; }
uint8_t SBH20IO::isBuzzerOn() const { return (state.ledStatus != UNDEF::USHORT) ? ((state.ledStatus & FRAME_LED::NO_BEEP) == 0) : UNDEF::BOOL; }

void SBH20IO::setTargetTemperature(int temp)
{
  if (temp >= WATER_TEMP::SET_MIN && temp <= WATER_TEMP::SET_MAX)
  {
    if (isPowerOn() == true && state.error == ERROR::NONE)
    {
      int setTemp = getTargetTemperature();
      bool modifying = false;
      if (setTemp == UNDEF::USHORT)
      {
        changeTargetTemperature(-1);
        modifying = true;
        int sleep = 20;
        int tries = 4 * BLINK::PERIOD / sleep;
        do { delay(sleep); setTemp = getTargetTemperature(); tries--; } while (setTemp == UNDEF::USHORT && tries);
        if (setTemp == UNDEF::USHORT) { delay(1); return; }
      }
      int deltaTemp = temp - setTemp;
      while (deltaTemp)
      {
        if (deltaTemp > 0) { changeTargetTemperature(1); if (modifying) { deltaTemp--; setTemp++; } }
        else { changeTargetTemperature(-1); if (modifying) { deltaTemp++; setTemp--; } }
        modifying = true;
      }
      delay(1);
    }
  }
}

bool SBH20IO::pressButton(volatile unsigned int &buttonPressCount)
{
  waitBuzzerOff();
  unsigned int tries = BUTTON::ACK_TIMEOUT / BUTTON::ACK_CHECK_PERIOD;
  buttonPressCount = BUTTON::PRESS_COUNT;
  while (buttonPressCount && tries) { delay(BUTTON::ACK_CHECK_PERIOD); tries--; }
  return tries;
}

void SBH20IO::setBubbleOn(bool on) { if (on ^ (isBubbleOn() == true)) pressButton(buttons.toggleBubble); }
void SBH20IO::setFilterOn(bool on) { if (on ^ (isFilterOn() == true)) pressButton(buttons.toggleFilter); }
void SBH20IO::setHeaterOn(bool on) { if (on ^ (isHeaterOn() == true || isHeaterStandby() == true)) pressButton(buttons.toggleHeater); }
void SBH20IO::setPowerOn(bool on) { if (on ^ (isPowerOn() == true)) pressButton(buttons.togglePower); }

bool SBH20IO::waitBuzzerOff() const
{
  int tries = BUTTON::ACK_TIMEOUT / BUTTON::ACK_CHECK_PERIOD;
  while (state.buzzer && tries) { delay(BUTTON::ACK_CHECK_PERIOD); tries--; }
  if (tries) { delay(2 * CYCLE::PERIOD); return true; }
  else { DEBUG_MSG("\nwBO fail"); return false; }
}

bool SBH20IO::changeTargetTemperature(int up)
{
  if (isPowerOn() == true && state.error == ERROR::NONE)
  {
    waitBuzzerOff();
    int tries = BUTTON::ACK_TIMEOUT / BUTTON::ACK_CHECK_PERIOD;
    if (up > 0)
    {
      buttons.toggleTempUp = BUTTON::PRESS_COUNT;
      while (buttons.toggleTempUp && tries) { delay(BUTTON::ACK_CHECK_PERIOD); tries--; }
    }
    else if (up < 0)
    {
      buttons.toggleTempDown = BUTTON::PRESS_COUNT;
      while (buttons.toggleTempDown && tries) { delay(BUTTON::ACK_CHECK_PERIOD); tries--; }
    }
    if (tries && state.buzzer) return true;
    else { DEBUG_MSG("\ncWT fail"); return false; }
  }
  return false;
}

uint16_t SBH20IO::convertDisplayToCelsius(uint16_t value) const
{
  uint16_t celsiusValue = display2Num(value);
  uint16_t tempUint = value & 0x000F;
  if (tempUint == DIGIT::LET_F)
  {
    float fValue = (float)celsiusValue;
    celsiusValue = (uint16_t)round(((fValue - 32) * 5) / 9);
  }
  else if (tempUint != DIGIT::LET_C)
  {
    celsiusValue = UNDEF::USHORT;
  }
  return (celsiusValue >= 0) && (celsiusValue <= 60) ? celsiusValue : UNDEF::USHORT;
}

void IRAM_ATTR SBH20IO::latchFallingISR(void *arg)
{
  SBH20IO* self = static_cast<SBH20IO*>(arg);
  pinMode(self->pin_data_, INPUT);
}

void IRAM_ATTR SBH20IO::clockRisingISR(void *arg)
{
  SBH20IO* self = static_cast<SBH20IO*>(arg);
  static uint16_t frame = 0x0000;
  static uint16_t receivedBits = 0x0000;
  bool data = !digitalRead(self->pin_data_);
  bool enable = digitalRead(self->pin_latch_) == LOW;

  if (enable || receivedBits == (FRAME::BITS - 1))
  {
    frame = (frame << 1) + data;
    receivedBits++;

    if (receivedBits == FRAME::BITS)
    {
      state.frameCounter++;

      if (frame == FRAME_TYPE::CUE) { }
      else if (frame & FRAME_TYPE::DIGIT) decodeDisplay(frame);
      else if (frame & FRAME_TYPE::LED) decodeLED(frame);
      else if (frame & FRAME_TYPE::BUTTON) decodeButton(frame);

      receivedBits = 0;
    }
  }
  else
  {
    frame = 0;
    receivedBits = 0;
  }
}

inline uint8_t IRAM_ATTR SBH20IO::BCD(uint16_t value)
{
  switch (value & FRAME_DIGIT::SEGMENTS)
  {
  case FRAME_DIGIT::OFF:    return DIGIT::OFF;
  case FRAME_DIGIT::NUM_0:  return 0x0;
  case FRAME_DIGIT::NUM_1:  return 0x1;
  case FRAME_DIGIT::NUM_2:  return 0x2;
  case FRAME_DIGIT::NUM_3:  return 0x3;
  case FRAME_DIGIT::NUM_4:  return 0x4;
  case FRAME_DIGIT::NUM_5:  return 0x5;
  case FRAME_DIGIT::NUM_6:  return 0x6;
  case FRAME_DIGIT::NUM_7:  return 0x7;
  case FRAME_DIGIT::NUM_8:  return 0x8;
  case FRAME_DIGIT::NUM_9:  return 0x9;
  case FRAME_DIGIT::LET_C:  return DIGIT::LET_C;
  case FRAME_DIGIT::LET_D:  return DIGIT::LET_D;
  case FRAME_DIGIT::LET_E:  return DIGIT::LET_E;
  case FRAME_DIGIT::LET_F:  return DIGIT::LET_F;
  default:                  return DIGIT::LET_N;
  }
}

inline void IRAM_ATTR SBH20IO::decodeDisplay(uint16_t frame)
{
  static uint16_t value = 0;
  static uint16_t pValue = 0;
  static uint16_t stableValue = 0;
  static uint debounce = 0;
  static uint8_t largeDebounce = 0;
  static uint16_t stableTemp = 0x0000;

  uint8_t digit = BCD(frame);

  if (frame & FRAME_DIGIT::POS_1) value = (value & 0x0FFF) | (digit << 12);
  else if (frame & FRAME_DIGIT::POS_2) value = (value & 0xF0FF) | (digit << 8);
  else if (frame & FRAME_DIGIT::POS_3) value = (value & 0xFF0F) | (digit << 4);
  else if (frame & FRAME_DIGIT::POS_4)
  {
    value = (value & 0xFFF0) | digit;
    if (value != pValue) { pValue = value; debounce = 3; }
    else
    {
      if (debounce) debounce--;
      else if (value != stableValue)
      {
        largeDebounce = 250;
        stableValue = value;
        if (displayIsBlank(value)) { if (state.targetTemperature != stableTemp) { state.targetTemperature = stableTemp; state.stateUpdated = true; } }
        else if (displayIsError(value)) state.error = display2Error(value);
        else stableTemp = stableValue;
      }
      if (largeDebounce) largeDebounce--;
      else { if (state.currentTemperature != stableTemp) { state.currentTemperature = stableTemp; state.stateUpdated = true; } }
    }
  }
}

void IRAM_ATTR SBH20IO::decodeLED(uint16_t data) {
    // This is a stub to satisfy the linker. 
    // You can add LED logic here later.
}

inline void IRAM_ATTR SBH20IO::decodeButton(uint16_t frame)
{
  bool reply = false;
  if (frame & FRAME_BUTTON::FILTER) { if (buttons.toggleFilter) { reply = true; buttons.toggleFilter--; } }
  else if (frame & FRAME_BUTTON::HEATER) { if (buttons.toggleHeater) { reply = true; buttons.toggleHeater--; } }
  else if (frame & FRAME_BUTTON::BUBBLE) { if (buttons.toggleBubble) { reply = true; buttons.toggleBubble--; } }
  else if (frame & FRAME_BUTTON::POWER) { if (buttons.togglePower) { reply = true; buttons.togglePower--; } }
  else if (frame & FRAME_BUTTON::TEMP_UP) { if (buttons.toggleTempUp) { reply = true; buttons.toggleTempUp--; } }
  else if (frame & FRAME_BUTTON::TEMP_DOWN) { if (buttons.toggleTempDown) { reply = true; buttons.toggleTempDown--; } }

  if (reply)
    pinMode(s_data_pin, OUTPUT);  // use the static cached pin
}
