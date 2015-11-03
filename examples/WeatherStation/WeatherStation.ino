/*
 * Weather Station
 * Display temperature, pressure, relative humidity & light level.
 *
 * Shows how to use the Multipurpose Shield library.
 *
 * For use with PolyValens Multipurpose Shield 129009-1
 *
 * Belongs to:
 * "Mastering Microcontrollers Helped by Arduino"
 * ISBN 978-1-907920-23-3 (English)
 * ISBN 978-2-86661-190-3 (French)
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
 
#include <MultipurposeShield.h>

// Define the shield object and the peripherals that
// are available.
MultipurposeShield mps(hasLcd |
                       hasLed2 |
                       hasPressureSensor |
                       hasHumiditySensor |
                       hasLightSensor);

#define __CELSIUS__

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


void setup(void)
{
  mps.begin();
  mps.lcd.print("Weather Station");
  delay(1500);
  // Create special degree character to save display space.
  mps.lcd.createChar(0,degree);
  mps.lcd.clear();
  // Note: "lum" does not mean "lumen", that would be "lm".
  //            "0123456789012345"
  mps.lcd.print(" t  rh  mbar lum"); 
}


void loop(void)
{
  static uint32_t t_last = 0;
  
  // Read the light sensor.
  uint16_t light = mps.lightSensorRead();

  // Every 1000 ms do...
  if (millis()>t_last+1000)
  {
    t_last = millis();

    // LED2 on.
    mps.led2Write(HIGH);
    
    // Read the pressure sensor.
    // The correction value of 23 depends on your location.
    // First set to 0 and compare the displayed value with a
    // reference value obtained from another source (internet),
    // then calculate the difference and use that in place of 23.
    // The correction may be positive or negative.
    int p = mps.pressureSensorRead(23);
    
    // Read the humidity sensor twice as it provides relative
    // humidity and temperature.
    float rh = mps.humiditySensorReadRh();
    float t = mps.humiditySensorReadT();

#ifndef __CELSIUS__
    t = t*9.0/5.0 + 32.0; // Celsius to Fahrenheit.
#endif /* __CELSIUS__ */

    // Show results.
    mps.lcd.setCursor(0,1);
    
    // Print temperature, right justified.
    t += 0.5; // For rounding.
    // There is not enough space on the display for 3-digit values.
    if (t>99.0) t = 99.0;
    if (t>=0.0 && t<10.0) mps.lcd.print(' '); // Negative values need an extra position.
    mps.lcd.print(t,0); // Print without decimals.
    mps.lcd.write((uint8_t)0); // Print our special degree symbol.

    // Print separator.
    mps.lcd.print(' ');
    
    // Print relative humidity, right justified.
    rh += 0.5; // For rounding.
    // There is not enough space on the display for 3-digit values.
    if (rh>99.0) rh = 99.0;
    if (rh<10.0) mps.lcd.print(' ');
    mps.lcd.print(rh,0); // Print without decimals.
    mps.lcd.print("%");
    
    // Print separator.
    mps.lcd.print(' ');

    // Print atmospheric pressure, right justified.
    if (p<1000) mps.lcd.print(' ');
    if (p<100) mps.lcd.print(' '); // Very, very bad weather.
    if (p<10) mps.lcd.print(' '); // End of the world.
    mps.lcd.print(p);
    
    // Print separator.
    mps.lcd.print(' ');
    
    // Print luminosity, right justified.
    // There is not enough space on the display for 3-digit values.
    if (light>99) light = 99;
    if (light<10) mps.lcd.print(' ');
    mps.lcd.print(light);
    mps.lcd.print("%");

    // LED2 off.
    mps.led2Write(LOW);
  }
}

