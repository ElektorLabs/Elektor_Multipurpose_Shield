/*
 * Arduino driver for SHT1x temperature & relative humidity sensor.
 * Handles CRC (optional).
 *
 * Belongs to:
 * "Mastering Microcontrollers Helped by Arduino"
 * ISBN 978-1-907920-23-3 (English)
 * ISBN 978-2-86661-190-3 (French)
 * ISBN 978-3-89576-296-3 (German)
 * http://www.polyvalens.com/
 *
 * For use with PolyValens Multipurpose Shield 129009-1
 *
 * Copyright (c) 2015, Clemens Valens
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
 * BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#ifndef __SHT1x_H__
#define __SHT1x_H__

#include "Arduino.h"


//#define __USE_CRC__


#ifdef __USE_CRC__
static const uint8_t crc_table[256] =
{
    0,  49,  98,  83, 196, 245, 166, 151, 185, 136, 219, 234, 125,  76,  31,  46,
   67, 114,  33,  16, 135, 182, 229, 212, 250, 203, 152, 169,  62,  15,  92, 109,
  134, 183, 228, 213,  66, 115,  32,  17,  63,  14,  93, 108, 251, 202, 153, 168,
  197, 244, 167, 150,   1,  48,  99,  82, 124,  77,  30,  47, 184, 137, 218, 235,
   61,  12,  95, 110, 249, 200, 155, 170, 132, 181, 230, 215,  64, 113,  34,  19,
  126,  79,  28,  45, 186, 139, 216, 233, 199, 246, 165, 148,   3,  50,  97,  80,
  187, 138, 217, 232, 127,  78,  29,  44,   2,  51,  96,  81, 198, 247, 164, 149,
  248, 201, 154, 171,  60,  13,  94, 111,  65, 112,  35,  18, 133, 180, 231, 214,
  122,  75,  24,  41, 190, 143, 220, 237, 195, 242, 161, 144,   7,  54, 101,  84,
   57,   8,  91, 106, 253, 204, 159, 174, 128, 177, 226, 211,  68, 117,  38,  23,
  252, 205, 158, 175,  56,   9,  90, 107,  69, 116,  39,  22, 129, 176, 227, 210,
  191, 142, 221, 236, 123,  74,  25,  40,   6,  55, 100,  85, 194, 243, 160, 145,
   71, 118,  37,  20, 131, 178, 225, 208, 254, 207, 156, 173,  58,  11,  88, 105,
    4,  53, 102,  87, 192, 241, 162, 147, 189, 140, 223, 238, 121,  72,  27,  42,
  193, 240, 163, 146,   5,  52, 103,  86, 120,  73,  26,  43, 188, 141, 222, 239,
  130, 179, 224, 209,  70, 119,  36,  21,  59,  10,  89, 104, 255, 206, 157, 172
};

class SHT1x_crc
{
public:
  uint8_t get(void) { return _crc; }
  void set(uint8_t value) { _crc = value; }
  void reset(void) { set(0); }
  void update(uint8_t value) { _crc = crc_table[value^_crc]; }

  // Adapted from http://graphics.stanford.edu/~seander/bithacks.html
  uint8_t bit_reverse(uint8_t value)
  {
    uint8_t result = value;
    uint8_t i = 7;
    for (value>>=1; value; value>>=1)
    {   
      result <<= 1;
      result |= value & 1;
      i--;
    }
    return result <<= i;
  }

private:
  uint8_t _crc;
};
#endif /* __USE_CRC__ */


class SHT1x
{
public:
  SHT1x(void) {}
  SHT1x(uint8_t data, uint8_t sck, boolean disable_twi) { begin(data,sck,disable_twi); }

  void begin(uint8_t data, uint8_t sck, boolean disable_twi=false);
  void update(void);

  float get_temperature(void) { return _temperature; }
  float get_humidity(void) { return _humidity; } 
  float get_dewpoint(void);

  void start_sequence(void);
  void connection_reset(void);
  boolean soft_reset(void);

private:
  uint8_t _data;
  uint8_t _sck;
  float _temperature;
  float _humidity;

  void strobe(void);
  boolean send_byte(uint8_t value);
  uint8_t receive_byte(uint8_t ack);
  boolean status_register_read(uint8_t& result, uint8_t& crc2);
  boolean status_register_write(uint8_t value);
  boolean wait_for_measurement(void);
  boolean measure(int& result, uint8_t& crc2, uint8_t command);
  void calculcate(void);

#ifdef __USE_CRC__
  SHT1x_crc crc;
#endif /* __USE_CRC__ */
};


#endif /* __SHT1x_H__ */

