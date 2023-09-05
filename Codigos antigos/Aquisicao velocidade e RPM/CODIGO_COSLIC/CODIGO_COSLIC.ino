#define PIN_RPM_A 6
#define PIN_RPM_B 3
volatile int rpmA=0;
volatile int rpmB=0;
volatile float vel=0;
volatile int temp=0;

void setup()
{
  attachInterrupt(digitalPinToInterrupt(PIN_RPM_A), conta_A, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_RPM_B), conta_B, RISING);
  pinMode(3,INPUT);
  pinMode(6,INPUT);
  SERIAL_PORT_MONITOR.begin(115200);
}
void loop(){
  delay(1000);
  Serial.print("RPM A:");
  Serial.println((unsigned char) rpmA*60);
  Serial.print("Velocidade:");
  vel = ((rpmB/7)*5.99)/60;
  Serial.println((unsigned char) vel);
  Serial.print("RPM B:");
  Serial.println((unsigned char) rpmB*60);
  rpmA = 0;
  rpmB = 0;
}

void conta_A(){
  rpmA++;
}
void conta_B(){
  rpmB++;
}
