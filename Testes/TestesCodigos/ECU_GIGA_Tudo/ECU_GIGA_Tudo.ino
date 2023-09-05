/*BIBLIOTECAS:

  Acelerometro -> MPU6050 Adafruit
  Infravermelho -> MLX90614 Adafruit
  GPS -> Tiny GPS (Verificar se é a melhor opcao) - Peguei o exemplo simple_test

*/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 
#include <Adafruit_MLX90614.h>
#include <TinyGPS.h>

Adafruit_MPU6050 mpu;


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

TinyGPS gps;

#define RX1 32
#define TX1 33

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RX1, TX1);

  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); 
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  mlx.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC());

  Serial.println("");

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  while (Serial1.available())
    {
      char c = Serial1.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }
  
  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");

  delay(500);
  
}

