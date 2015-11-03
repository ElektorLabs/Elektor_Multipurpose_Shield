/*
 * Weather Station
 * Display temperature, pressure, humidity & light level.
 *
 * Belongs to:
 * "Mastering Microcontrollers Helped by Arduino"
 * ISBN 978-1-907920-23-3 (English)
 * ISBN 978-2-86661-190-3 (French)
 * http://www.polyvalens.com/
 *
 * For use with PolyValens Multipurpose Shield 129009-1
 *
 * Copyright (c) 2014, Clemens Valens
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
 
#include <LiquidCrystal.h>
LiquidCrystal lcd(6,7,5,4,3,2);

#define __CELSIUS__

//#define __USE_CRC__
#include "SHT1x.h"
SHT1x sht1x;

// Custom degree character.
uint8_t degree[8] = 
{
#ifdef __CELSIUS__
  0b11100,
  0b10100,
  0b11100,
  0b00000,
  0b00011,
  0b00100,
  0b00100,
  0b00011
#else
  0b11100,
  0b10100,
  0b11100,
  0b00000,
  0b00111,
  0b00100,
  0b00110,
  0b00100
#endif /* __CELSIUS__ */
};

uint32_t t_last;
#define LIGHT_MAX  (16)
uint16_t light[LIGHT_MAX];
uint8_t light_index = 0;
const int led = 13;
//const int p_offset = 1016 - 993; // For my sensor.

// Kanfen, 14/3/2014 @ 11h15, 19°C, 220m.
// Official pressure: 1028 mbar -> 4,23 V -> ADC reading of 867.
// Measured: 823 -> 4,02 V -> 981 mbar
// Offset: 867 - 823 = 44
//const int p_offset = 867 - 823; 

// Pluneret, 29/3/2014 @ 12h00, 21°C, 70m.
// Official pressure: 1010 mbar -> 4,15 V -> ADC reading of 850.
// Measured: 827 -> 4,04 V -> 985 mbar
// Offset: 850 - 827 = 23
const int p_offset = 850 - 827;

uint16_t running_average(uint16_t val)
{
  uint32_t sum;
  uint8_t i;
  
  light[light_index] = val;
  light_index = (light_index+1)&(LIGHT_MAX-1);
  
  sum = 0;
  for (i=0; i<LIGHT_MAX; i++)
  {
    sum += light[i];
  }
  sum /= LIGHT_MAX;
  return sum;
}

void setup(void)
{
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  lcd.createChar(0,degree);
  lcd.begin(16,2);
  lcd.print("Weather Station");
  delay(1500);
  lcd.clear();
  //        "0123456789012345"
  lcd.print(" t  rh  mbar lum");
  
  t_last = 0;
}

void loop(void)
{
  uint16_t light = running_average(1023-analogRead(A0));

  if (millis()>t_last+1000)
  {
    t_last = millis();

    digitalWrite(led,HIGH);
    
    int p = analogRead(A1);
    p = (17*(p+p_offset) + 1714) >> 4;

    sht1x.begin(SDA,SCL,true);
    sht1x.update();
    float t = sht1x.get_temperature();
    float rh = sht1x.get_humidity();

#ifndef __CELSIUS__
    t = t*9.0/5.0 + 32.0; // Celsius to Fahrenheit.
#endif /* __CELSIUS__ */

    lcd.setCursor(0,1);
    if (t>=0 && t<10) lcd.print(' ');
    lcd.print(t+0.5,0);
    lcd.write((uint8_t)0);
    lcd.print(' ');
    lcd.print(rh+0.5,0);
    lcd.print("% ");
    lcd.print(p);
    if (p<1000) lcd.print(' ');
    lcd.print(' ');
    lcd.print(light);
    if (light<10) lcd.print(' ');
    if (light<100) lcd.print(' ');

    digitalWrite(led,LOW);
  }
}

