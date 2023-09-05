#define PIN_RPM 3

#define DIAMETRO 0.51916  // Diametro efetivo da roda em metros
#define COMPRIMENTO (PI * DIAMETRO) // Comprimento da roda
#define DENTES 1 // Dentes na roda fônica que vão ser lidos pelo sensor

#define hz 3 // Taxa de atualizacao dos dados

// Contadores
unsigned int contador_rpm = 0;

// Velocidade
volatile unsigned int RPM_Homo = 0; // Variável para salvar o RPM da homocinetica

// Timers
unsigned long int tempo = 0;
//hw_timer_t* timer_500ms = NULL; 

void interrupcao_RPM(){contador_rpm++;}

void calculo_RPM() 
{
  RPM_Homo = contador_rpm * 2 * 60;
  contador_rpm = 0;
}

ISR(TIMER1_COMPA_vect){

  cli(); // Desativação de outras interrupções para evitar problemas de "core panic"
  TCNT1  = 0;

  calculo_RPM(); 

  sei(); // Reabilita Interrupções.
}

void start_timer(){

  cli();                      //stop interrupts for till we make the settings
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
 
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescalar 256  
  
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
  
  /*4. Set the value of register A to 31250*/
  OCR1A = 31250;             //Finally we set compare register A to this value  
  sei();                     //Enable back the interrupts
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(PIN_RPM, INPUT);

  tempo = millis();

  attachInterrupt(digitalPinToInterrupt(PIN_RPM), interrupcao_RPM, RISING); 
  start_timer();
}

void loop() {

  if(millis() - tempo >= 1000/hz) // Leitura de dados a cada "tantos" milissegundos //Se o resto da divisão de Tempo por "tantos" milissegundos for igual a 0 ...
  {
    
    Serial.print("RPM: ");
    Serial.println(RPM_Homo);

    Serial.println("");

    tempo = millis();  
  
}
}