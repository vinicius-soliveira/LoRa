#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <RH_RF95.h>
#include <string.h>


//Radio definitions
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

//BME sensor definitions
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

using namespace std;
// Singleton instance of the radio driver
String dados = " ";
RH_RF95 rf95(RFM95_CS, RFM95_INT);
Adafruit_BME680 bme; 
int period = 10000;
unsigned long time_now = 0;
unsigned long time_now2 = 0;


void setup() {

pinMode(RFM95_RST, OUTPUT);
digitalWrite(RFM95_RST, HIGH);

analogReadResolution(12);

  
  while (!rf95.init()) 
  {
      while (1);
  }
  
  // Defaults after init are 434.0MHz, ee, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) 
  {
  
        while (1);
  }
  
  rf95.spiWrite(0x12, 0x02);
  rf95.spiWrite(0x1D, 0x92);
  rf95.spiWrite(0x1E, 0xC4);
  rf95.spiWrite(0x24, 0x05);

  rf95.setTxPower(23, false); // potencia de transmissão

  if (!bme.begin()) 
  {
        while (1);
  }

  //Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}



void loop()
 {
  if (! bme.performReading()) {
      return;
  }

  float temp50=0, temp75=0, temp100=0;
  time_now = millis();

  for(int i = 0; i < 10; i++)
  {
   time_now2 = millis();
   temp100 += float(analogRead(A0))*(330/4095.0);
   temp75 += float(analogRead(A1))*(330/4095.0);
   temp50 += float(analogRead(A2))*(330/4095.0);
   
while(millis() < time_now2 + 1000){
        
    }

  }

temp100 /= 10;
temp75 /= 10;
temp50 /= 10;

 dados = "#" + String(temp100, 2) + "/" + String(temp75, 2) + "/"+  String(temp50, 2) + "/" + String(bme.temperature,2) + "/" + String(bme.humidity,2) + "/" + String(bme.pressure/100, 2);
 int tamanho = dados.length() + 1;
 char pacote[tamanho];
 dados.toCharArray(pacote, tamanho);

 rf95.send((uint8_t *)pacote, tamanho);
 rf95.waitPacketSent();

 while(millis() < time_now + 1000)
 {
        
  }
}
