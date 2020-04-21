#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_PCD8544.h>
 
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

#include <DHT11.h>

#include <LowPower.h>


const int PIN_DTH11 = 2;
DHT11 dht11(PIN_DTH11);
 
const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);
 
int ax, ay, az;

const int PIN_RESET = 3;  // LCD1 Reset
const int PIN_SCE = 4;    // LCD2 Chip Select
const int PIN_DC = 5;     // LCD3 Dat/Command
const int PIN_SDIN = 6;   // LCD4 Data in
const int PIN_SCLK = 7;   // LCD5 Clk
                          // LCD6 Vcc
const int PIN_VLED = 8;   // LCD7 Vled
                          // LCD8 Gnd

const int PIN_BUT_DIS = 9;

Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_SCLK, PIN_SDIN, PIN_DC, PIN_SCE, PIN_RESET);
 
#define LOGO_MOTO_HEIGHT 30
#define LOGO_MOTO_WIDTH  32
 
static const unsigned char PROGMEM logo_moto_bmp[] = { 
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000001,B11111000,B00000000,
  B00000000,B00000001,B11110100,B00000000,
  B00000011,B11111111,B11110100,B00000000,
  B00000011,B11111111,B11110100,B00000000,
  B00000000,B00011111,B11110100,B00000000,
  B00000000,B00000001,B11111100,B00000000,
  B00000000,B00000000,B11111000,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00001110,B00000000,B00000000,
  B00000000,B00001111,B00000000,B00000000,
  B00000000,B00000011,B00000000,B00000000,
  B00000000,B00000110,B00000000,B00000000,
  B00000000,B00001101,B10000000,B00000000,
  B00000000,B00001111,B11000000,B00000000,
  B00000000,B00001111,B11100000,B00000000,
  B00000000,B11111101,B11100000,B00000000,
  B00000000,B11111000,B11110000,B00000000,
  B00000000,B00000000,B11110000,B00000000,
  B00000000,B00000111,B11100000,B00000000,
  B00000011,B11000111,B00001111,B10000000,
  B00000110,B01100111,B00011000,B11000000,
  B00000100,B00110011,B00010000,B01000000,
  B00001100,B00010011,B10010000,B01000000,
  B00000100,B00010001,B10010000,B01000000,
  B00000110,B00110001,B10011000,B11000000,
  B00000011,B11100000,B00001111,B10000000,
  B00000000,B00000000,B00000000,B00000000,
};

#define LOGO_WIFI_HEIGHT 24
#define LOGO_WIFI_WIDTH  32

static const unsigned char PROGMEM logo_wifi_bmp[] = {
  B00000000,B00001111,B11110000,B00000000,
  B00000000,B11111111,B11111111,B00000000,
  B00000011,B11111111,B11111111,B11000000,
  B00000111,B11111100,B00111111,B11100000,
  B00011111,B11000000,B00000011,B11111000,
  B00111111,B00000000,B00000000,B11111100,
  B01111100,B00000000,B00000000,B00111110,
  B11111000,B00001111,B11110000,B00011111,
  B01110000,B01111111,B11111110,B00001110,
  B01100000,B11111111,B11111111,B00000110,
  B00000001,B11111000,B00011111,B10000000,
  B00000011,B11100000,B00000111,B11000000,
  B00000111,B11000000,B00000011,B11100000,
  B00000111,B00000000,B00000000,B11100000,
  B00000011,B00001111,B11110000,B11000000,
  B00000000,B00011111,B11111000,B00000000,
  B00000000,B00111111,B11111100,B00000000,
  B00000000,B01111100,B00111110,B00000000,
  B00000000,B00110000,B00001100,B00000000,
  B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000001,B10000000,B00000000,
  B00000000,B00000011,B11000000,B00000000,
  B00000000,B00000011,B11000000,B00000000,
  B00000000,B00000001,B10000000,B00000000,
};

#define LOGO_THIEF_HEIGHT 32
#define LOGO_THIEF_WIDTH  64

