unsigned long tempo;

uint8_t velocidade = 0;
uint16_t rpm = 0;
float latitude = 0.0;
float longitude = 0.0;

void setup()
{
  Serial.begin(115200);

  tempo = millis();
}

void loop()
{
  if((millis() - tempo) > 500)
  {  
    tempo = millis();
    
    velocidade += 1;
    if(velocidade > 50)
      velocidade = 0;

    rpm += 100;
    if(rpm > 4500)
      rpm = 0;

    latitude += 0.5;
    if(latitude > 30.0)
      latitude = -30.0;

    longitude += 0.5;
    if (longitude > 30.0)
      longitude = -30.0;

    Serial.print("Vel:");
    Serial.print(velocidade);
    Serial.print("RPM:");
    Serial.print(rpm);
    Serial.print("LAT:");
    Serial.print(latitude);
    Serial.print("LNG:");
    Serial.println(longitude);
  }
    
}