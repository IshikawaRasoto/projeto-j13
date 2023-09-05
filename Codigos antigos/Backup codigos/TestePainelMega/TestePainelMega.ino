/*
 *  Código de funcionamento do Painel elétrico do J12
 *  Versão 1.5 
 *  Criado em 14/09 por Alexandre Chicora
 *  Input: I2C ECU Central
 *  Output: Barras de LED e display 7 segmentos para visualização dos dados do carro pelo piloto
 *  Repositório: github.com/
 */

#include <Wire.h> //biblioteca que implementa a I2C no arduino

// Pinos conectados nos transistores que fecham o circuito das barras de LED: RPM e combustível
#define BAR_RPM A0
#define BAR_CMB A4
// Pinos conectados no CI 4514 (Multiplexador das Barras de LED: RPM e combustível)
#define MULT_A 2 
#define MULT_B 3 
#define MULT_C A2  
#define MULT_D A3  
// Pinos conectados no CI 7447 (Codificação do display de 7 segmentos)
#define CI_DISPB A6
#define CI_DISPC A5
#define CI_DISPD A8
#define CI_DISPA A9
// Pinos que fecham o circuito do transistor dos displays de 7 segmentos (velocidade)
#define VEL_DISP1 4 //dezenas
#define VEL_DISP2 A1 //unidades
// Pinos dos LED's individuais
#define LED1 8
#define LED2 9
#define LED3 A7
#define LED4 19
#define LED5 A15
#define LED6 A14

#define IP 0x8 //endereço do slave para o qual o master da I2C enviara os dados

// Protótipos
void disp_velocidade(int);
void disp_rpm(int);
void disp_cmb(int);
void barraLeds(int);
void disp_led(int);
void inputI2C(int);
void TratarDados();

String BufDados = ""; //buffer de recebimento das mensagens via I2C
unsigned long int valor = 0;
bool flagBuff = false;
int delayVeloc = 4; //tempo que o display de velocidade fica ligado
int delayBarLED = 2; //tempo de delay entre os acionamentos das barras de LED (combustivel e rpm)

// Variáveis para salvar os ultimos valores
  int Comb = 12, Vel = 0, LED = 0, RPM = 1;

void setup() 
{
  Serial.begin(9600);
  Wire.begin(IP);    //testar sem input
  Wire.onReceive(inputI2C);//funcao alvo que sera executada quando o master enviar dados

  pinMode(BAR_RPM, OUTPUT);
  pinMode(BAR_CMB, OUTPUT);
  pinMode(MULT_A, OUTPUT);
  pinMode(MULT_B, OUTPUT);
  pinMode(MULT_D, OUTPUT);
  pinMode(MULT_C, OUTPUT);
  pinMode(CI_DISPB, OUTPUT);
  pinMode(CI_DISPA, OUTPUT);
  pinMode(CI_DISPD, OUTPUT);
  pinMode(CI_DISPC, OUTPUT);
  pinMode(VEL_DISP1, OUTPUT); //dezenas
  pinMode(VEL_DISP2, OUTPUT); //unidades
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  TestePainel();
   
}

void loop() 
{

  if(flagBuff)
  {
    TratarDados();
    flagBuff = false;
  }
  
  disp_cmb(Comb);
  disp_velocidade(Vel);
  disp_rpm(RPM);
  disp_led(LED);

}

