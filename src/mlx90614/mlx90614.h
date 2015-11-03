/*
 * Arduino driver for MLX90614 infrared temperature sensor.
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

#ifndef __MLX90614_H__
#define __MLX90614_H__

#include "Arduino.h"


#define MLX90614_ADDRESS  (0x5a)
#define MLX90614_READ_TEMPERATURE  (0x07)


class MLX90614
{
public:
  MLX90614(void);
  
  void begin(uint8_t sdaPin, uint8_t sclPin);
  uint16_t readRaw(void);
  float read(void) { return 0.02*(float)readRaw() - 273.15; }   
  
private:
  uint8_t _sda;
  uint8_t _scl;
  uint8_t _crc;
  void crcUpdate(uint8_t value);
};


#endif /* __MLX90614_H__ */
