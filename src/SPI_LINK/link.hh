#pragma once
#include <Arduino.h>
#include <SPI.h>

class SPI_LINK
{
private:
    /* SPI CONFIG */
    const byte VSPI_MISO = MISO;
    const byte VSPI_MOSI = MOSI;
    const byte VSPI_SCLK = SCK;
    const byte VSPI_SS = SS;

    const int spiClk = 8000000; // 8 MHz

    /* SPI Object */
    SPIClass *vspi;

public:
    uint8_t rx_buffer[3];

public:
    void BEGIN(byte SPI_PORT)
    {
        vspi = new SPIClass(SPI_PORT); // USE VSPI

        vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS); // Innit SPI connection

        // Set up slave select pins as outputs as the Arduino API
        // Doesn't handle automatically pulling SS low
        pinMode(VSPI_SS, OUTPUT);    // VSPI SS
        digitalWrite(VSPI_SS, HIGH); // pull ss high to signify end of data transfer
    };

    void SPI_TRANSFER(uint8_t *tx_data)
    {

        digitalWrite(VSPI_SS, LOW); // pull SS slow to prep other end for transfer

        vspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));

        for (int i = 0; i < 3; i++)
        {

            rx_buffer[i] = vspi->transfer(tx_data[i]);
        }

        vspi->endTransaction();

        digitalWrite(VSPI_SS, HIGH); // pull ss high to signify end of data transfer
    };
};