void TestePainel()
{
  int cont = 0, barra = 1;

  digitalWrite(BAR_CMB, LOW);
  digitalWrite(BAR_RPM, LOW);
  
  digitalWrite(VEL_DISP2, LOW);
  digitalWrite(VEL_DISP1, LOW);
  for(cont; cont < 12; cont++)
  {
    disp_led(11);
    disp_led(21);
    disp_led(31);
    disp_led(41);
    disp_led(51);
    disp_led(61);

    digitalWrite(BAR_CMB, HIGH);
    digitalWrite(BAR_RPM, LOW);
    barraLeds(barra);
    
    digitalWrite(VEL_DISP1, LOW); 
    digitalWrite(VEL_DISP2, HIGH);
    codificacaoDisplay(cont);
    delay(200);
    
    digitalWrite(BAR_CMB, LOW);
    digitalWrite(BAR_RPM, HIGH);
    barraLeds(barra);
    barra++;
        
    disp_led(10);
    disp_led(20);
    disp_led(30);
    disp_led(40);
    disp_led(50);
    disp_led(60);
    
    digitalWrite(VEL_DISP1, HIGH);
    digitalWrite(VEL_DISP2, LOW);
    codificacaoDisplay(cont);
    delay(200);
  }

  // Piscada tripla final
  
  for(cont = 0; cont < 3; cont++)
  {
    digitalWrite(VEL_DISP1, HIGH);
    digitalWrite(VEL_DISP2, HIGH);
    disp_led(11);
    disp_led(21);
    disp_led(31);
    disp_led(41);
    disp_led(51);
    disp_led(61);
    digitalWrite(BAR_CMB, HIGH);
    digitalWrite(BAR_RPM, HIGH);
    delay(150);
    digitalWrite(BAR_CMB, LOW);
    digitalWrite(BAR_RPM, LOW);
    digitalWrite(VEL_DISP2, LOW);
    digitalWrite(VEL_DISP1, LOW);
    disp_led(10);
    disp_led(20);
    disp_led(30);
    disp_led(40);
    disp_led(50);
    disp_led(60);
    delay(150);
   }
}

void disp_velocidade(int velocidade)
{
    digitalWrite(VEL_DISP1, LOW);   //desativa o transitor do display das dezenas
    digitalWrite(VEL_DISP2, LOW);  //desativa o transistor do display das unidades
    int unidade = velocidade % 10; //separar o valor recebido em dezena e unidade
    int dezena = velocidade / 10;
    
    digitalWrite(VEL_DISP2, LOW);  //desativa o transistor que fecha o circuito do display da unidade
    digitalWrite(VEL_DISP1, HIGH); //ativa o transistor que fecha o circuito do display da dezena
    codificacaoDisplay(dezena);
    delay(delayVeloc);
    
    digitalWrite(VEL_DISP1, LOW);   //desativa o transitor do display das dezenas
    digitalWrite(VEL_DISP2, HIGH);  //ativa o transistor do display das unidades
    codificacaoDisplay(unidade);
    delay(delayVeloc);              //intervalo que ficara ligado
    
    digitalWrite(VEL_DISP1, LOW);   //desativa o transitor do display das dezenas
    digitalWrite(VEL_DISP2, LOW);   //desativa o transistor do display das unidades
}

void codificacaoDisplay(int valor)  //codificacao do display no CI 7447 de acordo com o valor recebido individualmente
{                                   // tabela verdade no datasheet do CI 7447
  switch(valor)
  {
    case 0:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 1:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break; 
    case 2:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break;  
    case 3:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 4:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 5:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
    case 6:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
   case 7:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, HIGH);
        digitalWrite(CI_DISPC, HIGH);
        digitalWrite(CI_DISPD, LOW);
        break;
   case 8:
        digitalWrite(CI_DISPA, LOW);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, HIGH);
        break;
  case 9:
        digitalWrite(CI_DISPA, HIGH);
        digitalWrite(CI_DISPB, LOW);
        digitalWrite(CI_DISPC, LOW);
        digitalWrite(CI_DISPD, HIGH);
        break;
  default:
        break;
  }
}

void disp_rpm(int nroLED)
{
  digitalWrite(BAR_CMB, LOW);
  digitalWrite(BAR_RPM, HIGH); //transistor do acionamento da barra de leds do RPM
  barraLeds(nroLED); //valor de 1 a 12 correspondente ao led que será acendido
  delay(delayBarLED);
}

void disp_cmb(int nroLED)
{
  digitalWrite(BAR_RPM, LOW);
  digitalWrite(BAR_CMB, HIGH);
  barraLeds(nroLED); //valor de 1 a 12 correspondente ao led que será acendido
  delay(delayBarLED);
}

