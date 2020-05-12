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

#include <String.h>

RF24 radio(9, 10);
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL}; // 0xE8E8F0F0E1LL;

const int PIN_DTH11 = 7;
DHT11 dht11(PIN_DTH11);
 
const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);
 
int err;
int newTemp = 0, newHum = 0;

float ref_ang_x, ref_ang_y;

int bat = 0;

int alarm = 0;
char data[3] = "off";

int state = 0;

void setup(void)
{
//  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);

//  printf_begin();
//  radio.printDetails();
}
 
void loop(void)
{
  int ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
 
  float ang_x = atan(ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14);
  float ang_y = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14);

//  float hum, temp;
//  if((err = dht11.read(hum, temp)) == 0){ // Si devuelve 0 ha leido bien
//    newTemp = temp;
//    newHum = hum;
//  }

  // TODO: Read bat level
  
  if (radio.available())
  {   
    radio.read(data, sizeof data);
//    Serial.print("data: ");
//    Serial.print(data[0]);
//    Serial.print(data[1]);
//    Serial.println(data[2]);
  }

  switch (state) {
    case 0: // Alarma desactivada
      Serial.println("State 0");
      if(data[0] == 'o' && data[1] == 'n'){
        ref_ang_x = ang_x;
        ref_ang_y = ang_y;
        state = 1;
        cleanData();
      }else if(data[0] == 's' && data[1] == 'i' && data[2] == 'l'){
        ref_ang_x = ang_x;
        ref_ang_y = ang_y;
        state = 2;
        cleanData();
      }
      break;
    case 1: // Alarma activada
      Serial.println("State 1");
      if( (abs(ref_ang_x - ang_x) > 20) || (abs(ref_ang_y - ang_y) > 20) ){
        alarm = 1;
        activateAlarmSound();
        ref_ang_x = ang_x;
        ref_ang_y = ang_y;
      }
      if(data[0] == 'o' && data[1] == 'f' && data[2] == 'f'){
        ref_ang_x = 0;
        ref_ang_y = 0;
        state = 0;
        alarm = 0;
        cleanData();
      }
      break;
    case 2: // Alarma silenciosa
      Serial.println("State 2");
      if( (abs(ref_ang_x - ang_x) > 20) || (abs(ref_ang_y - ang_y) > 20) ){
        alarm = 1;
        ref_ang_x = ang_x;
        ref_ang_y = ang_y;
      }
      if(data[0] == 'o' && data[1] == 'f' && data[2] == 'f'){
        ref_ang_x = 0;
        ref_ang_y = 0;
        state = 0;
        alarm = 0;
        cleanData();
      }
      break;
  }

  char msgToSend[14];
  createMSG(msgToSend, alarm, state, 15, 15, 98); // alarm, state, newTemp, newHum, bat);

//  Serial.print("msg: ");
//  Serial.println(msgToSend);

  radio.stopListening();
  radio.write(msgToSend, 14);
  radio.startListening();

  if(state == 0){
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF); 
  }else{
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  }
  
}

// ----------------------------------------------------------------------------------------------------
void cleanData(){
  data[0] = "";
  data[1] = "";
  data[2] = ""; 
}

// ----------------------------------------------------------------------------------------------------
void createMSG(char * _msg, int _alarm, int _mode, int _temp, int _hum, int _bat){
  String alarm, mode, temp, hum, bat;
  alarm = (String)_alarm;
  mode = (String)_mode;
  temp = (String)_temp;
  hum = (String)_hum;
  bat = (String)_bat;

  String result;
  result = alarm + "/" + mode + "/" + temp + "/" + hum + "/" + bat + "/";
  result.toCharArray(_msg, 14);
}

// ----------------------------------------------------------------------------------------------------
void activateAlarmSound(){
  
}
