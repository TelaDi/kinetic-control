#define SerialPC Serial
//Минимальное и максимальное значения ШИМ для движения вперед/назад
#define MIN_PWM 100
#define MAX_PWM 250
// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с модулями IMU
#include <troyka-imu.h>
//Библиотеки для работы с NRF24
#include <SPI.h>
#include "RF24.h"
//Переменные, определяющие направление движения
bool left = false;
bool right = false;
bool top = false;
bool down = false;

// создаём объект для работы с акселерометром
Accelerometer accel;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 */
RF24 radio(9, 10);
const uint64_t pipe = 0xF0F1F2F3F4LL; // индитификатор передачи, "труба"
//Массив данных
byte data[] = {0, 0, 0};
void setup() {
  //Задержка перед включением пульта
  //Нужна была для отладки
  delay(5000);
  //инициализация Serial соединения
  SerialPC.begin(115200);
  SerialPC.println("setup");
  
  // инициализация акселерометра
  SerialPC.println("Init accel");
  accel.begin();
  SerialPC.println("Accel init");
  delay(100);
  
  //Инициализация NRF24L01
  SerialPC.println("Init radio");
  radio.begin();
  SerialPC.println("radio init");
  delay(100);
  //Установка параметров NRF24L01
  SerialPC.println("Set radio param");
  // канал (0-127)
  radio.setChannel(9); 
  // скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS
  // RF24_250KBPS на nRF24L01 (без +) неработает. // меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);  
  // мощность передатчика, RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);
  // открываем трубу на передачу.
  radio.openWritingPipe(pipe); 
  SerialPC.println("end set radio param");
  delay(100);
  
  SerialPC.println("end setup");
}
//Сюда складываем значения от акселерометра
int x, y, z;
//Скорость 
int speed = 0;

void loop() {
  SerialPC.println("loop");
  
  //Читаем значения от акселерометра
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
  
  //Устанавливаем значения направления
  //и скорости "по умолчанию"
  //ТЕ нет поворота, нет движения, скорость = 0
  //Нет поворота
  left = false;
  right = false;
  //Нет движения 
  down = false;
  top = false;
  speed = 0;
  //Определяем есть ли поворот руки
  //Поворот направо
  if ( y > 3) {
    right = true;
  }//Поворот налево
  else if ( y < -3) {
    left = true;
  }
  
  //Определяем наклоняется ли рука
  //Рука вниз
  if( x <= -3) {
      down = true;
      speed = map(constrain(x, -6, -3), -3, -6, MIN_PWM, MAX_PWM);
  }//рука вверх
  else if(x >= 3) {
      top = true;
      speed = map(constrain(x, 3, 6), 3, 6, MIN_PWM, MAX_PWM);  
  }
  //Заносим значения в массив, который будем отправлять
  //Направление вперед/назад
  //конструкция a = (b) ? 1 : 2; равносильна
  // if (b) { a = 1; } else {a = 2;}
  data[0] = (top) ? 1 : (down) ? 2 : 0;
  //Скорость
  data[1] = speed;
  //Движение по/против часовой стрелки
  data[2] = (left) ? 1 : (right) ? 2 : 0;
  //Выводим значения перед отправкой в Serial
  //Для отладки
  for(int i = 0; i < 3; i++) {
    SerialPC.print(data[i]);
    SerialPC.print(" ");
  }
  SerialPC.println("");
  SerialPC.println("clear buff");
  //Отчищаем буфер перед отправкой (необходимо на некоторых модуллях)
  radio.flush_tx();
  SerialPC.println("sending...");
  //Пытаемся отправить
  if (!radio.write(data, sizeof(data))) {
	  //Если не удалось отправить выводим в Serial сообщение об ошибке
    SerialPC.println("fail");
  }
  
  delay(100);
  SerialPC.println("end loop");
}
