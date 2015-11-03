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


#include "MultipurposeShield.h"


#define has_peripheral(a)  (_capabilities&(a))!=0


MultipurposeShield::MultipurposeShield(uint32_t peripherals)
{
  _capabilities = peripherals;
}


void MultipurposeShield::begin(void)
{
  if (has_peripheral(kDigitalIn0))
  {
    // Digital input.
    pinMode(kPinDigitalIn0,INPUT_PULLUP);
  }
  if (has_peripheral(kDigitalOut0))
  {
    // Digital output.
    pinMode(kPinDigitalOut0,OUTPUT);
    digitalWrite(kPinDigitalOut0,LOW);
  }
  if (has_peripheral(kDigitalIn1))
  {
    // Digital input.
    pinMode(kPinDigitalIn1,INPUT_PULLUP);
  }
  if (has_peripheral(kDigitalOut1))
  {
    // Digital output.
    pinMode(kPinDigitalOut1,OUTPUT);
    digitalWrite(kPinDigitalOut1,LOW);
  }
  if (has_peripheral(kLcd))
  {
    // Initialise LCD.
    lcd.init(1,kPinLcdRs,kPinLcdRw,kPinLcdE,kPinLcdD4,kPinLcdD5,kPinLcdD6,kPinLcdD7,0,0,0,0);
    lcd.begin(16,2);
  }
  if (has_peripheral(kRemote))
  {
    // Digital input.
    pinMode(kPinRemote,INPUT_PULLUP);
  }
  if (has_peripheral(kPushbutton1))
  {
    // Digital input.
    pinMode(kPinPushbutton1,INPUT_PULLUP);
  }
  if (has_peripheral(kTransistor2))
  {
    // Digital output.
    pinMode(kPinTransistor2,OUTPUT);
    digitalWrite(kPinTransistor2,LOW);
  }
  if (has_peripheral(kPushbutton2))
  {
    // Digital input.
    pinMode(kPinPushbutton2,INPUT_PULLUP);
  }
  if (has_peripheral(kTransistor1))
  {
    // Digital output.
    pinMode(kPinTransistor1,OUTPUT);
    digitalWrite(kPinTransistor1,LOW);
  }
  if (has_peripheral(kLed1))
  {
    // Digital output.
    pinMode(kPinLed1,OUTPUT);
    digitalWrite(kPinLed1,LOW);
  }
  if (has_peripheral(kBuzzer))
  {
    // Digital output.
    pinMode(kPinBuzzer,OUTPUT);
    digitalWrite(kPinBuzzer,LOW);
  }
  if (has_peripheral(kTemperature))
  {
    // Digital input.
  }
  if (has_peripheral(kLed2))
  {
    // Digital output.
    pinMode(kPinLed2,OUTPUT);
    digitalWrite(kPinLed2,LOW);
  }
  if (has_peripheral(kLight))
  {
    // Analog input.
  }
  if (has_peripheral(kSound))
  {
    // Analog input.
  }
  if (has_peripheral(kPressure))
  {
    // Analog input.
  }
  if (has_peripheral(kAnalogIn))
  {
    // Analog input.
  }
  if (has_peripheral(kTrimmer))
  {
    // Analog input.
  }
  if (has_peripheral(kHumidity))
  {
    // I2C or similar.
  }
  if (has_peripheral(kIr))
  {
    // I2C or similar.
  }
}


int16_t MultipurposeShield::pressureSensorRead(int16_t offset)
{
  int16_t p;
  if (has_peripheral(kPressure))
  {
    // Use fixed-point arithmetic including rounding and error correction.
    return (17*(analogRead(A1)+offset) + 1714) >> 4;
  }
  return -1;
}


boolean MultipurposeShield::humiditySensorRead(void)
{
  if (has_peripheral(kHumidity))
  {
    sht11.begin(SDA,SCL,true);
    sht11.update();
    return true;
  }
  return false;
}


float MultipurposeShield::humiditySensorReadHumidity(void)
{
  if (humiditySensorRead()==true)
  {
    return sht11.get_humidity();
  }
  return -1;
}


float MultipurposeShield::humiditySensorReadTemperature(void)
{
  if (humiditySensorRead()==true)
  {
    return sht11.get_temperature();
  }
  return -1;
}


float MultipurposeShield::humiditySensorReadDewPoint(void)
{
  if (humiditySensorRead()==true)
  {
    return sht11.get_dewpoint();
  }
  return -1;
}
