#define SerialPC SerialUSB
// библиотека для работы I²C
#include <Wire.h>
//Библиотеки для работы с NRF24
#include <SPI.h>
#include "RF24.h"
//#define USE_LED 1
//#define DEDUG 1
#define DEGUG_SERIAL 1
/**/
// библиотека для работы с модулями IMU
#include <troyka-imu.h>
const int leftLed = 2;
const int rightLed = 5;
const int topLed = 3;
const int downLed = 4;
bool left = false;
bool right = false;
bool top = false;
bool down = false;

// создаём объект для работы с акселерометром
Accelerometer accel;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9, 10);
const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"
byte data[3] = {0, 0, 0};
void setup() {

//#if defined DEBUG_SERIAL
  delay(5000);
  SerialPC.begin(115200);
  SerialPC.println("setup");
//#endif
//#if defined USE_LED
/*
  pinMode(leftLed, OUTPUT);
  pinMode(rightLed, OUTPUT);
  pinMode(topLed, OUTPUT);
  pinMode(downLed, OUTPUT);
  digitalWrite(topLed, HIGH);*/
//#endif
//#if defined DEBUG_SERIAL
  SerialPC.println("Init accel");
//#endif
  // инициализация акселерометра
  accel.begin();
//#if defined DEBUG_SERIAL
  SerialPC.println("Accel init");
//#endif
//#if defined USE_LED
//  digitalWrite(rightLed, HIGH);
//#endif
  delay(100);
//#if defined DEBUG_SERIAL
  SerialPC.println("Init radio");
//#endif
  radio.begin();
//#if defined DEBUG_SERIAL
  SerialPC.println("radio init");
//#endif
//#if defined USE_LED
 // digitalWrite(downLed, HIGH);
//#endif
  delay(100);
//#if defined DEBUG_SERIAL
  SerialPC.println("Set radio param");
//#endif
  radio.setChannel(9); // канал (0-127)// скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS// RF24_250KBPS на nRF24L01 (без +) неработает. // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);  // мощьность передатчика, RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(pipe); // открываем трубу на передачу.
//#if defined DEBUG_SERIAL
  SerialPC.println("end set radio param");
//#endif
//#if defined USE_LED
  //digitalWrite(leftLed, HIGH);
//#endif
  delay(100);
  /**/
  //showInit();
  //showError(1);
//#if defined DEBUG_SERIAL
  SerialPC.println("end setup");
#//endif
}

float x, y, z;
int speed = 0;
const int middleMin = 35;
const int middleMax = 5;

void loop() {
//#if defined DEBUG_SERIAL
  SerialPC.println("loop");
//#endif
//#if defined DEBUG_SERIAL
  SerialPC.println("read accel x, y, z");
//#endif
  /*
  #if defined DEDUG
  {
    bool p[] = {true, false, true, false};
    showError(2, p, 200);
  }
  #endif
  /**/
  x = accel.readX_G();
  y = accel.readY_G();
  z = accel.readZ_G();
//#if defined DEBUG_SERIAL
  SerialPC.println("end read.");
//#endif
  /*
  #if defined DEDUG
  showError(1);
  #endif*/
  if (x == y && y == z && z == 0) {
    //bool p[] = {false, false, true, true};
  //  showError(2, p, 200);
//#if defined DEBUG_SERIAL
    SerialPC.println("x = y = z = 0");
//#endif
    return;
  };

  left = false;
  right = false;
  //Нет поворота
  if ( (35 < y && y < 41) || y < 5) {

  }//Поворот налево
  else if (20 < y  && y < 35) {
    left = true;
  }//Поворот направо
  else if (y < 15) {
    right = true;
  }
  down = false;
  top = false;
  if ( middleMin < x || x < middleMax ) {
    speed = 0;
  }//рука вниз
  else if ( 29 < x) {
    down = true;
    top = false;
    speed = map(constrain(x, 29, middleMin), middleMin, 29, 0, 255);
  }//рука вверх
  else if (x < 11) {
    down = false;
    top = true;
    speed = map(constrain(x, middleMax, 11), middleMax, 11, 0, 255);
  }

  data[0] = (top) ? 1 : (down) ? 255 : 0;
  data[1] = speed;
  data[2] = (left) ? 1 : (right) ? 255 : 0;
  for(int i = 0; i < 3; i++) {
    SerialPC.print(data[i]);
    SerialPC.print(" ");
  }
  SerialPC.println("");
//#if defined USE_LED
//  digitalWrite(leftLed, left);
//  digitalWrite(rightLed, right);
//  digitalWrite(topLed, top);
//  digitalWrite(downLed, down);
//#endif
//#if defined DEBUG_SERIAL
  SerialPC.println("clear buff");
//#endif
  //radio.openWritingPipe(pipe); // открываем трубу на передачу.
  radio.flush_tx();
//#if defined DEBUG_SERIAL
  SerialPC.println("sending...");
//#endif
  if (!radio.write(data, sizeof(data))) {
    bool p[] = {true, true, false, false};
    //showError(2, p, 200);
//#if defined DEBUG_SERIAL
    SerialPC.println("fail");
//#endif
  }/**/
  delay(200);
//#if defined DEBUG_SERIAL
  SerialPC.println("end loop");
//#endif
}
/*
void showInit() {
#if defined USE_LED
  digitalWrite(topLed, HIGH);
  delay(200);

  digitalWrite(downLed, HIGH);
  delay(200);

  digitalWrite(leftLed, HIGH);
  delay(200);

  digitalWrite(rightLed, HIGH);
  delay(200);
#endif
};
void showError(int num) {
  bool p[] = {true, true, true, true};
  showError(num, p);
}
void showError(int num, bool d[]) {
  showError(num, d, 200);
}
void showError(int num, bool d[], int ms) {
  for (int i = 0; i < num; i++) {
#if defined USE_LED
    digitalWrite(leftLed, d[2]);
    digitalWrite(rightLed, d[3]);
    digitalWrite(topLed, d[0]);
    digitalWrite(downLed, d[1]);
    delay(ms);
    digitalWrite(leftLed, LOW);
    digitalWrite(rightLed, LOW);
    digitalWrite(topLed, LOW);
    digitalWrite(downLed, LOW);
    delay(ms);
#endif
  }
}
*/
