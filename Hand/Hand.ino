#define SerialPC Serial
#define MIN_PWM 100
#define MAX_PWM 250
// библиотека для работы I²C
#include <Wire.h>
//Библиотеки для работы с NRF24
#include <SPI.h>
#include "RF24.h"
/**/
// библиотека для работы с модулями IMU
#include <troyka-imu.h>
bool left = false;
bool right = false;
bool top = false;
bool down = false;

// создаём объект для работы с акселерометром
Accelerometer accel;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9, 10);
const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"
byte data[] = {0, 0, 0};
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

int x, y, z;
int speed = 0;

void loop() {
  SerialPC.println("loop");
  SerialPC.print("read x: ");
  x = accel.readX_G();
  SerialPC.println(x);  
  SerialPC.print("read y: ");
  y = accel.readY_G();
  SerialPC.println(y);
  SerialPC.print("read z: ");
  z = accel.readZ_G();
  Serial.println(z);
  SerialPC.println("end read.");
  //Нет поворота
  left = false;
  right = false;
  //Поворот направо
  if ( y > 3) {
    right = true;
  }//Поворот налево
  else if ( y < -3) {
    left = true;
  }//Поворот направо
  //Нет движения 
  down = false;
  top = false;
  speed = 0;
  //Рука вниз
  if( x <= -3) {
      down = true;
      speed = map(constrain(x, -6, -3), -3, -6, MIN_PWM, MAX_PWM);
  }//рука вверх
  else if(x >= 3) {
      top = true;
      speed = map(constrain(x, 3, 6), 3, 6, MIN_PWM, MAX_PWM);  
  }
  
  data[0] = (top) ? 1 : (down) ? 2 : 0;
  data[1] = speed;
  data[2] = (left) ? 1 : (right) ? 2 : 0;
  for(int i = 0; i < 3; i++) {
    SerialPC.print(data[i]);
    SerialPC.print(" ");
  }
  SerialPC.println("");
  SerialPC.println("clear buff");
  radio.flush_tx();
  SerialPC.println("sending...");
  if (!radio.write(data, sizeof(data))) {
    SerialPC.println("fail");
  }
  delay(100);
  SerialPC.println("end loop");
}
