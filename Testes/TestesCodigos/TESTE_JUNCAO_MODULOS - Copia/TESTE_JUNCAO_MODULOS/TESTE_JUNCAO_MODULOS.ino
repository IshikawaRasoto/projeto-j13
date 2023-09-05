/*

  TESTE_JUNCAO_MODULOS.ino - Arquivo para produção e prototipagem do código para atingir sua versão final.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

*/

#include "defines.hpp"
#include "Configuracao.hpp"

#include "Veiculo.hpp"

volatile unsigned int contadorRodaE = 0;
volatile unsigned int contadorRodaD = 0;
volatile unsigned int contadorEixoT = 0;
volatile unsigned int contadorRPM = 0;

hw_timer_t* timer_500ms = nullptr;

Veiculo baja;

void interrupcaoRodaE()
{
  baja.incrementarRodaE();
}

void interrupcaoRodaD()
{
  baja.incrementarRodaD();
}

void interrupcaoEixoT()
{
  baja.incrementarEixoT();
}
void interrupcaoRPM(){
  baja.incrementarRPM();
}

void interrupcaoTimer()
{
  noInterrupts();

  baja.atualizar();

  interrupts();
}

void configurarTimer()
{
  timer_500ms = timerBegin(0, 80, true);

  timerAttachInterrupt(timer_500ms, &interrupcaoTimer, true);

  timerAlarmWrite(timer_500ms, 500000, true);

  timerAlarmEnable(timer_500ms);
}

void setup(){
  
  iniciarSeriais();
  configurarPinos();
  configurarTimer();
  
}

void loop(){

}