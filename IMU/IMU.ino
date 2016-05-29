//ТК я работаю с несколькими платами, и у некоторых из них 
//апаратный UART и программный отличаются я пишу следующую строчку
//чтобы потом можно было легко изменить, при использовании другой платы
#define SerialPC Serial
// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с модулями IMU
#include "troyka-imu.h"
 
Accelerometer accel;

void setup()
{
  // открываем последовательный порт
  SerialPC.begin(115200);
  delay(3000);
  // пока не появились данные с USB
  // ждём
  //while (!SerialUSB.available()) {}
  // выводим сообщение о начале инициализации
  SerialPC.println("Begin init...");
  // инициализация акселерометра
  accel.begin();
  // выводим сообщение об удачной инициализации
  SerialPC.println("Init completed");
}
 
void loop()
{
/*
	!!!ВАЖНО!!!
	библиотека возвращает значение приблизительно от -10.00 до +10.00
	Причем значения типа float(1.0, 1.53 9.87 и так далее)
	Мне не было необходимости получать значения с сотыми,
	Поэтому в данном коде используется тип int, дробная часть отбрасывается
*/
  // Считываем значение по оси X и выводим его в Serial
  int x = accel.readX_G(); 
  SerialPC.print(x);
  SerialPC.print("\t");
  // Считываем значение по оси Y и выводим его в Serial
  int y = accel.readY_G();
  SerialPC.print(y);
  SerialPC.print("\t");
  //Считываем значение по оси Z и выводим его в Serial
  int z = accel.readZ_G();
  SerialPC.print(z);
  SerialPC.print("\t\t");
 
  SerialPC.println("");
  delay(300);
}
