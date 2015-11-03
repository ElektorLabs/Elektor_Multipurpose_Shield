/*
 * Library for use with PolyValens Multipurpose Shield 129009-1
 *
 * Belongs to:
 * "Mastering Microcontrollers Helped by Arduino"
 * ISBN 978-1-907920-23-3 (English)
 * ISBN 978-2-86661-190-3 (French)
 * ISBN 978-3-89576-296-3 (German)
 * http://www.polyvalens.com/
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

//#define __CELSIUS__
//#define __USE_CRC__

#ifndef __MULTIPURPOSESHIELD_H__
#define __MULTIPURPOSESHIELD_H__


#include "LiquidCrystal\LiquidCrystal.h"
#include "SHT1x\SHT1x.h"



enum multipurposeShieldPeripherals
{
  // Digital IO.
  kDigitalIn0  = 0x00000001, // RX,   pin 0
  kDigitalOut0 = 0x00000002, //       pin 0
  kDigitalIn1  = 0x00000004, //       pin 1
  kDigitalOut1 = 0x00000008, // TX,   pin 1
  kLcd         = 0x00000010, // LCD1, pin 2,3,4,5,6,7
  kRemote      = 0x00000020, // IC1,  pin 8
  kPushbutton1 = 0x00000040, // S1,   pin 9
  kTransistor2 = 0x00000080, // T2,   pin 9
  kPushbutton2 = 0x00000100, // S2,   pin 10
  kTransistor1 = 0x00000200, // T1,   pin 10
  kLed1        = 0x00000400, // LED1, pin 11
  kBuzzer      = 0x00000800, // BUZ1, pin 11
  kTemperature = 0x00001000, // IC2,  pin 12
  kLed2        = 0x00002000, // LED2, pin 13
  // Analog inputs.
  kLight       = 0x00004000, // LDR1, A0
  kSound       = 0x00008000, // MIC1, A0
  kPressure    = 0x00010000, // IC3,  A1
  kAnalogIn    = 0x00020000, // pin   A2
  kTrimmer     = 0x00040000, // P1,   A3
  // I2C and friends.
  kHumidity    = 0x00080000, // IC4,  A4,A5
  kIr          = 0x00100000, // IC5,  A4,A5
};


enum multipurposeShieldPeripheralPins
{
  // Digital IO.
  kPinDigitalIn0  = 0,
  kPinDigitalOut0 = 0,
  kPinDigitalIn1  = 1,
  kPinDigitalOut1 = 1,
  kPinRemote      = 8,
  kPinPushbutton1 = 9,
  kPinTransistor2 = 9,
  kPinPushbutton2 = 10,
  kPinTransistor1 = 10,
  kPinLed1        = 11,
  kPinBuzzer      = 11,
  kPinTemperature = 12,
  kPinLed2        = 13,
  // Analog inputs.
  kPinLight       = 14,
  kPinSound       = 14,
  kPinPressure    = 15,
  kPinAnalogIn    = 16,
  kPinTrimmer     = 17,
  // LCD
  kPinLcdRs = 6,
  kPinLcdRw = 255,
  kPinLcdE = 7,
  kPinLcdD4 = 5,
  kPinLcdD5 = 4,
  kPinLcdD6 = 3,
  kPinLcdD7 = 2,
};


class MultipurposeShield
{
public:
  MultipurposeShield(uint32_t peripherals);

  void begin(void);

  // Humidity sensor IC4 SHT11.
  boolean humiditySensorRead(void);
  float humiditySensorReadHumidity(void);
  float humiditySensorReadTemperature(void);
  float humiditySensorReadDewPoint(void);

  // Pressure sensor IC3 MPX4115A
  int16_t pressureSensorRead(int16_t offset=0);

  LiquidCrystal lcd;
  SHT1x sht11;

private:
  uint32_t _capabilities;
};


#endif /* __MULTIPURPOSESHIELD_H__ */