static const unsigned char PROGMEM logo_thief_bmp[] = {
  B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00000111,B10000000,B11110000,B00000000,B00000000,
  B00000000,B00000000,B00000000,B00001111,B11000001,B11110000,B00000000,B00000000,
  B00000000,B00000000,B10000000,B00001111,B11000001,B11111000,B00000000,B00000000,
  B00000001,B00000000,B00000000,B00000111,B10000000,B11110000,B00000000,B00000000,
  B00001111,B11000000,B00000000,B00000011,B00000000,B01100000,B00000000,B00000000,
  B00011111,B00000001,B00000000,B00000001,B11000011,B10000000,B00000000,B00000000,
  B00011110,B00000000,B00000001,B10000111,B11100111,B11000000,B00000000,B00000000,
  B00011100,B00000001,B10000100,B11111111,B11100111,B11100000,B00000000,B00000000,
  B00001000,B01000011,B00001110,B01111111,B11111111,B11110000,B00000000,B00000000,
  B00011000,B00001110,B00011111,B10000011,B11111111,B11111001,B00000000,B00000000,
  B00000011,B11111100,B00111111,B10000011,B11111111,B11111111,B00000000,B00000000,
  B00001111,B11111000,B00011111,B10000011,B11100111,B11001111,B10110000,B00000000,
  B00011111,B11000000,B00001111,B00000011,B11100111,B11001111,B11110000,B00000000,
  B00111111,B11000000,B00000010,B00000011,B11111111,B11101111,B11110000,B00000000,
  B00111111,B11000000,B00000000,B00000001,B11111111,B11110111,B11110000,B00000000,
  B01110111,B11100000,B00000000,B00001111,B11111101,B11111011,B11111111,B00000000,
  B01100111,B11110000,B00000000,B00111111,B11111111,B11011111,B01111111,B11000000,
  B11000111,B11110000,B00000000,B01110011,B11110111,B11101110,B11111001,B11100000,
  B10001111,B11111000,B00000000,B01100111,B11110111,B11111100,B11011100,B01100000,
  B00001111,B11111100,B00000000,B11000111,B11111111,B11111101,B10011100,B01110000,
  B00001111,B11111110,B00000000,B11001111,B11111111,B11111001,B10000000,B00110000,
  B00001111,B11111110,B00000000,B11000111,B11111111,B11110001,B10000000,B00110000,
  B00001111,B11111111,B00000000,B11000000,B00011000,B00000001,B10000000,B00110000,
  B00011111,B11111111,B10000000,B11100000,B00111000,B00000001,B11000000,B01110000,
  B00000111,B00001100,B00000000,B01100000,B01110000,B00000000,B11000000,B11100000,
  B00000111,B00001100,B00000000,B01111000,B11100000,B00000000,B11110001,B11000000,
  B00001110,B00001100,B00000000,B00011111,B11000000,B00000000,B01111111,B10000000,
  B00001100,B00001100,B00000000,B00000111,B00000000,B00000000,B00011110,B00000000,
  B00011000,B00001100,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
  B00000000,B00001000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,
};

//void setup()
//{
//   Serial.begin(9600);
//   Wire.begin();
//   mpu.initialize();
//   Serial.println(mpu.testConnection() ? F("IMU iniciado correctamente") : F("Error al iniciar IMU"));
//}
//
//void loop(){
//  
//  // Leer las aceleraciones 
//  mpu.getAcceleration(&ax, &ay, &az);
// 
//  //Calcular los angulos de inclinacion
//  float accel_ang_x = atan(ax / sqrt(pow(ay, 2) + pow(az, 2)))*(180.0 / 3.14);
//  float accel_ang_y = atan(ay / sqrt(pow(ax, 2) + pow(az, 2)))*(180.0 / 3.14);
// 
//  // Mostrar resultados
//  Serial.print(F("Inclinacion en X: "));
//  Serial.print(accel_ang_x);
//  Serial.print(F("\tInclinacion en Y:"));
//  Serial.println(accel_ang_y);
//  delay(10);
//
//  int err;
//  float temp, hum;
//  if((err = dht11.read(hum, temp)) == 0){ // Si devuelve 0 ha leido bien
//      Serial.print("Temperatura: ");
//      Serial.print(temp);
//      Serial.print(" Humedad: ");
//      Serial.print(hum);
//      Serial.println();
//  }else{
//      Serial.println();
//      Serial.print("Error Num :");
//      Serial.print(err);
//      Serial.println();
//  }
//  delay(1000);            //Recordad que solo lee una vez por segundo      
//}


