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
  hasDigitalIn0     = 0x00000001, // RX, pin 0
  hasDigitalOut0    = 0x00000002, // pin 0
  hasDigitalIn1     = 0x00000004, // pin 1
  hasDigitalOut1    = 0x00000008, // TX, pin 1
  hasLcd            = 0x00000010, // LCD1, pin 2,3,4,5,6,7
  hasRcDetector     = 0x00000020, // IC1, pin 8
  hasPushbutton1    = 0x00000040, // S1, pin 9
  hasTransistor2    = 0x00000080, // T2, pin 9
  hasPushbutton2    = 0x00000100, // S2, pin 10
  hasTransistor1    = 0x00000200, // T1, pin 10
  hasLed1           = 0x00000400, // LED1, pin 11
  hasBuzzer         = 0x00000800, // BUZ1, pin 11
  hasThermometer    = 0x00001000, // IC2, pin 12
  hasLed2           = 0x00002000, // LED2, pin 13
  // Analog inputs.
  hasLightSensor    = 0x00004000, // LDR1, A0
  hasMicrophone     = 0x00008000, // MIC1, A0
  hasPressureSensor = 0x00010000, // IC3, A1
  hasAnalogIn       = 0x00020000, // pin A2
  hasPotentiometer  = 0x00040000, // P1, A3
  // I2C and friends.
  hasHumiditySensor = 0x00080000, // IC4, A4,A5
  hasIrThermometer  = 0x00100000, // IC5, A4,A5
};


enum multipurposeShieldPeripheralPins
{
  // Digital IO.
  pinDigitalIn0 = 0,
  pinDigitalOut0 = 0,
  pinDigitalIn1 = 1,
  pinDigitalOut1 = 1,
  pinRcDetector = 8,
  pinPushbutton1 = 9,
  pinTransistor2 = 9,
  pinPushbutton2 = 10,
  pinTransistor1 = 10,
  pinLed1 = 11,
  pinBuzzer = 11,
  pinThermometer = 12,
  pinLed2 = 13,
  // Analog inputs.
  pinLightSensor = 14,
  pinMicrophone = 14,
  pinPressureSensor = 15,
  pinAnalogIn = 16,
  pinPotentiometer = 17,
  // LCD
  pinLcdRs = 6,
  pinLcdRw = 255,
  pinLcdE = 7,
  pinLcdD4 = 5,
  pinLcdD5 = 4,
  pinLcdD6 = 3,
  pinLcdD7 = 2,
};


class MultipurposeShield
{
public:
  MultipurposeShield(uint32_t peripherals);

  void begin(void);

  // Pressure sensor IC3 MPX4115.
  int16_t pressureSensorRead(int16_t offset=0);

  // Humidity sensor IC4 SHT11.
  boolean humiditySensorRead(void);
  float humiditySensorReadRh(void);
  float humiditySensorReadT(void);
  float humiditySensorReadDewPoint(void);

  // Light sensor LDR1.
  int16_t lightSensorRead(boolean asPercentage=true);

  // Digital outputs (unchecked).
  inline void digitalOut0Write(uint8_t value) { digitalWrite(pinDigitalOut0,value); }
  inline void digitalOut1Write(uint8_t value) { digitalWrite(pinDigitalOut1,value); }

  // Digital inputs (unchecked).
  inline uint8_t digitalIn0Read(void) { digitalRead(pinDigitalIn0); }
  inline uint8_t digitalIn1Read(void) { digitalRead(pinDigitalIn1); }
  
  // LEDs.
  void led1Write(uint8_t value) { digitalWriteChecked(hasLed1,pinLed1,value); }
  void led2Write(uint8_t value) { digitalWriteChecked(hasLed2,pinLed2,value); }
  
  // Transistors.
  void transistor1Write(uint8_t value) { digitalWriteChecked(hasTransistor1,pinTransistor1,value); }
  void transistor2Write(uint8_t value) { digitalWriteChecked(hasTransistor2,pinTransistor2,value); }

  // Pushbuttons.
  uint8_t pushbutton1Read(void) { return digitalReadChecked(hasPushbutton1,pinPushbutton1); }
  uint8_t pushbutton2Read(void) { return digitalReadChecked(hasPushbutton2,pinPushbutton2); }
  
  LiquidCrystal lcd;
  SHT1x sht11;

private:
  uint32_t _capabilities;

  void digitalWriteChecked(uint32_t capability, uint8_t pin, uint8_t value);
  int8_t digitalReadChecked(uint32_t capability, uint8_t pin);
};


#endif /* __MULTIPURPOSESHIELD_H__ */
