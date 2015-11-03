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

#include "ds1820.h"


#define DS1820_DQ_LO DS1820_DQ_DDR |= _BV(DS1820_DQ_BIT); \
                     DS1820_DQ_PORT &= ~_BV(DS1820_DQ_BIT)

#define DS1820_DQ_HI DS1820_DQ_PORT |= _BV(DS1820_DQ_BIT); \
                     DS1820_DQ_DDR &= ~_BV(DS1820_DQ_BIT)

#define DS1820_DQ_IN (DS1820_DQ_PIN & _BV(DS1820_DQ_BIT))


boolean DS1820::reset(void)
{
  boolean presence = false;
  DS1820_DQ_LO;
  delayMicroseconds(500);
  DS1820_DQ_HI;
  int timeout = 480;
  int dt = 30;
  while (timeout>0)
  {
    delayMicroseconds(dt);
    timeout -= dt;
    if (DS1820_DQ_IN==0)
    {
      presence = true;
      break;
    }
  }
  // Finish timeout.
  delayMicroseconds(timeout);
  return presence;
}


uint8_t DS1820::timeSlot(uint8_t value)
{
  uint8_t result = 0;
  DS1820_DQ_LO;
  delayMicroseconds(2);
  if (value!=0) DS1820_DQ_HI;
  delayMicroseconds(10);
  if (DS1820_DQ_IN!=0) result = 1;
  delayMicroseconds(50);
  DS1820_DQ_HI;
  return result;
}


void DS1820::writeByte(uint8_t value)
{
  for (uint8_t mask=0x01; mask!=0; mask<<=1)
  {
    timeSlot(value&mask);
  }
}


uint8_t DS1820::readByte(void)
{
  uint8_t result = 0;
  for (uint8_t mask=0x01; mask!=0; mask<<=1)
  {
    if (timeSlot(1)!=0) result |= mask;
  }
  return result;
}


float DS1820::read(void)
{
  float result = 0.0;
  if (reset()==true)
  {
    writeByte(0xcc);
    writeByte(0x44);
    while (timeSlot(1)==0);
    if (reset()==true)
    {
      writeByte(0xcc);
      writeByte(0xbe);
      for (int i=0; i<DS1820_SCRATCHPAD_SIZE; i++)
      {
        _scratchpad[i] = readByte();
      }
      reset();
      result = (_scratchpad[1]*256.0 + _scratchpad[0])/16.0;
    }
  }
  return result;
}
