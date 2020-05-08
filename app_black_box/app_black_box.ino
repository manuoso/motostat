#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

#include <DHT11.h>

#include <LowPower.h>

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

const int PIN_DTH11 = 7;
DHT11 dht11(PIN_DTH11);
 
const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);
 
int ax, ay, az;

char data[16]="Hola mundo" ;
 
void setup(void)
{
  Wire.begin();
  mpu.initialize();
  
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);
  
  radio.openWritingPipe(pipe);
}
 
void loop(void)
{
  radio.write(data, sizeof data);

  // Leer las aceleraciones 
  mpu.getAcceleration(&ax, &ay, &az);
 
  // Calcular los angulos de inclinacion
  float accel_ang_x = atan(ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14);
  float accel_ang_y = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14);

  int err;
  float temp, hum;
  if((err = dht11.read(hum, temp)) == 0){ // Si devuelve 0 ha leido bien
    // TODO
  }else{
    // TODO
  }
  delay(1000); // Solo lee una vez por segundo  

}
