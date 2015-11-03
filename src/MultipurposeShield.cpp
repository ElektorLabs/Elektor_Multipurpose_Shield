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
  if (has_peripheral(hasDigitalIn0))
  {
    // Digital input.
    pinMode(pinDigitalIn0,INPUT_PULLUP);
  }
  if (has_peripheral(hasDigitalOut0))
  {
    // Digital output.
    pinMode(pinDigitalOut0,OUTPUT);
    digitalWrite(pinDigitalOut0,LOW);
  }
  if (has_peripheral(hasDigitalIn1))
  {
    // Digital input.
    pinMode(pinDigitalIn1,INPUT_PULLUP);
  }
  if (has_peripheral(hasDigitalOut1))
  {
    // Digital output.
    pinMode(pinDigitalOut1,OUTPUT);
    digitalWrite(pinDigitalOut1,LOW);
  }
  if (has_peripheral(hasLcd))
  {
    // Initialise LCD.
    lcd.init(1,pinLcdRs,pinLcdRw,pinLcdE,pinLcdD4,pinLcdD5,pinLcdD6,pinLcdD7,0,0,0,0);
    lcd.begin(16,2);
  }
  if (has_peripheral(hasRcDetector))
  {
    // Digital input.
    pinMode(pinRcDetector,INPUT_PULLUP);
  }
  if (has_peripheral(hasPushbutton1))
  {
    // Digital input.
    pinMode(pinPushbutton1,INPUT_PULLUP);
  }
  if (has_peripheral(hasTransistor2))
  {
    // Digital output.
    pinMode(pinTransistor2,OUTPUT);
    digitalWrite(pinTransistor2,LOW);
  }
  if (has_peripheral(hasPushbutton2))
  {
    // Digital input.
    pinMode(pinPushbutton2,INPUT_PULLUP);
  }
  if (has_peripheral(hasTransistor1))
  {
    // Digital output.
    pinMode(pinTransistor1,OUTPUT);
    digitalWrite(pinTransistor1,LOW);
  }
  if (has_peripheral(hasLed1))
  {
    // Digital output.
    pinMode(pinLed1,OUTPUT);
    digitalWrite(pinLed1,LOW);
  }
  if (has_peripheral(hasBuzzer))
  {
    // Digital output.
    pinMode(pinBuzzer,OUTPUT);
    digitalWrite(pinBuzzer,LOW);
  }
  if (has_peripheral(hasThermometer))
  {
    // Digital input.
  }
  if (has_peripheral(hasLed2))
  {
    // Digital output.
    pinMode(pinLed2,OUTPUT);
    digitalWrite(pinLed2,LOW);
  }
  if (has_peripheral(hasLightSensor))
  {
    // Analog input.
  }
  if (has_peripheral(hasMicrophone))
  {
    // Analog input.
  }
  if (has_peripheral(hasPressureSensor))
  {
    // Analog input.
  }
  if (has_peripheral(hasAnalogIn))
  {
    // Analog input.
  }
  if (has_peripheral(hasPotentiometer))
  {
    // Analog input.
  }
  if (has_peripheral(hasHumiditySensor))
  {
    // I2C or similar.
  }
  if (has_peripheral(hasIrThermometer))
  {
    // I2C or similar.
  }
}


int16_t MultipurposeShield::pressureSensorRead(int16_t offset)
{
  int16_t p;
  if (has_peripheral(hasPressureSensor))
  {
    // Use fixed-point arithmetic including rounding and error correction.
    return (17*(analogRead(pinPressureSensor)+offset) + 1714) >> 4;
  }
  return -1;
}


boolean MultipurposeShield::humiditySensorRead(void)
{
  if (has_peripheral(hasHumiditySensor))
  {
    sht11.begin(SDA,SCL,true);
    sht11.update();
    return true;
  }
  return false;
}


float MultipurposeShield::humiditySensorReadRh(void)
{
  if (humiditySensorRead()==true)
  {
    return sht11.get_humidity();
  }
  return -1;
}


float MultipurposeShield::humiditySensorReadT(void)
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


int16_t MultipurposeShield::lightSensorRead(boolean asPercentage)
{
  if (has_peripheral(hasLightSensor))
  {
    // The LDR gives a high reading for low light levels, so invert it.
    int32_t l = 1023 - analogRead(pinLightSensor);
    if (asPercentage==true)
    {
      // Calculate percentage.
      return (int16_t) (100*l/1023);
    }
    else return (int16_t) l; // return raw value.
  }
  return -1;
}


void MultipurposeShield::digitalWriteChecked(uint32_t capability, uint8_t pin, uint8_t value)
{ 
  if (has_peripheral(capability))
  {
    digitalWrite(pin,value); 
  }
}


int8_t MultipurposeShield::digitalReadChecked(uint32_t capability, uint8_t pin)
{ 
  if (has_peripheral(capability))
  {
    return digitalRead(pin); 
  }
  return -1;
}
