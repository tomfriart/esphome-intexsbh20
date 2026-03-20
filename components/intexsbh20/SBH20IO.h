/*
 * project:  Intex PureSpa SB-H20 WiFi Controller
 *
 * file:     SBH20IO.h
 *
 * encoding: UTF-8
 * created:  14th March 2021
 *
 * Copyright (C) 2021 Jens B.
 *
 * SPDX-License-Identifier: CC-BY-NC-SA-4.0
 */

#ifndef SBH20IO_H
#define SBH20IO_H

#include <WString.h>
#include <Arduino.h>
#include "common.h"

class SBH20IO
{
public:
  class UNDEF
  {
  public:
    static const uint8_t BOOL = 0xFF;
    static const uint16_t USHORT = 0xFFFF;
  };

  class WATER_TEMP
  {
  public:
    static const int SET_MIN = 20;
    static const int SET_MAX = 40;
  };

public:
  void setup(LANG language, uint8_t data_pin, uint8_t clock_pin, uint8_t latch_pin);
  void loop();

public:
  bool isOnline() const;

  int getCurrentTemperature() const;
  int getTargetTemperature() const;
  void forceReadTargetTemperature();

  uint8_t isBubbleOn() const;
  uint8_t isFilterOn() const;
  uint8_t isHeaterOn() const;
  uint8_t isHeaterStandby() const;
  uint8_t isPowerOn() const;
  uint8_t isBuzzerOn() const;

  void setTargetTemperature(int temp);

  void setBubbleOn(bool on);
  void setFilterOn(bool on);
  void setHeaterOn(bool on);
  void setPowerOn(bool on);

  unsigned int getErrorValue() const;
  String getErrorMessage(unsigned int errorValue) const;

  unsigned int getRawLedValue() const;

  unsigned int getTotalFrames() const;
  unsigned int getDroppedFrames() const;

  // pin accessors needed by ISRs via arg pointer
  uint8_t get_data_pin() const { return pin_data_; }
  uint8_t get_clock_pin() const { return pin_clock_; }
  uint8_t get_latch_pin() const { return pin_latch_; }

private:
  class CYCLE
  {
  public:
    static const unsigned int TOTAL_FRAMES = 32;
    static const unsigned int DISPLAY_FRAMES = 5;
    static const unsigned int PERIOD = 21;
    static const unsigned int RECEIVE_TIMEOUT = 50 * CYCLE::PERIOD;
  };

  class FRAME
  {
  public:
    static const unsigned int BITS = 16;
    static const unsigned int FREQUENCY = CYCLE::TOTAL_FRAMES / CYCLE::PERIOD;
  };

  class BLINK
  {
  public:
    static const unsigned int PERIOD = 500;
    static const unsigned int TEMP_FRAMES = PERIOD / 4 * FRAME::FREQUENCY;
    static const unsigned int STOPPED_FRAMES = 2 * PERIOD * FRAME::FREQUENCY;
  };

  class BUTTON
  {
  public:
    static const unsigned int PRESS_COUNT = BLINK::PERIOD / CYCLE::PERIOD - 2;
    static const unsigned int ACK_CHECK_PERIOD = 10;
    static const unsigned int ACK_TIMEOUT = 2 * PRESS_COUNT * CYCLE::PERIOD;
  };

private:
  struct State
  {
    uint16_t currentTemperature = UNDEF::USHORT;
    uint16_t targetTemperature = UNDEF::USHORT;
    uint16_t ledStatus = UNDEF::USHORT;

    bool buzzer = false;
    uint16_t error = 0;
    unsigned int lastErrorChang