int state = 0;
unsigned long mainStart = 0;
bool mainRunning = true;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_BUT_DIS, INPUT_PULLUP); 
  
  pinMode(PIN_VLED, OUTPUT);
  digitalWrite(PIN_VLED, LOW);
  
  display.begin();

  mainStart = millis();
}
 
void loop() {

  int pul = checkButton();

  if(mainRunning && (millis() - mainStart ) >= 10000){
    mainRunning = false;
    state = 1;
  }
  
  switch (state) {
    case 0: // init display
      printInitDisplay();
      if(pul == 4){
        state = 1;
      }else if(pul == 1){
        state = 3;
      }
      break;
    case 1: // sleep display
      display.clearDisplay();
      display.display();
      digitalWrite(PIN_VLED, HIGH);
      state = 2;
      break;
    case 2: // waiting for awake in sleep display
      LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF); 
      if(pul == 1){
        digitalWrite(PIN_VLED, LOW);
        mainRunning = true;
        mainStart = millis();
        state = 0;
      }
      break;
    case 3: // menu status display
      printMenuDisplay(true, 69, 69, 99, 99, "off");
      if(pul == 4){
        state = 1;
      }
      break;
    case 4: // thief display
      printThiefDisplay();
      delay(300);
      display.clearDisplay();
      display.display();
      break;
  }

  delay(100);
}


// ----------------------------------------------------------------------------------------------------
void printInitDisplay(){
  display.clearDisplay();
  display.setTextSize(1);
  
  display.setTextColor(BLACK);
  display.setCursor(6, 0);
  display.print("MotoStat App");

  display.setCursor(0, 40);
  display.print("Clic for Menu!");

  display.drawBitmap(8, 12, logo_wifi_bmp, LOGO_WIFI_WIDTH, LOGO_WIFI_HEIGHT, BLACK);
  
  display.drawBitmap(45, 9, logo_moto_bmp, LOGO_MOTO_WIDTH, LOGO_MOTO_HEIGHT, BLACK);
  
  display.display();
}

