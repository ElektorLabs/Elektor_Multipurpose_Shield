/*
 * Arduino driver for SHT1x temperature & relative humidity sensor.
 * Handles CRC (optional).
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

#include "SHT1x.h"


#define SHT1X_CMD_READ_TEMPERATURE  0x03
#define SHT1X_CMD_READ_REL_HUMIDITY  0x05
#define SHT1X_CMD_READ_STATUS  0x07
#define SHT1X_CMD_WRITE_STATUS  0x06
#define SHT1X_CMD_SOFT_RESET  0x1e

const float C1 = -2.0468;  // for 12 Bit RH
const float C2 = +0.0367;  // for 12 Bit RH
const float C3 = -0.0000015955;  // for 12 Bit RH
const float T1 = +0.01;  // for 12 Bit RH
const float T2 = +0.00008;  // for 12 Bit RH	


static uint8_t debug = 0;


void SHT1x::begin(uint8_t data, uint8_t sck, boolean disable_twi)
{
  _data = data;
  _sck = sck;
  _temperature = 0.0;
  _humidity = 0.0;
  connection_reset();
  if (disable_twi==true)
  {
    // In case the sensor shares the TWI bus.
    // Calling Wire.begin will restore this.
    TWCR &= ~(_BV(TWEN) | _BV(TWIE) | _BV(TWEA));
  }
}


void SHT1x::update(void)
{
  int t;
  int h;
  uint8_t crc2;
  boolean error;
  
#ifdef __USE_CRC__
  uint8_t stat;
  error = status_register_read(stat,crc2);
  if (error==false)
  {
    crc.set(crc.bit_reverse(stat));
  }
  else
  {
    if (debug) Serial.println("status ack error");
    crc.reset();
  }
#endif /* __USE_CRC__ */

  error = measure(t,crc2,SHT1X_CMD_READ_TEMPERATURE);
#ifdef __USE_CRC__
  if (crc.get()!=0)
  {
    if (debug) Serial.println("Temperature CRC error.");
  }
#endif /* __USE_CRC__ */

  error |= measure(h,crc2,SHT1X_CMD_READ_REL_HUMIDITY);
#ifdef __USE_CRC__
  if (crc.get()!=0)
  {
    if (debug) Serial.println("Humidity CRC error.");
  }
#endif /* __USE_CRC__ */

  if (error==false)
  {
    _temperature = t;
    _humidity = h;
    calculcate();
    if (debug)
    {
      Serial.print("T=");
      Serial.print(_temperature);
      Serial.print(", RH=");
      Serial.print(_humidity);
      Serial.print("%, dew point=");
      Serial.println(get_dewpoint());
    }
  }
  else
  {
    if (debug) Serial.println("measure ack error");
  }
}


float SHT1x::get_dewpoint(void)
{ 
  float k = (log10(_humidity)-2)/0.4343 + (17.62*_temperature)/(243.12+_temperature);
  return 243.12*k/(17.62-k);
}


void SHT1x::strobe(void)
{
  digitalWrite(_sck,HIGH);
  delay(1);
  digitalWrite(_sck,LOW);
}


void SHT1x::start_sequence(void)
{
  pinMode(_data,OUTPUT);
  pinMode(_sck,OUTPUT);
  digitalWrite(_data,HIGH);
  digitalWrite(_sck,LOW);
  digitalWrite(_sck,HIGH);
  digitalWrite(_data,LOW);
  digitalWrite(_sck,LOW);
  digitalWrite(_sck,HIGH);
  digitalWrite(_data,HIGH);
  digitalWrite(_sck,LOW);
}


void SHT1x::connection_reset(void)
{
  int i;
  pinMode(_data,OUTPUT);
  pinMode(_sck,OUTPUT);
  digitalWrite(_data,HIGH);
  digitalWrite(_sck,LOW);
  for (i=0; i<9; i++)
  {
    strobe();
  }
  start_sequence();
}


boolean SHT1x::soft_reset(void)
{
  boolean result;
  connection_reset();
  result = send_byte(SHT1X_CMD_SOFT_RESET);
  delay(15); // 11 ms minimum
  return result;
}


boolean SHT1x::send_byte(uint8_t value)
{
  uint8_t i;
  boolean error = true;

  pinMode(_data,OUTPUT);
  pinMode(_sck,OUTPUT);
  
  for (i=0x80; i>0; i>>=1)
  {
    if ((value&i)!=0) digitalWrite(_data,HIGH);
    else digitalWrite(_data,LOW);
    strobe();
  }

  pinMode(_data,INPUT_PULLUP);  
  digitalWrite(_sck,HIGH);
  if (digitalRead(_data)==0) error = false;
  digitalWrite(_sck,LOW);
  return error;
}


uint8_t SHT1x::receive_byte(uint8_t ack)
{
  int i;
  uint8_t value = 0;

  pinMode(_data,INPUT_PULLUP);  
  
  for (i=0x80; i>0; i/=2)
  {
    digitalWrite(_sck,HIGH);
    if (digitalRead(_data)!=0) value |= i;
    digitalWrite(_sck,LOW);
  }
  
  pinMode(_data,OUTPUT);  
  digitalWrite(_data,ack);
  strobe();
  digitalWrite(_data,HIGH);

  return value;
}


boolean SHT1x::status_register_read(uint8_t& result, uint8_t& crc2)
{ 
  boolean error;
  start_sequence();
  error = send_byte(SHT1X_CMD_READ_STATUS);
  result = receive_byte(LOW);
  crc2 = receive_byte(HIGH);
  return error;
}


boolean SHT1x::status_register_write(uint8_t value)
{ 
  boolean error;
  start_sequence();
  error = send_byte(SHT1X_CMD_WRITE_STATUS);
  error |= send_byte(value);
  return error;
}


boolean SHT1x::wait_for_measurement(void)
{
  pinMode(_data,INPUT_PULLUP);  
  while (digitalRead(_data)!=0);
  return true;
}


boolean SHT1x::measure(int& result, uint8_t& crc2, uint8_t command)
{
  boolean error;
  uint8_t temp;
  start_sequence();
  error = send_byte(command);
#ifdef __USE_CRC__
  crc.reset();
  crc.update(command);
#endif /* __USE_CRC__ */
  wait_for_measurement();
  temp = receive_byte(LOW);
#ifdef __USE_CRC__
  crc.update(temp);
#endif /* __USE_CRC__ */
  result = temp << 8;
  temp = receive_byte(LOW);
#ifdef __USE_CRC__
  crc.update(temp);
#endif /* __USE_CRC__ */
  result += temp;
  crc2 = receive_byte(HIGH);
#ifdef __USE_CRC__
  crc.update(crc.bit_reverse(crc2)); // Now CRC should equal 0.
#endif /* __USE_CRC__ */
  return error;
}


void SHT1x::calculcate()
{ 
  _temperature = _temperature*0.01 - 40.1;  // [degrees C], 14 bits @ 5V
  _humidity = C3*_humidity*_humidity + C2*_humidity + C1;  // [%RH]
  _humidity = (_temperature-25)*(T1+T2*_humidity) + _humidity;  // _temperature compensated _humidity [%RH]
  _humidity = constrain(_humidity,0.1,100);
}
