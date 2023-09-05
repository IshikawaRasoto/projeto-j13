#define PIN_RODAD 39 
#define PIN_RODAE 34 
#define PIN_EIXOT 35
#define PIN_RPM 36

#define DIAMETRO 0.51916  // Diametro efetivo da roda em metros
#define COMPRIMENTO (PI * DIAMETRO) // Comprimento da roda
#define DENTES 1 // Dentes na roda fônica que vão ser lidos pelo sensor

// Contadores
unsigned int contador_rodaE = 0;
unsigned int contador_rodaD = 0;
unsigned int contador_eixoT = 0;
unsigned int contador_rpm = 0;

// Velocidade
volatile unsigned char vel_rodaE = 0;
volatile unsigned char vel_rodaD = 0;
volatile unsigned char vel_eixoT = 0;
volatile unsigned int RPM_Homo = 0; // Variável para salvar o RPM da homocinetica

// Tempos
volatile unsigned long int tempo_rodaE = 0; // Tempo do último pulso
volatile unsigned long int tempo_rodaD = 0;
volatile unsigned long int tempo_eixoT = 0;
volatile unsigned long int tempo_rpm = 0;

// Timers
unsigned long int Tempo = 0;
hw_timer_t* timer_500ms = NULL; 





void interrupcao_rodaE(){contador_rodaE++;}
void interrupcao_rodaD(){contador_rodaD++;}
void interrupcao_eixoT(){contador_eixoT++;}
void interrupcao_RPM(){contador_rpm++;}

void calculo_RPM() 
{
  /* 
      Para calcular o RPM da homocinética é feita a seguinte fórmula:
              RPM = [(1 / Período) * 1000 * 60] / 1
      No nosso caso o período e dado pelo tempo entre os dois pulsos e que sai em milissegundos.
      Sendo assim, para poder achar o RPM precisamos multiplicar por 1000 e depois por 60.
      Como essa interrupção será chamada 1 vez(es) a cada volta completa da homocinética será
      necessário dividir por 1 para ter o RPM da volta.
  */

  RPM_Homo = contador_rpm * 2 * 60;
  contador_rpm = 0;

  //RPM_Homo = (60000/DENTES)/ (millis() - tempo_rpm);
  //tempo_rpm = millis();
}

void calculo_rodaD(){
  /* 
      Para calcular a velocidade da Roda Dianteira Direita é feita a seguinte fórmula:
              vel_RODAD = [(COMPRIMENTO/4 dentes)/período] * 1000 * 3.6
      No nosso caso o período e dado pelo tempo entre os dois pulsos e que sai em milissegundos.
      Sendo assim, para poder achar o a velocidade precisamos multiplicar por 1000 e depois por 3.6.
      Como essa interrupção será chamada 4 vez(es) a cada volta completa da homocinética será
      necessário dividir por 4 para ter o RPM da volta.
  */

  vel_rodaD = (float)((contador_rodaD * 2.0 * COMPRIMENTO) * 3.6 / 4.0);
  contador_rodaD = 0;
  //vel_rodaD = ((COMPRIMENTO/4)/(millis() - tempo_rodaD)) * 1000 * 3.6;
  //tempo_rodaD = millis();
}

void calculo_rodaE(){
  /* 
      Para calcular a velocidade da Roda Dianteira esquerda é feita a seguinte fórmula:
              vel_RODAD = [(COMPRIMENTO/4 dentes)/período] * 1000 * 3.6
      No nosso caso o período e dado pelo tempo entre os dois pulsos e que sai em milissegundos.
      Sendo assim, para poder achar o a velocidade precisamos multiplicar por 1000 e depois por 3.6.
      Como essa interrupção será chamada 4 vez(es) a cada volta completa da homocinética será
      necessário dividir por 4 para ter o RPM da volta.
  */

  vel_rodaE = (float)((contador_rodaE * 2.0 * COMPRIMENTO) * 3.6 / 4.0);
  contador_rodaE = 0;

  //vel_rodaE = ((COMPRIMENTO/4)/(millis() - tempo_rodaE)) * 1000 * 3.6;
  //tempo_rodaE = millis();
}

void calculo_eixoT(){
  /* 
      Para calcular a velocidade do eixo traseiro é feita a seguinte fórmula:
              vel_RODAD = [(COMPRIMENTO/1 dentes)/período] * 1000 * 3.6
      No nosso caso o período e dado pelo tempo entre os dois pulsos e que sai em milissegundos.
      Sendo assim, para poder achar o a velocidade precisamos multiplicar por 1000 e depois por 3.6.
      Como essa interrupção será chamada 4 vez(es) a cada volta completa da homocinética será
      necessário dividir por 4 para ter o RPM da volta.
  */
  
  vel_eixoT = (float)((contador_eixoT * 2.0 * COMPRIMENTO) * 3.6 / 4.0);
  contador_eixoT = 0;

  //vel_eixoT = ((COMPRIMENTO/1)/(millis() - tempo_eixoT)) * 1000 * 3.6;
  //tempo_eixoT = millis();
}

unsigned int Velocidade()
{
  /*
      Para calcular a velocidade em Km/h fazemos a média das rodas dianteiras:
      O RPM mostra quantas voltas foram dadas em 1 minuto, se multimplicarmos pelo comprimento da roda temos
      a distância em metros percorrida pela roda em 1 minuto, ao dividir por 60 teremos a distância em metros por segundo.
      Então é feita a transformação para Km/h                
   */

  return (vel_rodaD + vel_rodaE)/2;
}

void interrupcato_timer(){

  noInterrupts(); // Desativação de outras interrupções para evitar problemas de "core panic"

  calculo_RPM(); 
  calculo_rodaD();
  calculo_rodaE();
  calculo_eixoT();

  interrupts(); // Reabilita Interrupções.
}


void start_timer(){

  timer_500ms = timerBegin(0, 80, true);

  timerAttachInterrupt(timer_500ms, &interrupcato_timer, true);

  timerAlarmWrite(timer_500ms, 500000, true);

  timerAlarmEnable(timer_500ms);
}



void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(PIN_RODAD, INPUT);
  pinMode(PIN_RODAE, INPUT);
  pinMode(PIN_EIXOT, INPUT);
  pinMode(PIN_RPM, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_RPM), interrupcao_RPM, FALLING); //Uso com Sensor Hall (NF)
  attachInterrupt(digitalPinToInterrupt(PIN_RODAD), interrupcao_rodaD, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_RODAE), interrupcao_rodaE, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_EIXOT), interrupcao_eixoT, RISING);

  Tempo = millis();

  start_timer();
}

void loop() {

  if(Tempo - millis() >= 500) // Leitura de dados a cada "tantos" milissegundos //Se o resto da divisão de Tempo por "tantos" milissegundos for igual a 0 ...
  {

   // delay(500);

    Serial.print("Velocidade do veículo (km/h): ");
    Serial.println(Velocidade());
    
    Serial.print("RPM: ");
    Serial.println(RPM_Homo);

    Serial.print("Velocidade Eixo Traseiro (km/h): ");
    Serial.println(vel_eixoT);

    Serial.print("Velocidade Roda Dianteira Esquerda (km/h): ");
    Serial.println(vel_rodaE);

    Serial.print("Velocidade Roda Dianteira Direita (km/h): ");
    Serial.println(vel_rodaD);

    Serial.println("");

    //Tempo = millis();

  }
  
}