// ----------------------------------------------------------------------------------------------------
void printMenuDisplay(bool _rf, int _tmp, int _hum, int _bat1, int _bat2, char _mod[4]){
  display.clearDisplay();
  display.setTextSize(1);

  // signal status display
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.print("RF Signal");
  display.setCursor(57, 1);
  display.write(26); // -> simbol
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.drawPixel(65,0, BLACK);
  display.drawPixel(65,1, BLACK);
  display.drawPixel(65,2, BLACK);
  display.drawPixel(65,3, BLACK);
  display.drawPixel(65,4, BLACK);
  display.drawPixel(65,5, BLACK);
  display.drawPixel(65,6, BLACK);
  display.drawPixel(65,7, BLACK);
  display.setCursor(66, 0);
  if(_rf){
    display.print("on");
  }else{
    display.print("off");
  }
  display.setTextColor(BLACK);

  // tmp status display
  display.setCursor(0, 10);
  display.print("Tmp");
  display.setCursor(22, 11);
  display.write(26); // -> simbol
  display.setTextColor(WHITE, BLACK);
  display.drawPixel(32,10, BLACK);
  display.drawPixel(32,11, BLACK);
  display.drawPixel(32,12, BLACK);
  display.drawPixel(32,13, BLACK);
  display.drawPixel(32,14, BLACK);
  display.drawPixel(32,15, BLACK);
  display.drawPixel(32,16, BLACK);
  display.drawPixel(32,17, BLACK);
  display.drawPixel(32,9, BLACK);
  display.drawPixel(33,9, BLACK);
  display.drawPixel(34,9, BLACK);
  display.drawPixel(35,9, BLACK);
  display.drawPixel(36,9, BLACK);
  display.drawPixel(37,9, BLACK);
  display.drawPixel(38,9, BLACK);
  if(_tmp/10 > 0){
    display.drawPixel(39,9, BLACK);
    display.drawPixel(40,9, BLACK);
    display.drawPixel(41,9, BLACK);
    display.drawPixel(42,9, BLACK);
    display.drawPixel(43,9, BLACK);
    display.drawPixel(44,9, BLACK);
  }
  display.setCursor(33, 10);
  display.print(_tmp);
  display.setTextColor(BLACK);
  display.setCursor(48, 10);
  display.write(9); // º simbol
  display.setCursor(54, 10);
  display.print("C"); 

  // hum status display
  display.setTextColor(BLACK);
  display.setCursor(0, 20);
  display.print("Hum");
  display.setCursor(22, 21);
  display.write(26); // -> simbol
  display.setTextColor(WHITE, BLACK);
  display.drawPixel(32,20, BLACK);
  display.drawPixel(32,21, BLACK);
  display.drawPixel(32,22, BLACK);
  display.drawPixel(32,23, BLACK);
  display.drawPixel(32,24, BLACK);
  display.drawPixel(32,25, BLACK);
  display.drawPixel(32,26, BLACK);
  display.drawPixel(32,27, BLACK);
  display.drawPixel(32,19, BLACK);
  display.drawPixel(33,19, BLACK);
  display.drawPixel(34,19, BLACK);
  display.drawPixel(35,19, BLACK);
  display.drawPixel(36,19, BLACK);
  display.drawPixel(37,19, BLACK);
  display.drawPixel(38,19, BLACK);
  if(_hum/10 > 0){
    display.drawPixel(39,19, BLACK);
    display.drawPixel(40,19, BLACK);
    display.drawPixel(41,19, BLACK);
    display.drawPixel(42,19, BLACK);
    display.drawPixel(43,19, BLACK);
    display.drawPixel(44,19, BLACK);
  }
  display.setCursor(33, 20);
  display.print(_hum);
  display.setTextColor(BLACK);
  display.setCursor(50, 20);
  display.print("%"); 

  // bat status display
  display.setCursor(0, 30);
  display.print("Bat R|T");
  display.setCursor(44, 31);
  display.write(26); // -> simbol
  display.setTextColor(WHITE, BLACK);
  display.drawPixel(52,30, BLACK);
  display.drawPixel(52,31, BLACK);
  display.drawPixel(52,32, BLACK);
  display.drawPixel(52,33, BLACK);
  display.drawPixel(52,34, BLACK);
  display.drawPixel(52,35, BLACK);
  display.drawPixel(52,36, BLACK);
  display.drawPixel(52,37, BLACK);
  display.drawPixel(52,29, BLACK);
  display.drawPixel(53,29, BLACK);
  display.drawPixel(54,29, BLACK);
  display.drawPixel(55,29, BLACK);
  display.drawPixel(56,29, BLACK);
  display.drawPixel(57,29, BLACK);
  display.drawPixel(58,29, BLACK);
  display.drawPixel(59,29, BLACK);
  display.drawPixel(60,29, BLACK);
  display.drawPixel(61,29, BLACK);
  display.drawPixel(62,29, BLACK);
  display.drawPixel(63,29, BLACK);
  display.drawPixel(64,29, BLACK);
  display.drawPixel(65,29, BLACK);
  display.drawPixel(66,29, BLACK);
  display.drawPixel(67,29, BLACK);
  display.drawPixel(68,29, BLACK);
  display.drawPixel(69,29, BLACK);
  display.drawPixel(70,29, BLACK);
  display.drawPixel(71,29, BLACK);
  display.drawPixel(72,29, BLACK);
  display.drawPixel(73,29, BLACK);
  display.drawPixel(74,29, BLACK);
  display.drawPixel(75,29, BLACK);
  display.drawPixel(76,29, BLACK);
  display.drawPixel(77,29, BLACK);
  display.drawPixel(78,29, BLACK);
  display.drawPixel(79,29, BLACK);
  display.drawPixel(80,29, BLACK);
  display.drawPixel(81,29, BLACK);
  display.drawPixel(82,29, BLACK);
  display.drawPixel(83,29, BLACK);
  display.setCursor(53, 30);
  display.print(_bat1);
  display.print("|");
  display.print(_bat2);
  
  // mode status display
  display.setTextColor(BLACK);
  display.setCursor(0, 40);
  display.print("Mode");
  display.setCursor(28, 41);
  display.write(26); // -> simbol
  display.setTextColor(WHITE, BLACK);
  display.drawPixel(38,40, BLACK);
  display.drawPixel(38,41, BLACK);
  display.drawPixel(38,42, BLACK);
  display.drawPixel(38,43, BLACK);
  display.drawPixel(38,44, BLACK);
  display.drawPixel(38,45, BLACK);
  display.drawPixel(38,46, BLACK);
  display.drawPixel(38,47, BLACK);
  display.setCursor(39, 40);
  display.print(_mod);
  display.setTextColor(BLACK);
  
  display.display();
}

