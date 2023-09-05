/*

  Veiculo.cpp - Arquivo para programação da biblioteca do Sistema.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

*/
#include "Veiculo.hpp"
#include "defines.hpp"



/* 

  Classe Veículo

*/

Veiculo::Veiculo():
  velocidade(0),
  RPM(0),
  velocidadeRodaE(0),
  velocidadeRodaD(0),
  velocidadeEixoT(0),
  contadorRPM(0),
  contadorRodaD(0),
  contadorRodaE(0),
  contadorEixoT(0)
{}

Veiculo::~Veiculo(){}

void Veiculo::atualizar()
{
  atualizarRPM();
  atualizarVelocidadeRodaE();
  atualizarVelocidadeRodaD();
  atualizarVelocidadeEixoT();

  atualizado = true;
}

void Veiculo::atualizarRPM()
{
  RPM = contadorRPM * 2 * 60;
  contadorRPM = 0;
}

void Veiculo::atualizarVelocidadeRodaD()
{
  velocidadeRodaD = (float)((contadorRodaD * 2.0 * COMPRIMENTO) * 3.6 / 4.0);
  contadorRodaD = 0;
}

void Veiculo::atualizarVelocidadeRodaE()
{
  velocidadeRodaE = (float) ((contadorRodaE * 2.0 * COMPRIMENTO) * 3.6 / 4.0);
  contadorRodaE = 0;
}

void Veiculo::atualizarVelocidadeEixoT()
{
  velocidadeEixoT = (float) ((contadorEixoT * 2.0 * COMPRIMENTO) * 3.6 / 4.0);
  contadorEixoT = 0;
}

void Veiculo::atualizarVelocidade()
{
  velocidade = (velocidadeRodaE + velocidadeRodaD) / 2.0;
}

bool Veiculo::estaAtualizado()
{
  if(atualizado)
  {
    atualizado = false;
    return true;
  }
  return false;
}

void Veiculo::incrementarRodaE(){contadorRodaE++;}
void Veiculo::incrementarRodaD(){contadorRodaD++;}
void Veiculo::incrementarEixoT(){contadorEixoT++;}
void Veiculo::incrementarRPM(){contadorRPM++;}

float Veiculo::getVelocidade(){return velocidade;}
float Veiculo::getVelocidadeRodaE(){return velocidadeRodaE;}
float Veiculo::getVelocidadeRodaD(){return velocidadeRodaD;}
float Veiculo::getVelocidadeEixoT(){return velocidadeEixoT;}
int Veiculo::getRPM(){return RPM;}