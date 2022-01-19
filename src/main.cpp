#include <Arduino.h>
#include "./SPI_LINK/link.hh"

SPI_LINK COM;

void setup()
{
  Serial.begin(9600);
  COM.BEGIN(VSPI);
}

void loop()
{

  byte sending_data[3] = {1, 2, 3};

  COM.SPI_TRANSFER(sending_data);

  for (int i = 0; i < 3; i++)
  {
    Serial.println(COM.rx_buffer[i]);
  }

  Serial.println(" ");

  delay(1000);
}