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
