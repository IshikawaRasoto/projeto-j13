#include "mcp2515_can.h"
#include <SPI.h>
#include <Wire.h>

void I2C(char, int);

#define SPI_CS 10
uint32_t CAN_ID = 0x100;
uint32_t ID_MSG = 0;
unsigned char Buf[8] = {0};
mcp2515_can CAN(SPI_CS);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while(!Serial){};
  while(CAN_OK != CAN.begin(CAN_125KBPS))
  {
    Serial.println("CAN falhou, tenta novamente...");
    delay(100);
  }

  Serial.println("CAN Iniciada");
}

void loop() {
  if(CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(8, Buf);
    ID_MSG = CAN.getCanId();

    if(ID_MSG == 1)
    {
      for(int i = 0; i < 8; i++)
      {
        switch(i) // Com base na indexação de cada ECU
        {
          case 0:
            I2C('V', Buf[i]);

            Serial.print("V: ");
            Serial.println(Buf[i]);

            break;
          case 1: 
            I2C('R', Buf[i]);

            // Serial.print("R: ");
            // Serial.println(Buf[i]);

            break;
          case 2: // Temperatura
            if(Buf[i] == 1){

              I2C('L',11); // Envio 11 pois significa que o LED 1 tem valor de 1(HIGH)

              // Serial.print("T: ");
              // Serial.println(Buf[i]);
            }
            else if(Buf[i] == 0){

              I2C('L',10);

              // Serial.print("T: ");
              // Serial.println(Buf[i]);
            }
            break;
          case 3: // Freio
            if(Buf[i] == 1){

              I2C('L',21);

              Serial.print("F: ");
              Serial.println(Buf[i]);
            }
            else if(Buf[i] == 0){

              I2C('L',20);

              Serial.print("F: ");
              Serial.println(Buf[i]);
            }
            break;
          case 6: // Bateria
            if(Buf[i] == 1){

              I2C('L',31);

              // Serial.print("B: ");
              // Serial.println(Buf[i]);
            }
            else if(Buf[i] == 0){

              I2C('L',30);

              // Serial.print("B: ");
              // Serial.println(Buf[i]);
            }
          break;
        }
      }
    }
    else if (ID_MSG == 10)
    {
      for(int i = 0; i < 8; i++)
      {
        switch(i)
        {
          case 0:
            I2C('V', Buf[i]);

            Serial.print("V: ");
            Serial.println(Buf[i]);

            break;
          case 1: 
            I2C('R', Buf[i]);

            // Serial.print("R: ");
            // Serial.println(Buf[i]);

            break;
          case 2: // Temperatura
            if(Buf[i] == 1){

              I2C('L',11); // Envio 11 pois significa que o LED 1 tem valor de 1(HIGH)

              // Serial.print("T: ");
              // Serial.println(Buf[i]);
            }
            else if(Buf[i] == 0){

              I2C('L',10);

              // Serial.print("T: ");
              // Serial.println(Buf[i]);
            }
            break;
          case 3: // Freio
            if(Buf[i] == 1){

              I2C('L',21);

              Serial.print("F: ");
              Serial.println(Buf[i]);
            }
            else if(Buf[i] == 0){

              I2C('L',20);

              Serial.print("F: ");
              Serial.println(Buf[i]);
            }
            break;
          case 6: // Bateria
            if(Buf[i] == 1){

              I2C('L',31);

              // Serial.print("B: ");
              // Serial.println(Buf[i]);
            }
            else if(Buf[i] == 0){

              I2C('L',30);

              // Serial.print("B: ");
              // Serial.println(Buf[i]);
            }
          break;
        }
      }
    }
  }
}

void I2C(char ID, int Valor)
{
  Wire.beginTransmission(0x8); // Inicia transmissão com o endereço 0x8 (Painel)
  Wire.write(ID);
  Wire.write(Valor);
  Wire.write(";");
  Wire.endTransmission();
  delay(20);
}
