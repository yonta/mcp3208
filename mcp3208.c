#include "mcp3208.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

static const int spi_ch = 0;          //! spi channnel number, 0 or 1
static const float adc_vref = 3.3;    //! ADC ref voltage [V]
static const int adc_resolution = 12; //! ADC resolution, 12bits in MCP3208
static const int adc_max = pow(2, adc_resolution); //! analog bit range, 4096

//! Initialize SPI interface by WiringPi
void init()
{
  // WiriginPi needs su permission
  wiringPiSetup();
  // ADC MCP3208 is stable with 244.14kHz
  const int spi_speed = 245000;
  if (wiringPiSPISetup(spi_ch, spi_speed) == -1) {
    printf("wiringPiSPISetup failed in SPI0.\n")
    exit(EXIT_FAILURE);
  }
}

//! Analog read function like arduino
/*!
  \param adc_pin ADC pin number (0 - 7)
  \return analog bit (0 - 4095)
*/
uint16_t analogRead(uint8_t adc_pin)
{
  unsigned char buff[] = {static_cast<char>(0b110 | ((adc_pin & 0b111) >> 2)),
                          static_cast<char>((adc_pin & 0b111) << 6),
                          static_cast<char>(0)};

  wiringPiSPIDataRW(spi_ch, buff, 3);
  return ((buff[1] & 0b1111) << 8) | buff[2];
}

//! Analog read function by Voltage[V]
/*!
  \param adc_pin ADC pin number (0 - 7)
  \return input voltage (0 ~ Vref [V])
*/
float analogReadVoltage(uint8_t adc_pin)
{
  uint16_t bit = analogRead(adc_pin);
  float voltage = (float)bit * adc_vref / adc_max;
  return voltage;
}
