#define PINO_RPM 3
#define PINO_VEL 2

#define BAUDRATE 115200

volatile unsigned long aux;
volatile unsigned long tempoRPM;
volatile unsigned long tempoVelocidade;
volatile unsigned long tempoCalculo;

volatile uint16_t rpm = 0;
volatile uint8_t velocidade = 0;

#define CONST_PI 3.14159
#define DIAMETRO 0.51916 // Diâmetro da roda em metros
#define COMPRIMENTO CONST_PI * DIAMETRO

void interrupcaoRPM()
{ 
  aux = millis();
  rpm = 60.0/((aux - tempoRPM)/1000.0);
  tempoRPM = aux;
}

void interrupcaoVelocidade()
{
  aux = millis();
  velocidade = COMPRIMENTO * 3.6 / ((aux - tempoVelocidade)/1000.0);
  tempoVelocidade = aux;
}


void setup()
{
  Serial.begin(BAUDRATE);
  tempoRPM = millis();
  tempoVelocidade = millis();
  tempoCalculo = millis();

  attachInterrupt(PINO_RPM, interrupcaoRPM, RISING);
  attachInterrupt(PINO_VEL, interrupcaoVelocidade, RISING);
}

void loop()
{
  if(millis() - tempoCalculo > 500)
  {
    tempoCalculo = millis();
    Serial.print("V");
    Serial.print(velocidade);
    Serial.print("R");
    Serial.println(rpm);
    velocidade = 0;
    rpm = 0;
  }
}