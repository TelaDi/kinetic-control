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
  delay(5000);
  SerialPC.begin(115200);
  SerialPC.println("setup");
  SerialPC.println("Init accel");
  // инициализация акселерометра
  accel.begin();
  SerialPC.println("Accel init");
  delay(100);
  SerialPC.println("Init radio");
  radio.begin();
  SerialPC.println("radio init");
  delay(100);
  SerialPC.println("Set radio param");
  radio.setChannel(9); // канал (0-127)// скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS// RF24_250KBPS на nRF24L01 (без +) неработает. // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);  // мощьность передатчика, RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(pipe); // открываем трубу на передачу.
  SerialPC.println("end set radio param");
  delay(100);
  SerialPC.println("end setup");
}

float x, y, z;
int speed = 0;
const int middleMin = 35;
const int middleMax = 5;

void loop() {
  SerialPC.println("loop");
  SerialPC.println("read accel x, y, z");
  x = accel.readX_G();
  y = accel.readY_G();
  z = accel.readZ_G();
  SerialPC.println("end read.");
  if (x == y && y == z && z == 0) {
    SerialPC.println("x = y = z = 0");
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
  SerialPC.println("clear buff");
  radio.flush_tx();
  SerialPC.println("sending...");
  if (!radio.write(data, sizeof(data))) {
    bool p[] = {true, true, false, false};
    SerialPC.println("fail");
  }
  delay(200);
  SerialPC.println("end loop");
}
