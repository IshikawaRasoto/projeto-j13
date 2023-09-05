#include <TinyGPSPlus.h>

#define RXPIN 4
#define TXPIN 2

#define GPS_BAUD 9600

TinyGPSPlus gps;

double latitude_atual = 0;
double longitude_atual = 0;
double velocidade = 0;
double altitude_metros = 0;
double curso = 0;

bool imprimir = false;

hw_timer_t * timer = NULL;

void funcao_timer(){
  imprimir = true;
}

void setup(){
  Serial.begin(115200);
  Serial1.begin(GPS_BAUD, SERIAL_8N1, RXPIN, TXPIN);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &funcao_timer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
}

void loop(){

  while(Serial1.available()){
    char c = Serial1.read();
    gps.encode(c);
    if(gps.location.isUpdated()){
      latitude_atual = gps.location.lat();
      longitude_atual = gps.location.lng();
      velocidade = gps.speed.kmph();
      altitude_metros = gps.altitude.meters();
      curso = gps.course.deg();
    }

  }

  if(imprimir){
    imprimir = false;

    Serial.print("Latitude: ");
    Serial.print(latitude_atual);
    Serial.print(" | Longitude: ");
    Serial.print(longitude_atual);
    Serial.print(" | Velocidade: ");
    Serial.print(velocidade, 2);
    Serial.print(" km/h | Curso: ");
    Serial.print(curso);
    Serial.print(" º | Altitude: ");
    Serial.println(altitude_metros);
  }
}