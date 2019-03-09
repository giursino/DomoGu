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

#ifndef KNXFLOAT_H_
#define KNXFLOAT_H_

#include <cstdint>
#include <vector>


class KnxFloat
{
public:

  /**
   * @brief KnxFloat Create a KnxFloat from fixed-point integer. Let the exponent automatic calculated.
   * @param value_mult_100 fixed-point integer (value multiply 100)
   * Example:
   *  12.5   ->  1250
   *  23     ->  2300
   *  -4.791 ->  -479
   *  1.4567 ->   146
   *  0.01   ->     1
   */
  KnxFloat(int32_t value_mult_100);

  /**
  * @brief KnxFloat Create a KnxFloat from fixed-point integer. Set exponent manually.
  * @param value_mult_100 fixed-point integer (value multiply 100)
  */
  KnxFloat(int32_t value_mult_100, uint8_t exp);

  /**
   * @brief KnxFloat Create a KnxFloat from float. Let the exponent automatic calculated.
   * @param value float value
   */
  KnxFloat(float value);

  /**
   * @brief KnxFloat Create a KnxFloat from raw value.
   * @param value raw value
   */
  KnxFloat(uint16_t raw_value);

  /**
   * @brief KnxFloat Create a KnxFloat from raw value.
   * @param value raw value
   */
  KnxFloat(std::vector<std::uint8_t> raw_value);

  /**
   * @brief GetFloat
   * @return float value
   */
  float GetFloat() const;

  /**
   * @brief GetRaw
   * @return raw value
   */
  uint16_t GetRaw() const;

private:

  /**
   * Format: 2 octets F_16;
   * Octect nb:
   *      2_MSB,              1_LSB
   * [M E E E E M M M]   [M M M M M M M M]
   * FloatValue = (0.01*M)*2^E
   * E = [0..15]
   * M = [-2048 .. 2047]
   * For all Datapoint Types 9.0xx, the encoded value 7FFFh shall always be used
   * to denote invalid data.
  */
#pragma pack(1)
  typedef union {
    struct{
      uint16_t m :11;
      uint16_t e :4;
      uint16_t s :1;
    };
    uint16_t raw;
  } KnxFloatType;
#pragma pack()

  KnxFloatType m_value;

  enum class ExponentCalculationMode {
    Automatic,
    Fixed
  };

  /**
   * @brief FloatToInt Convert float value to integer without lost approximation.
   * @param value float value
   * @return integer value
   */
  int32_t FloatToInt(const float value) const;

  /**
   * @brief IntToKnxFloat Convert integer multiply 100 to KnxFloat.
   * @param value_mult_100 fixed-point number (integer multiply 100)
   * @param exp_mode The way how it is calculated the exponent (automatic/manual)
   * @param exp_value The exponent value when it is use automatic calculation
   * @return
   */
  KnxFloatType IntToKnxFloat(const int32_t value_mult_100,
                             const ExponentCalculationMode exp_mode = ExponentCalculationMode::Automatic,
                             const uint8_t exp_value = 0) const;

  /**
   * @brief KnxFloatToFloat Convert KNX floating (PDT_KNX_FLOAT) to float.
   * @param value PDT_KNX_FLOAT value
   * @return float value
   */
  float KnxFloatToFloat(const KnxFloatType value) const;
};


#endif // KNXFLOAT_H_
