#include <SPI.h>
//Bibliotecas do Sensor BME 680
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
//Bibliotecas do rádio
#include <RH_RF95.h>
#include<RHSPIDriver.h>

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define RF95_FREQ 915 

RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define LED 13
void setup()
{
//rf95.setHeaderTo(0xCD);
//rf95.setThisAddress(0xAE);

pinMode(LED, OUTPUT);  
pinMode(RFM95_RST, OUTPUT);
digitalWrite(RFM95_RST, HIGH);

Serial.begin(115200);
delay(100);
Serial.println("Teste Feather LoRa RX!");
// manual reset
digitalWrite(RFM95_RST, LOW);
delay(10);
digitalWrite(RFM95_RST, HIGH);
delay(10);
while (!rf95.init()) {
Serial.println("Inicialização falhou");
while (1);
}
Serial.println("Inicialização OK!");
// Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
if (!rf95.setFrequency(RF95_FREQ)) {
  Serial.println("setFrequency falhou");
while (1);
}
Serial.print("Set Freq em: "); Serial.println(RF95_FREQ);
// Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
// The default transmitter power is 13dBm, using PA_BOOST.
// If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
// you can set transmitter powers from 5 to 23 dBm:
//const RH_RF95::ModemConfig Config = {
//  RH_RF95_BW_500KHZ | RH_RF95_CODING_RATE_4_5, RH_RF95_SPREADING_FACTOR_1024CPS
//}; //BW = 500 kHz, CR = 4/5 e SF = 10

//rf95.setModemRegisters(&Config);
rf95.spiWrite(0x12, 0x02);
rf95.spiWrite(0x1D, 0x92);
rf95.spiWrite(0x1E, 0xC4);

rf95.spiWrite(0x24, 0x05);

rf95.setTxPower(23, false);
}

void loop()
{

if (rf95.available())
{

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

if (rf95.recv(buf, &len))
{

Serial.print((char*)buf);
Serial.print("/");
Serial.print(rf95.lastRssi(), DEC);
Serial.print("!");


}
else
{
Serial.println("Receive failed");
}
}
}