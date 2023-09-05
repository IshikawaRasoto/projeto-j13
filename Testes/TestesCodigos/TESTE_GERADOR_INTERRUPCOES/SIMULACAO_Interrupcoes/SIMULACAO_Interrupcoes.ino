
hw_timer_t * rodaD = NULL;
hw_timer_t * rodaE = NULL;
hw_timer_t * eixoT = NULL;
hw_timer_t * rpm = NULL;

bool rodaD_state = false;
bool rodaE_state = false;
bool eixoT_state = false;
bool rpm_state = false;

#define PINO_RODAD 16
#define PINO_RODAE 17
#define PINO_EIXOT 18
#define PINO_RPM 19

void teste_rodaD(){
  if(rodaD_state)
    digitalWrite(PINO_RODAD, LOW);
  else
    digitalWrite(PINO_RODAD, HIGH);
    
  rodaD_state = !rodaD_state;
}

void teste_rodaE(){
  if(rodaE_state)
    digitalWrite(PINO_RODAE, LOW);
  else
    digitalWrite(PINO_RODAE, HIGH);
  
  rodaE_state = !rodaE_state;
}

void teste_eixoT(){
  if(eixoT_state)
    digitalWrite(PINO_EIXOT, LOW);
  else
    digitalWrite(PINO_EIXOT, HIGH);

  eixoT_state = !eixoT_state;
}

void teste_rpm(){
  if(rpm_state)
    digitalWrite(PINO_RPM, LOW);
  else
    digitalWrite(PINO_RPM, HIGH);

  rpm_state = !rpm_state;
}


void setup(){
  Serial.begin(115200);

  pinMode (PINO_RODAD, OUTPUT);
  pinMode (PINO_RODAE, OUTPUT);
  pinMode (PINO_EIXOT, OUTPUT);
  pinMode (PINO_RPM, OUTPUT);

  digitalWrite(PINO_RODAD, LOW);
  digitalWrite(PINO_RODAE, LOW);
  digitalWrite(PINO_EIXOT, LOW);
  digitalWrite(PINO_RPM, LOW);

  rodaD = timerBegin(0, 80, true);
  rodaE = timerBegin(1, 80, true);
  eixoT = timerBegin(2, 80, true);
  rpm = timerBegin(3, 80, true);

  timerAttachInterrupt(rodaD, &teste_rodaD, true);
  timerAttachInterrupt(rodaE, &teste_rodaE, true);
  timerAttachInterrupt(eixoT, &teste_eixoT, true);
  timerAttachInterrupt(rpm, &teste_rpm, true);

  timerAlarmWrite(rodaD, 25000/2, true); // RODA 40HZ - 50km/h
  timerAlarmWrite(rodaE, 30000/2, true); 
  timerAlarmWrite(eixoT, 100000/2, true); // Eixo traseiro 10Hz, 1/4 do dianteiro
  timerAlarmWrite(rpm, 20000/2, true); // 3000RPM

  timerAlarmEnable(rodaD);
  timerAlarmEnable(rodaE);
  timerAlarmEnable(eixoT);
  timerAlarmEnable(rpm);
}

void loop(){

}