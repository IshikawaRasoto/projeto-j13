/*

  Configuracao.cpp - Arquivo para programação da biblioteca Configuração.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

*/
#include <Arduino.h>

#include "Configuracao.hpp"
#include "defines.hpp"




void iniciarSeriais()
{
  Serial.begin(BAUD_SERIAL);
}

void configurarPinos()
{
  pinMode(PINO_RODAD, INPUT);
  pinMode(PINO_RODAE, INPUT);
  pinMode(PINO_EIXOT, INPUT);
  pinMode(PINO_RPM, INPUT);
}

