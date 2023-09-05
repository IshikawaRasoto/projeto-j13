/*

  Veiculo.hpp - Arquivo para declaração da biblioteca do Sistema.
  Criado em 14/07/2023 pelo Lambari para implementar no J12 até novembro e servir como base para o J13.

  Essa biblioteca possui o código Procedural.

*/

#pragma once

#include <Arduino.h>

class Veiculo
{
  private:

    volatile float velocidade;
    volatile int RPM;
    volatile float velocidadeRodaE;
    volatile float velocidadeRodaD;
    volatile float velocidadeEixoT;

    volatile int contadorRPM;
    volatile int contadorRodaD;
    volatile int contadorRodaE;
    volatile int contadorEixoT;

    bool atualizado;

    
  public:

    Veiculo();
    ~Veiculo();

    void incrementarRodaE();
    void incrementarRodaD();
    void incrementarEixoT();
    void incrementarRPM();

    void atualizarVelocidadeRodaE();
    void atualizarVelocidadeRodaD();
    void atualizarVelocidadeEixoT();
    void atualizarRPM();
    void atualizarVelocidade();

    float getVelocidade();
    int getRPM();
    float getVelocidadeRodaE();
    float getVelocidadeRodaD();
    float getVelocidadeEixoT();


    bool estaAtualizado();

    void atualizar();

};