
void setup() {
  // put your setup code here, to run once:

  pinMode(4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //35Hz e 0.1% DutyCycle

  digitalWrite(4, HIGH);
  delayMicroseconds(28/2);
  digitalWrite(4, LOW);
  delayMicroseconds(28458/2);

}

