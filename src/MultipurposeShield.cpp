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


#define multipurposeShield(a)  (_peripherals&(a))!=0


MultipurposeShield::MultipurposeShield(uint32_t peripherals)
{
  _peripherals = peripherals;
}


void MultipurposeShield::begin(void)
{
  if (multipurposeShield(hasDigitalIn0))
  {
    // Digital input.
    pinMode(pinDigitalIn0,INPUT_PULLUP);
  }
  if (multipurposeShield(hasDigitalOut0))
  {
    // Digital output.
    pinMode(pinDigitalOut0,OUTPUT);
    digitalWrite(pinDigitalOut0,LOW);
  }
  if (multipurposeShield(hasDigitalIn1))
  {
    // Digital input.
    pinMode(pinDigitalIn1,INPUT_PULLUP);
  }
  if (multipurposeShield(hasDigitalOut1))
  {
    // Digital output.
    pinMode(pinDigitalOut1,OUTPUT);
    digitalWrite(pinDigitalOut1,LOW);
  }
  if (multipurposeShield(hasLcd))
  {
    // Initialise LCD.
    lcd.init(1,pinLcdRs,pinLcdRw,pinLcdE,pinLcdD4,pinLcdD5,pinLcdD6,pinLcdD7,0,0,0,0);
    lcd.begin(16,2);
  }
  if (multipurposeShield(hasRcDetector))
  {
    // Digital input.
    pinMode(pinRcDetector,INPUT_PULLUP);
  }
  if (multipurposeShield(hasPushbutton1))
  {
    // Digital input.
    pinMode(pinPushbutton1,INPUT_PULLUP);
  }
  if (multipurposeShield(hasTransistor2))
  {
    // Digital output.
    pinMode(pinTransistor2,OUTPUT);
    digitalWrite(pinTransistor2,LOW);
  }
  if (multipurposeShield(hasPushbutton2))
  {
    // Digital input.
    pinMode(pinPushbutton2,INPUT_PULLUP);
  }
  if (multipurposeShield(hasTransistor1))
  {
    // Digital output.
    pinMode(pinTransistor1,OUTPUT);
    digitalWrite(pinTransistor1,LOW);
  }
  if (multipurposeShield(hasLed1))
  {
    // Digital output.
    pinMode(pinLed1,OUTPUT);
    digitalWrite(pinLed1,LOW);
  }
  if (multipurposeShield(hasBuzzer))
  {
    // Digital output.
    pinMode(pinBuzzer,OUTPUT);
    digitalWrite(pinBuzzer,LOW);
  }
  if (multipurposeShield(hasThermometer))
  {
    // Digital input.
  }
  if (multipurposeShield(hasLed2))
  {
    // Digital output.
    pinMode(pinLed2,OUTPUT);
    digitalWrite(pinLed2,LOW);
  }
  if (multipurposeShield(hasLightSensor))
  {
    // Analog input.
  }
  if (multipurposeShield(hasMicrophone))
  {
    // Analog input.
  }
  if (multipurposeShield(hasPressureSensor))
  {
    // Analog input.
  }
  if (multipurposeShield(hasAnalogIn))
  {
    // Analog input.
  }
  if (multipurposeShield(hasPotentiometer))
  {
    // Analog input.
  }
  if (multipurposeShield(hasHumiditySensor))
  {
    // I2C or similar.
  }
  if (multipurposeShield(hasIrThermometer))
  {
    // I2C or similar.
  }
}


float MultipurposeShield::thermometerRead(void)
{
  if (multipurposeShield(hasThermometer))
  {
    return ds18b20.read();
  }
  return FLT_MAX;
}


int16_t MultipurposeShield::pressureSensorRead(int16_t offset)
{
  if (multipurposeShield(hasPressureSensor))
  {
    // Use fixed-point arithmetic including rounding and error correction.
    return (17*(analogRead(pinPressureSensor)+offset) + 1714) >> 4;
  }
  return -1;
}


boolean MultipurposeShield::humiditySensorRead(void)
{
  if (multipurposeShield(hasHumiditySensor))
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
  return FLT_MAX;
}


float MultipurposeShield::humiditySensorReadT(void)
{
  if (humiditySensorRead()==true)
  {
    return sht11.get_temperature();
  }
  return FLT_MAX;
}


float MultipurposeShield::humiditySensorReadDewPoint(void)
{
  if (humiditySensorRead()==true)
  {
    return sht11.get_dewpoint();
  }
  return FLT_MAX;
}


float MultipurposeShield::infraredThermometerRead(void)
{
  if (multipurposeShield(hasIrThermometer))
  {
    mlx90614.begin(A4,A5);
    return mlx90614.read();
  }
  return FLT_MAX;
}


int16_t MultipurposeShield::lightSensorRead(boolean asPercentage)
{
  if (multipurposeShield(hasLightSensor))
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


void MultipurposeShield::digitalWriteChecked(uint32_t hasPeripheral, uint8_t pin, uint8_t value)
{
  if (multipurposeShield(hasPeripheral))
  {
    digitalWrite(pin,value);
  }
}


int8_t MultipurposeShield::digitalReadChecked(uint32_t hasPeripheral, uint8_t pin)
{
  if (multipurposeShield(hasPeripheral))
  {
    return digitalRead(pin);
  }
  return -1;
}
