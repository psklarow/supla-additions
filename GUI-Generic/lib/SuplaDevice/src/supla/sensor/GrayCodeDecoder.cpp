/*

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

 */


#include "GrayCodeDecoder.h"

#ifdef __DEBUG__
#define log_debug(...) { Serial.printf(__FILE__ " "); printf(__VA_ARGS__); }
#else // __DEBUG__
#define log_debug(...) {}
#endif // __DEBUG__

#define log_status(...) { Serial.printf(__FILE__ " :"); Serial.printf(__VA_ARGS__); }
#define log_error(...) { Serial.printf(__FILE__ " :"); Serial.printf(__VA_ARGS__); }

namespace Supla {
namespace Sensor {

GrayCodeDecoder::GrayCodeDecoder(uint8_t pinCount, const uint8 pins[], bool invert) 
  : _isError(false), _pinCount(0) {
    log_status("Hello from GrayCodeDecoder! (pinCount %d)\n", pinCount);
    if (pinCount >= MAX_GRAY_DECODER_BITS) {
      log_error("Too many bits %d. Max is 8.\n", pinCount);
      _isError = true;
      return;
    }

    for (size_t i = 0; i < pinCount; i++) {
      log_status("D%d is IO%d\n", i, pins[i]);
      _pins[i] = pins[i];
      pinMode(pins[i], INPUT_PULLUP); // set pin as input
    }
    _pinCount = pinCount;
    _invert = invert;
}

unsigned int GrayCodeDecoder::getValue() {
  if (_isError) {
      log_error("GrayCodeDecoder: getValue error");
      return 42.0;
  }
  else {
      // read pins into one number, LS bit first
      GrayCodeValueType num = 0;
      for (size_t i=0; i<_pinCount; i++) {
          auto pin = _pins[i];
          bool pinValue = false;
#ifdef ARDUINO_ARCH_ESP8266
          if (pin == A0) {
            int reading = analogRead(pin);
            const int VALUE_977mV = 1000;     // 977 millivolts, assuming 1V = 1024
            pinValue = reading > VALUE_977mV; // anything above 977 mV means 'high'

            log_debug("Analog: %d %d\n", reading, pinValue);
          }
          else 
#endif
          {
            pinValue = digitalRead(pin);
          }
          if (pinValue) {
            log_debug("pin %d io[%d] is 1\n\r", i, _pins[i]);

            num += 1<<i;
          }
          else {
            log_debug("pin %d io[%d] is 0\n\r", i, _pins[i]);
          }
      }

      // convert from Gray Code (https://en.wikipedia.org/wiki/Gray_code)
      auto mask = num;
      while (mask) {
          mask >>= 1;
          num ^= mask;
      }
      log_debug("GrayCodeDecoder::getValue() = %d\n", num);
      return num;
  }
}

}  // namespace Sensor
}  // namespace Supla