// ----------------------------------------------------------------------------------------------------
void printThiefDisplay(){
  display.clearDisplay();
  display.setTextSize(2);

  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 0);
  display.write(42);
  display.print("thief");
  display.write(42);

  display.drawBitmap(12, 16, logo_thief_bmp, LOGO_THIEF_WIDTH, LOGO_THIEF_HEIGHT, BLACK);

  display.display();
}

// ----------------------------------------------------------------------------------------------------
//  MULTI-CLICK:  One Button, Multiple Events
// From: https://forum.arduino.cc/index.php?topic=14479.0

// Button timing variables
int debounce = 20;          // ms debounce period to prevent flickering when pressing or releasing the button
int DCgap = 250;            // max ms between clicks for a double click event
int holdTime = 1000;        // ms hold period: how long to wait for press+hold event
int longHoldTime = 3000;    // ms long hold period: how long to wait for press+hold event

// Button variables
boolean buttonVal = HIGH;   // value read from button
boolean buttonLast = HIGH;  // buffered value of the button's previous state
boolean DCwaiting = false;  // whether we're waiting for a double click (down)
boolean DConUp = false;     // whether to register a double click on next release, or whether to wait and click
boolean singleOK = true;    // whether it's OK to do a single click
long downTime = -1;         // time the button was pressed down
long upTime = -1;           // time the button was released
boolean ignoreUp = false;   // whether to ignore the button release because the click+hold was triggered
boolean waitForUp = false;        // when held, whether to wait for the up event
boolean holdEventPast = false;    // whether or not the hold event happened already
boolean longHoldEventPast = false;// whether or not the long hold event happened already

int checkButton() {    
  int event = 0;
  buttonVal = digitalRead(PIN_BUT_DIS);
  // Button pressed down
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - upTime) > debounce)
  {
    downTime = millis();
    ignoreUp = false;
    waitForUp = false;
    singleOK = true;
    holdEventPast = false;
    longHoldEventPast = false;
    if ((millis()-upTime) < DCgap && DConUp == false && DCwaiting == true)  DConUp = true;
    else  DConUp = false;
    DCwaiting = false;
  }
  // Button released
  else if (buttonVal == HIGH && buttonLast == LOW && (millis() - downTime) > debounce)
  {        
    if (not ignoreUp)
    {
       upTime = millis();
       if (DConUp == false) DCwaiting = true;
     else
     {
       event = 2;
       DConUp = false;
       DCwaiting = false;
       singleOK = false;
     }
    }
  }
  // Test for normal click event: DCgap expired
  if ( buttonVal == HIGH && (millis()-upTime) >= DCgap && DCwaiting == true && DConUp == false && singleOK == true && event != 2)
  {
    event = 1;
    DCwaiting = false;
  }
  // Test for hold
  if (buttonVal == LOW && (millis() - downTime) >= holdTime) {
    // Trigger "normal" hold
    if (not holdEventPast)
    {
       event = 3;
       waitForUp = true;
       ignoreUp = true;
       DConUp = false;
       DCwaiting = false;
       //downTime = millis();
       holdEventPast = true;
    }
    // Trigger "long" hold
    if ((millis() - downTime) >= longHoldTime)
    {
       if (not longHoldEventPast)
       {
         event = 4;
         longHoldEventPast = true;
       }
    }
  }
  buttonLast = buttonVal;
  return event;
}
