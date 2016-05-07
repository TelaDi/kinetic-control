#define SerialPC SerialUSB
// библиотека для работы I²C
#include <Wire.h>
//Библиотеки для работы с NRF24
#include <SPI.h>
#include "RF24.h"

/**/
// библиотека для работы с модулями IMU
#include <troyka-imu.h>
// создаём объект для работы с акселерометром
Accelerometer accel;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9, 10);
const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"
int data[3] = {0, 0, 0};
void setup() {
  delay(5000);
  SerialPC.begin(115200);
  SerialPC.println("setup");
  radio.begin();
  delay(100);
  SerialPC.println("Set radio param");
  radio.setChannel(9); // канал (0-127)// скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS// RF24_250KBPS на nRF24L01 (без +) неработает. // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);  // мощьность передатчика, RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(pipe); // открываем трубу на передачу.
  SerialPC.println("end set radio param");
  SerialPC.println("end setup");
}

float x, y, z;
int speed = 0;
const int middleMin = 35;
const int middleMax = 5;

void loop() {
  SerialPC.println("loop");
  data[0] = 100;
  data[1] = 200;
  data[2] = 1000;
  SerialPC.println("clear buff");
  radio.flush_tx();
  SerialPC.println("sending...");
  if (!radio.write(data, sizeof(data))) {
    SerialPC.println("fail");
  }
  delay(200);
  SerialPC.println("end loop");

}
