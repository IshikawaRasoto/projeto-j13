/***************************************************
  This is a library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1747 3V version
  ----> https://www.adafruit.com/products/1748 5V version

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Adafruit MLX90614 test");

  mlx.begin();
  mpu.begin();

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  

}

void loop() {
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");
  

  Serial.println();
  delay(500);
}
