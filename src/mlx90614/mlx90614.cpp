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

#include "mlx90614.h"
#include <Wire.h>


const uint8_t crcTable[256] =
{
    0,   7,  14,   9,  28,  27,  18,  21,  56,  63,  54,  49,  36,  35,  42,  45,
  112, 119, 126, 121, 108, 107,  98, 101,  72,  79,  70,  65,  84,  83,  90,  93,
  224, 231, 238, 233, 252, 251, 242, 245, 216, 223, 214, 209, 196, 195, 202, 205,
  144, 151, 158, 153, 140, 139, 130, 133, 168, 175, 166, 161, 180, 179, 186, 189,
  199, 192, 201, 206, 219, 220, 213, 210, 255, 248, 241, 246, 227, 228, 237, 234,
  183, 176, 185, 190, 171, 172, 165, 162, 143, 136, 129, 134, 147, 148, 157, 154,
   39,  32,  41,  46,  59,  60,  53,  50,  31,  24,  17,  22,   3,   4,  13,  10,
   87,  80,  89,  94,  75,  76,  69,  66, 111, 104,  97, 102, 115, 116, 125, 122,
  137, 142, 135, 128, 149, 146, 155, 156, 177, 182, 191, 184, 173, 170, 163, 164,
  249, 254, 247, 240, 229, 226, 235, 236, 193, 198, 207, 200, 221, 218, 211, 212,
  105, 110, 103,  96, 117, 114, 123, 124,  81,  86,  95,  88,  77,  74,  67,  68,
   25,  30,  23,  16,   5,   2,  11,  12,  33,  38,  47,  40,  61,  58,  51,  52,
   78,  73,  64,  71,  82,  85,  92,  91, 118, 113, 120, 127, 106, 109, 100,  99,
   62,  57,  48,  55,  34,  37,  44,  43,   6,   1,   8,  15,  26,  29,  20,  19,
  174, 169, 160, 167, 178, 181, 188, 187, 150, 145, 152, 159, 138, 141, 132, 131,
  222, 217, 208, 215, 194, 197, 204, 203, 230, 225, 232, 239, 250, 253, 244, 243
};


MLX90614::MLX90614(void)
{
  _sda = 0;
  _scl = 0;
  _crc = 0;
}


void MLX90614::begin(uint8_t sdaPin, uint8_t sclPin)
{
  _sda = sdaPin;
  _scl = sclPin;
  pinMode(_sda,INPUT_PULLUP);
  pinMode(_scl,INPUT_PULLUP);
  Wire.begin(MLX90614_ADDRESS);
}


void MLX90614::crcUpdate(uint8_t value)
{ 
  _crc = crcTable[value^_crc];
}


uint16_t MLX90614::readRaw(void) 
{ 
  uint16_t value = 0; 
  uint8_t pec = 0; 
  uint8_t lsb; 
  uint8_t msb; 

  // Read temperature register.
  Wire.beginTransmission(MLX90614_ADDRESS); 
  Wire.write(MLX90614_READ_TEMPERATURE); 
  // Restart without sending a stop condition.
  Wire.endTransmission(false); 
  Wire.requestFrom(MLX90614_ADDRESS,3,true); 
  if (Wire.available()!=0) lsb = Wire.read();
  if (Wire.available()!=0) msb = Wire.read();
  if (Wire.available()!=0) pec = Wire.read();
  Wire.endTransmission(true); 

  _crc = 0;
  crcUpdate(MLX90614_ADDRESS<<1);
  crcUpdate(MLX90614_READ_TEMPERATURE);
  crcUpdate((MLX90614_ADDRESS<<1)|0x01);
  crcUpdate(lsb);
  crcUpdate(msb);
  if (pec==_crc) value = ((msb&0x7f)<<8) + lsb;
  return value; 
}   