void barraLeds(int nroLed)  //acende o respectivo led na barra de LED de acordo com a tabela de multiplexação do CI 4514
{                           //atentar para o mapeamento dos pinos da placa e no datasheet do CI (não são os mesmos)
  switch(nroLed)
  {  
    case 12:    
      digitalWrite(MULT_A, LOW);
      digitalWrite(MULT_B, LOW);
      digitalWrite(MULT_C, HIGH);
      digitalWrite(MULT_D, LOW);
      break;
    case 11:    
      digitalWrite(MULT_A, HIGH);
      digitalWrite(MULT_B, HIGH);
      digitalWrite(MULT_C, LOW);
      digitalWrite(MULT_D, LOW);
      break;
    case 10:   
      digitalWrite(MULT_A, HIGH);
      digitalWrite(MULT_B, LOW);
      digitalWrite(MULT_C, LOW);
      digitalWrite(MULT_D, LOW);
      break;
    case 9:    
      digitalWrite(MULT_A, LOW);
      digitalWrite(MULT_B, HIGH);
      digitalWrite(MULT_C, LOW);
      digitalWrite(MULT_D, LOW);
      break;
    case 8:    
      digitalWrite(MULT_A, LOW);
      digitalWrite(MULT_B, LOW);
      digitalWrite(MULT_C, LOW);
      digitalWrite(MULT_D, LOW);
      break;
   case 7:    
      digitalWrite(MULT_A, HIGH);
      digitalWrite(MULT_B, LOW);
      digitalWrite(MULT_C, HIGH);
      digitalWrite(MULT_D, LOW);
      break;
   case 6:    
      digitalWrite(MULT_A, LOW);
      digitalWrite(MULT_B, HIGH);
      digitalWrite(MULT_C, LOW);
      digitalWrite(MULT_D, HIGH);
      break;
   case 5:    
      digitalWrite(MULT_A, HIGH);
      digitalWrite(MULT_B, HIGH);
      digitalWrite(MULT_C, LOW);
      digitalWrite(MULT_D, HIGH);
      break;
   case 4:    
      digitalWrite(MULT_A, HIGH);
      digitalWrite(MULT_B, LOW);
      digitalWrite(MULT_C, HIGH);
      digitalWrite(MULT_D, HIGH);
      break;
   case 3:   
      digitalWrite(MULT_A, LOW);
      digitalWrite(MULT_B, LOW);
      digitalWrite(MULT_C, HIGH);
      digitalWrite(MULT_D, HIGH);
      break;
   case 2:    
      digitalWrite(MULT_A, LOW);
      digitalWrite(MULT_B, HIGH);
      digitalWrite(MULT_C, HIGH);
      digitalWrite(MULT_D, HIGH);
      break;  
   case 1:    
      digitalWrite(MULT_A, HIGH);
      digitalWrite(MULT_B, LOW);
      digitalWrite(MULT_C, LOW);
      digitalWrite(MULT_D, HIGH);
      break;
  }
}
void disp_led(int nLED)
{ 
  int nLED1 = 0, nLED2 = 0;
  nLED1 = nLED / 10; //primeiro bit ; nro do led que irá acender ou apagar
  nLED2 = nLED % 10; //segundo bit ; se for igual a 1 acende, se for igual a 0 apaga    
  
  switch(nLED1) //qual led irá acender ou apagar
  {
    case 1:
      digitalWrite(LED1, nLED2);
      break;
    case 2:
      digitalWrite(LED2, nLED2);
      break;
    case 3:
      digitalWrite(LED3, nLED2);
      break;  
    case 4:
      digitalWrite(LED4, nLED2);
      break;      
    case 5:
      digitalWrite(LED5, nLED2);
      break;  
    case 6:
      digitalWrite(LED6, nLED2);
      break;
  }
}

void inputI2C(int nEventos)
{
  BufDados = "";
  while(1 < Wire.available()) // Leitura de cada byte
  { 
    char c = (char)Wire.read();
//    Serial.print("C: ");
//    Serial.print(c);
    BufDados += c;    
    // valor = (int) Wire.read();   
  }
//  Serial.println();
//  Serial.print("BUF: ");
//  Serial.println(BufDados);
  char lixo = Wire.read();
  if(lixo == ';')
    flagBuff = true;
  // TratarDados();
}

void TratarDados()
{
  char aux = BufDados[1];
//  Serial.print("AUX: ");
//  Serial.println(aux);
  // int valor = 0;
  char dado = (char)BufDados[0];
  valor = (int) aux;
//  Serial.print("dado: ");
  Serial.print(dado);
//  Serial.print("valor: ");
  Serial.print(valor);
  Serial.print(";");

  switch(dado)
  {
    case 'C':
      Comb = valor;
      break;
    case 'L':
      LED = valor;
      break;
    case 'V':
      Vel = valor;
      break;
    case 'R':
      RPM = valor;
      break;
  }
}
