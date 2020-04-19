#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
#include <Adafruit_PCD8544.h>
 
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>

#include <DHT11.h>


const int pinDTH11 = 2;
DHT11 dht11(pinDTH11);
 
const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);
 
int ax, ay, az;
int gx, gy, gz;

const int PIN_RESET = 3;  // LCD1 Reset
const int PIN_SCE = 4;    // LCD2 Chip Select
const int PIN_DC = 5;     // LCD3 Dat/Command
const int PIN_SDIN = 6;   // LCD4 Data in
const int PIN_SCLK = 7;   // LCD5 Clk
                          // LCD6 Vcc
                          // LCD7 Vled
                          // LCD8 Gnd

Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_SCLK, PIN_SDIN, PIN_DC, PIN_SCE, PIN_RESET);
 
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
 
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

const int PIN_X_JOY = A2;
const int PIN_Y_JOY = A3;
const int PIN_B_JOY = 8;

int xPosJoy = 0;
int yPosJoy = 0;
int buttonJoy = 0;

//void setup()
//{
//   Serial.begin(9600);
//   Wire.begin();
//   mpu.initialize();
//   Serial.println(mpu.testConnection() ? F("IMU iniciado correctamente") : F("Error al iniciar IMU"));
//
//   pinMode(PIN_X_JOY, INPUT);
//   pinMode(PIN_Y_JOY, INPUT);
//   
//   //activar resistencia pull-up en el pin pulsador 
//   pinMode(PIN_B_JOY, INPUT_PULLUP); 
//}
// 
//void loop(){
  
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

//  xPosJoy = analogRead(PIN_X_JOY);
//  yPosJoy = analogRead(PIN_Y_JOY);
//  buttonJoy = digitalRead(PIN_B_JOY);
//    
//  Serial.print("X: ");
//  Serial.print(xPosJoy);
//  Serial.print(" | Y: ");
//  Serial.print(yPosJoy);
//  Serial.print(" | Button: ");
//  Serial.println(buttonJoy);
//  
//  delay(100); // añadir un poco de retraso entre lecturas
      
//}

void setup() {
  Serial.begin(9600);
  
  display.begin();
  // init done
  
  display.clearDisplay();   // clears the screen and buffer
  
  // signal status display
  display.setTextSize(1);
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
  display.print("off");
  display.setTextColor(BLACK);
  
  // temp and bat status display
  // temp
  display.setCursor(0, 9);
  display.print("Tmp");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.drawPixel(0,20, BLACK);
  display.drawPixel(0,21, BLACK);
  display.drawPixel(0,22, BLACK);
  display.drawPixel(0,23, BLACK);
  display.drawPixel(0,24, BLACK);
  display.drawPixel(0,25, BLACK);
  display.drawPixel(0,26, BLACK);
  display.drawPixel(0,27, BLACK);
  
  display.drawPixel(0,19, BLACK);
  display.drawPixel(1,19, BLACK);
  display.drawPixel(2,19, BLACK);
  display.drawPixel(3,19, BLACK);
  display.drawPixel(4,19, BLACK);
  display.drawPixel(5,19, BLACK);
  display.drawPixel(6,19, BLACK);
  display.drawPixel(7,19, BLACK);
  display.drawPixel(8,19, BLACK);
  display.drawPixel(9,19, BLACK);
  display.drawPixel(10,19, BLACK);
  display.drawPixel(11,19, BLACK);
  display.drawPixel(12,19, BLACK);
  
  display.setCursor(1, 20);
  display.print(69);
  display.setTextColor(BLACK);
  display.setCursor(16, 18);
  display.write(9); // º simbol
  display.setCursor(22, 20);
  display.print("C");

  // bat
  display.setTextColor(BLACK);
  display.setCursor(0, 30);
  display.print("Bat");
  
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.drawPixel(0,40, BLACK);
  display.drawPixel(0,41, BLACK);
  display.drawPixel(0,42, BLACK);
  display.drawPixel(0,43, BLACK);
  display.drawPixel(0,44, BLACK);
  display.drawPixel(0,45, BLACK);
  display.drawPixel(0,46, BLACK);
  display.drawPixel(0,47, BLACK);
  
  display.drawPixel(0,39, BLACK);
  display.drawPixel(1,39, BLACK);
  display.drawPixel(2,39, BLACK);
  display.drawPixel(3,39, BLACK);
  display.drawPixel(4,39, BLACK);
  display.drawPixel(5,39, BLACK);
  display.drawPixel(6,39, BLACK);
  display.drawPixel(7,39, BLACK);
  display.drawPixel(8,39, BLACK);
  display.drawPixel(9,39, BLACK);
  display.drawPixel(10,39, BLACK);
  display.drawPixel(11,39, BLACK);
  display.drawPixel(12,39, BLACK);
  
  display.setCursor(1, 40);
  display.print(69);
  display.setTextColor(BLACK);
  display.setCursor(18, 40);
  display.print("%");

  // mode status display
  display.setCursor(56, 9);
  display.print("sys");
  display.setCursor(56,19);
  display.print("mode");
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.drawPixel(56,29, BLACK);
  display.drawPixel(56,30, BLACK);
  display.drawPixel(56,31, BLACK);
  display.drawPixel(56,32, BLACK);
  display.drawPixel(56,33, BLACK);
  display.drawPixel(56,34, BLACK);
  display.drawPixel(56,35, BLACK);
  display.drawPixel(56,36, BLACK);
  display.setCursor(57, 29);
  display.print("off");
  display.setTextColor(BLACK);

  // alarm display
  display.setTextColor(WHITE, BLACK); // 'inverted' text
  display.setCursor(28, 40);
  display.write(32);
  display.write(42);
  display.print("thief");
  display.write(42);
  display.write(32);
  
  // bitmap display
  display.drawBitmap(26, 10, logo_moto_bmp, LOGO_MOTO_WIDTH, LOGO_MOTO_HEIGHT, BLACK);
  display.display();
   
}
 
void loop() {
 
}
  
