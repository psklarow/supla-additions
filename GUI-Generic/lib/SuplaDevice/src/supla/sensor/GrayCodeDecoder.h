/*

AUTHOR/LICENSE:

Copyright (c) 2025 Pawel Sklarow https://github.com/psklarow

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

DESCRIPTION:

Gray code scanner / decoder.
Could be used for decoding e.g. wind direction, device angle, disk rotation level
Visible in Supla as General Purpose Measurement, for easiness of configuration.
Supports up to 8 bit decoders.

 */

#ifndef _GRAY_CODE_DECODER_H_
#define _GRAY_CODE_DECODER_H_

#include <Arduino.h>
#include <supla/sensor/general_purpose_measurement.h>

namespace Supla {
namespace Sensor {

#define MAX_GRAY_DECODER_BITS 8
typedef uint8_t GrayCodeValueType;
static_assert(MAX_GRAY_DECODER_BITS <= sizeof(GrayCodeValueType)*8); // assure no longer bits than type allows

class GrayCodeDecoder {
 public:
  /** Creates a decoder.
   * @pins list of pins to read. Least significant bit first.
   * @pinCount count of pins to read
   */
  GrayCodeDecoder(uint8_t pinCount, const uint8 pins[], bool invert=false);

  /** Read input pins, convert from Gray Code, return it as number. */
  unsigned int getValue();

 private:
  bool _isError;
  uint8_t _pinCount;
  uint8_t _pins[MAX_GRAY_DECODER_BITS];
  bool _invert;
};


class GrayCodeSensor : public GeneralPurposeMeasurement {
public:
  GrayCodeSensor(uint8_t pinCount, uint8 pins[]): _decoder(pinCount, pins){}
  virtual double getValue() {
    return _decoder.getValue();
  }

private:
  GrayCodeDecoder _decoder;
};

}  // namespace Sensor
}  // namespace Supla

#endif  // _GRAY_CODE_DECODER_H_
