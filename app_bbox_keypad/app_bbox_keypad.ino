#include <Key.h>
#include <Keypad.h>

#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

#include <LowPower.h>

const byte countRows = 4;
const byte countColumns = 3;
 
const byte rowsPins[countRows] = { 3, 4, 5, 6};
const byte columnsPins[countColumns] = { 7, 8, 9};
 
char keys[countRows][countColumns] = {
  { '1','2','3'},
  { '4','5','6'},
  { '7','8','9'},
  { '*','0','#'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowsPins, columnsPins, countRows, countColumns);
 
const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);

const int PIN_BUT = 2;

char pwd[4];
int cont = 0;

float ref_ang_x = 0.0;
float ref_ang_y = 0.0;

bool activate = false;
int state = 0;

bool sleep = false;
 
void setup() {
  Serial.begin(115200);

  Wire.begin();
  mpu.initialize();

  pinMode(PIN_BUT, INPUT_PULLUP);

  attachInterrupt(0, wakeUp, LOW);

  pwd[0] = '0';
  pwd[1] = '0';
  pwd[2] = '0';
  pwd[3] = '0';
}
 
void loop() {
  if(sleep){
    Serial.println("Sleep");
    delay(1000);
    sleep = !sleep;
  }
  
  char key = keypad.getKey();

  int ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
 
  float ang_x = atan(ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14);
  float ang_y = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14);
  
  if(key > 0){
     switch (key) {
      case '*':
        pwd[0] = '0';
        pwd[1] = '0';
        pwd[2] = '0';
        pwd[3] = '0';
        cont = 0;
        break;
      case '#':
        activate = checkPWD();
        break;
      default:
        pwd[cont] = key;
        cont++;
        if(cont > 4){
          cont = 0;
        }
        break;
    } 
  }

  switch (state) {
    case 0: // Alarma desactivada
      if(activate){
        activate = false;
        ref_ang_x = ang_x;
        ref_ang_y = ang_y;
        state = 1;
      }

      break;
    case 1: // Alarma activada
      if( (abs(ref_ang_x - ang_x) > 20) || (abs(ref_ang_y - ang_y) > 20) ){
        activateAlarmSound(); 
      }
      if(activate){
        activate = false;
        ref_ang_x = 0.0;
        ref_ang_y = 0.0;
        state = 0;
      }
      break;
  }
    
}

// ----------------------------------------------------------------------------------------------------
void wakeUp()
{

//  while(!digitalRead(PIN_BUT));
//  while(digitalRead(PIN_BUT));
  
  sleep = !sleep;

}


// ----------------------------------------------------------------------------------------------------
void printPWD(){
  for(int i = 0; i < 4; i++){
    Serial.print(pwd[i]);
  }
  Serial.println("");
}

// ----------------------------------------------------------------------------------------------------
bool checkPWD(){
  if(pwd[0] == '1' && pwd[1] == '5' && pwd[2] == '0' && pwd[3] == '5'){
    return true;
  }else{
    return false;
  }
}

// ----------------------------------------------------------------------------------------------------
void activateAlarmSound(){
  
}
