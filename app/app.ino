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
 
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
B00000001, B11000000,
B00000001, B11000000,
B00000011, B11100000,
B11110011, B11100000,
B11111110, B11111000,
B01111110, B11111111,
B00110011, B10011111,
B00011111, B11111100,
B00001101, B01110000,
B00011011, B10100000,
B00111111, B11100000,
B00111111, B11110000,
B01111100, B11110000,
B01110000, B01110000,
B00000000, B00110000 };

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

//void setup() {
//   Serial.begin(9600);
// 
//   display.begin();
//   // init done
// 
//   // you can change the contrast around to adapt the display
//   // for the best viewing!
//   display.setContrast(50);
// 
//   display.display(); // show splashscreen
//   delay(2000);
//   display.clearDisplay();   // clears the screen and buffer
// 
//                       // draw a single pixel
//   display.drawPixel(10, 10, BLACK);
//   display.display();
//   delay(2000);
//   display.clearDisplay();
// 
//   // draw many lines
//   testdrawline();
//   display.display();
//   delay(2000);
//   display.clearDisplay();
// 
//   // draw rectangles
//   testdrawrect();
//   display.display();
//   delay(2000);
//   display.clearDisplay();
// 
//   // draw multiple rectangles
//   testfillrect();
//   display.display();
//   delay(2000);
//   display.clearDisplay();
// 
//   // draw mulitple circles
//   testdrawcircle();
//   display.display();
//   delay(2000);
//   display.clearDisplay();
// 
//   // draw a circle, 10 pixel radius
//   display.fillCircle(display.width() / 2, display.height() / 2, 10, BLACK);
//   display.display();
//   delay(2000);
//   display.clearDisplay();
// 
//   testdrawroundrect();
//   delay(2000);
//   display.clearDisplay();
// 
//   testfillroundrect();
//   delay(2000);
//   display.clearDisplay();
// 
//   testdrawtriangle();
//   delay(2000);
//   display.clearDisplay();
// 
//   testfilltriangle();
//   delay(2000);
//   display.clearDisplay();
// 
//   // draw the first ~12 characters in the font
//   testdrawchar();
//   display.display();
//   delay(2000);
//   display.clearDisplay();
// 
//   // text display tests
//   display.setTextSize(1);
//   display.setTextColor(BLACK);
//   display.setCursor(0, 0);
//   display.println("Hello, world!");
//   display.setTextColor(WHITE, BLACK); // 'inverted' text
//   display.println(3.141592);
//   display.setTextSize(2);
//   display.setTextColor(BLACK);
//   display.print("0x"); display.println(0xDEADBEEF, HEX);
//   display.display();
//   delay(2000);
// 
//   // rotation example
//   display.clearDisplay();
//   display.setRotation(1);  // rotate 90 degrees counter clockwise, can also use values of 2 and 3 to go further.
//   display.setTextSize(1);
//   display.setTextColor(BLACK);
//   display.setCursor(0, 0);
//   display.println("Rotation");
//   display.setTextSize(2);
//   display.println("Example!");
//   display.display();
//   delay(2000);
// 
//   // revert back to no rotation
//   display.setRotation(0);
// 
//   // miniature bitmap display
//   display.clearDisplay();
//   display.drawBitmap(30, 16, logo16_glcd_bmp, 16, 16, 1);
//   display.display();
// 
//   // invert the display
//   display.invertDisplay(true);
//   delay(1000);
//   display.invertDisplay(false);
//   delay(1000);
// 
//   // draw a bitmap icon and 'animate' movement
//   testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_WIDTH, LOGO16_GLCD_HEIGHT);
//}
// 
//void loop() {
// 
//}
//
//void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
//   uint8_t icons[NUMFLAKES][3];
//   randomSeed(666);     // whatever seed
// 
//                   // initialize
//   for (uint8_t f = 0; f< NUMFLAKES; f++) {
//      icons[f][XPOS] = random(display.width());
//      icons[f][YPOS] = 0;
//      icons[f][DELTAY] = random(5) + 1;
// 
//      Serial.print("x: ");
//      Serial.print(icons[f][XPOS], DEC);
//      Serial.print(" y: ");
//      Serial.print(icons[f][YPOS], DEC);
//      Serial.print(" dy: ");
//      Serial.println(icons[f][DELTAY], DEC);
//   }
// 
//   while (1) {
//      // draw each icon
//      for (uint8_t f = 0; f< NUMFLAKES; f++) {
//         display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, BLACK);
//      }
//      display.display();
//      delay(200);
// 
//      // then erase it + move it
//      for (uint8_t f = 0; f< NUMFLAKES; f++) {
//         display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
//         // move it
//         icons[f][YPOS] += icons[f][DELTAY];
//         // if its gone, reinit
//         if (icons[f][YPOS] > display.height()) {
//            icons[f][XPOS] = random(display.width());
//            icons[f][YPOS] = 0;
//            icons[f][DELTAY] = random(5) + 1;
//         }
//      }
//   }
//}
//  
//void testdrawchar(void) {
//   display.setTextSize(1);
//   display.setTextColor(BLACK);
//   display.setCursor(0, 0);
// 
//   for (uint8_t i = 0; i < 168; i++) {
//      if (i == '\n') continue;
//      display.write(i);
//      //if ((i > 0) && (i % 14 == 0))
//      //display.println();
//   }
//   display.display();
//}
// 
//void testdrawcircle(void) {
//   for (int16_t i = 0; i<display.height(); i += 2) {
//      display.drawCircle(display.width() / 2, display.height() / 2, i, BLACK);
//      display.display();
//   }
//}
// 
//void testfillrect(void) {
//   uint8_t color = 1;
//   for (int16_t i = 0; i<display.height() / 2; i += 3) {
//      // alternate colors
//      display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
//      display.display();
//      color++;
//   }
//}
// 
//void testdrawtriangle(void) {
//   for (int16_t i = 0; i<min(display.width(), display.height()) / 2; i += 5) {
//      display.drawTriangle(display.width() / 2, display.height() / 2 - i,
//         display.width() / 2 - i, display.height() / 2 + i,
//         display.width() / 2 + i, display.height() / 2 + i, BLACK);
//      display.display();
//   }
//}
// 
//void testfilltriangle(void) {
//   uint8_t color = BLACK;
//   for (int16_t i = min(display.width(), display.height()) / 2; i>0; i -= 5) {
//      display.fillTriangle(display.width() / 2, display.height() / 2 - i,
//         display.width() / 2 - i, display.height() / 2 + i,
//         display.width() / 2 + i, display.height() / 2 + i, color);
//      if (color == WHITE) color = BLACK;
//      else color = WHITE;
//      display.display();
//   }
//}
// 
//void testdrawroundrect(void) {
//   for (int16_t i = 0; i<display.height() / 2 - 2; i += 2) {
//      display.drawRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, BLACK);
//      display.display();
//   }
//}
// 
//void testfillroundrect(void) {
//   uint8_t color = BLACK;
//   for (int16_t i = 0; i<display.height() / 2 - 2; i += 2) {
//      display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
//      if (color == WHITE) color = BLACK;
//      else color = WHITE;
//      display.display();
//   }
//}
// 
//void testdrawrect(void) {
//   for (int16_t i = 0; i<display.height() / 2; i += 2) {
//      display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, BLACK);
//      display.display();
//   }
//}
// 
//void testdrawline() {
//   for (int16_t i = 0; i<display.width(); i += 4) {
//      display.drawLine(0, 0, i, display.height() - 1, BLACK);
//      display.display();
//   }
//   for (int16_t i = 0; i<display.height(); i += 4) {
//      display.drawLine(0, 0, display.width() - 1, i, BLACK);
//      display.display();
//   }
//   delay(250);
// 
//   display.clearDisplay();
//   for (int16_t i = 0; i<display.width(); i += 4) {
//      display.drawLine(0, display.height() - 1, i, 0, BLACK);
//      display.display();
//   }
//   for (int8_t i = display.height() - 1; i >= 0; i -= 4) {
//      display.drawLine(0, display.height() - 1, display.width() - 1, i, BLACK);
//      display.display();
//   }
//   delay(250);
// 
//   display.clearDisplay();
//   for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
//      display.drawLine(display.width() - 1, display.height() - 1, i, 0, BLACK);
//      display.display();
//   }
//   for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
//      display.drawLine(display.width() - 1, display.height() - 1, 0, i, BLACK);
//      display.display();
//   }
//   delay(250);
// 
//   display.clearDisplay();
//   for (int16_t i = 0; i<display.height(); i += 4) {
//      display.drawLine(display.width() - 1, 0, 0, i, BLACK);
//      display.display();
//   }
//   for (int16_t i = 0; i<display.width(); i += 4) {
//      display.drawLine(display.width() - 1, 0, i, display.height() - 1, BLACK);
//      display.display();
//   }
//   delay(250);
//}
