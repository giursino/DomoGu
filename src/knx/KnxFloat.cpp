/*
Copyright 2018 Giuseppe Ursino

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "KnxFloat.h"

const class DptKnxFloat {
public:
  class Range {
  public:
    const float max {float(670760.96)};
    const float min {float(-671088.64)};
  } range;

  class Mantissa {
  public:
    const uint8_t bitnum {11};
    const uint16_t bitmask {0x07FF};
    const uint16_t max {2048};
  } mantissa;

  class Exponent {
  public:
    const uint8_t bitnum {4};
  } exponent;

  const uint16_t invalid_value {0x7FFF};

} dpt_knx_float;


KnxFloat::KnxFloatType KnxFloat::IntToKnxFloat(const int32_t value_mult_100,
                                               const KnxFloat::ExponentCalculationMode exp_mode,
                                               const uint8_t exp_value) const
{
  int32_t value = value_mult_100;
  uint8_t sign = 0;
  uint8_t exp = 0;
  uint16_t tmp_exp = exp_value;
  KnxFloatType ret;

  if ((value > ( dpt_knx_float.range.max * 100))
      || (value < (dpt_knx_float.range.min * 100)))
  {
    ret.raw = dpt_knx_float.invalid_value;
    return ret;
  }

  if (value < 0) {
    // set negative sign
    sign = 1;
    // absolute value
    value = ~value + 1;
    // two-complement
    if (value < 0) {
      value = ~value;
    }
  }

  if (exp_mode == ExponentCalculationMode::Automatic) {
    while (value > ((1 << dpt_knx_float.mantissa.bitnum) - 1)) {
      // mantissa rescale
      uint8_t round = 0;
      if(value & 0x01) {
        round = 1;
      }
      value >>= 1;
      value += round;
      // exponent incrementation
      exp++;
    }
  }
  else {
    while (tmp_exp--) {
      // mantissa rescale
      value >>= 1;
    }
    exp = exp_value;
  }

  if ((exp) > ((1 << dpt_knx_float.exponent.bitnum) - 1)) {
    // ERROR: invalid number (too big)
    ret.raw = dpt_knx_float.invalid_value;
  }
  else {
    if (sign) {
      // one-complement
      value = (~value + 1) & dpt_knx_float.mantissa.bitmask;
    }

    ret.s = sign;
    ret.e = exp;
    ret.m = static_cast<uint16_t> (value);
  }

  return ret;
}


int32_t KnxFloat::FloatToInt(const float value) const
{
  if (value >= 0)
    return static_cast<int32_t> (value + float(0.5));
  else
    return static_cast<int32_t> (value - float(0.5));
}


float KnxFloat::KnxFloatToFloat(const KnxFloatType value) const
{
  uint8_t sign = 0;
  KnxFloatType value_in = value;
  int32_t value_out = 0;

  if (value_in.s) {
    sign = 1;
    // one-complement
    value_in.m = (~(value_in.m) + 1);
  }
  value_out = static_cast<int32_t> (value_in.m);
  if ((value_out == 0) && (sign == 1)) {
    // one-complement, if 0 not manged here
    value_out = dpt_knx_float.mantissa.max;
  }

  while (value_in.e) {
    // mantissa rescale
    value_out <<= 1;
    value_in.e--;
  }

  if (sign) {
    value_out = -value_out;
  }

  return (static_cast<float> (value_out) / float(100.0));;
}


KnxFloat::KnxFloat(int32_t value_mult_100)
{
    m_value = IntToKnxFloat(value_mult_100);
}


KnxFloat::KnxFloat(int32_t value_mult_100, uint8_t exp)
{
  m_value = IntToKnxFloat(value_mult_100, ExponentCalculationMode::Fixed, exp);
}


KnxFloat::KnxFloat(float value)
{
  m_value = IntToKnxFloat(FloatToInt((value * float(100))));
}


KnxFloat::KnxFloat(uint16_t raw_value)
{
  m_value.raw = raw_value;
}


KnxFloat::KnxFloat(std::vector<uint8_t> raw_value)
{
  m_value.raw = static_cast<uint16_t>(raw_value[0] << 8) + raw_value[1];
}


float KnxFloat::GetFloat() const
{
  return KnxFloatToFloat(m_value);
}


uint16_t KnxFloat::GetRaw() const
{
  return m_value.raw;
}
