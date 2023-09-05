#define PIN_RPM_M 19
#define PIN_RPM_I 18
#define taxa_atualizacao 3 // Em Hz

volatile int rpmM=0; // Rotacoes Por Minuto
int rpdM = 0; //Rotacoes Por Delay
volatile int rpmI=0;
int rpdI = 0;

float vel = 0;

volatile int temp=0;
volatile float velPas=0;

//int teste = 4000;

void conta_motor(){
  rpdM++;
}
void conta_indutivo(){
  rpdI++;
}

float calculo_Vel()
{
  vel = (((rpmI/7)*1.66)/60)*3.6; // rpmI / 7 -> rpm na roda -> *1.66 comprimento da roda -> /60 m/s -> *3.6 -> Km/h

  return(vel);
}


void setup()
{
  pinMode(PIN_RPM_M,INPUT);
  pinMode(PIN_RPM_I,INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_RPM_M), conta_motor, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_RPM_I), conta_indutivo, RISING);

  Serial.begin(115200);
}
void loop(){
  delay(1000/taxa_atualizacao);

  rpmM = rpdM*(60*taxa_atualizacao);
  rpmI = rpdI*(60*taxa_atualizacao);

  
  Serial.print("M: ");
  Serial.println(rpmM);
  //Serial.println(digitalRead(PIN_RPM_M));

  Serial.print("I: ");
  Serial.println(rpmI);
 // Serial.println(digitalRead(PIN_RPM_I));  

   Serial.print("V: ");
   Serial.println(calculo_Vel());

  Serial.println("");

 // Serial.println(digitalRead(3));

  rpdM = 0;
  rpdI = 0;
}

