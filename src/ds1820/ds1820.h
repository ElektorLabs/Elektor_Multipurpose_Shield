/*
 * Arduino driver for DS1820 one-wire temperature sensor.
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

#ifndef __DS1820_H__
#define __DS1820_H__

#include "Arduino.h"


// On the multipurpose shield the chip is connected to PB4 (Arduino pin 12)
#define DS1820_DQ_DDR  DDRB
#define DS1820_DQ_PORT  PORTB
#define DS1820_DQ_PIN  PINB
#define DS1820_DQ_BIT  4


#define DS1820_SCRATCHPAD_SIZE  9


class DS1820
{
public:
  DS1820(void) {}
  boolean reset(void);
  float read(void);

private:
  uint8_t _scratchpad[DS1820_SCRATCHPAD_SIZE];
  uint8_t timeSlot(uint8_t value);
  void writeByte(uint8_t value);
  uint8_t readByte(void);
};


#endif /* __DS1820_H__ */