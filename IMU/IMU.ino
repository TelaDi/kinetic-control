#define SerialPC Serial
// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с модулями IMU
#include "troyka-imu.h"
 
// создаём объект для работы с гироскопом
Gyroscope gyro;
// создаём объект для работы с акселерометром
Accelerometer accel;
// создаём объект для работы с компасом
//Compass compass;
// создаём объект для работы с барометром
Barometer barometer;
 
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
 
  // инициализация гироскопа
  //gyro.begin();
  // инициализация акселерометра
  accel.begin();
  // инициализация компаса
  //compass.begin();
  // инициализация барометра
  //barometer.begin();
  // выводим сообщение об удачной инициализации
  SerialPC.println("Init completed");
  //SerialPC.println("Gyroscope\t\t\tAccelerometer\t\t\tCompass\t\tBarometer");
}
 
void loop()
{
  // вывод угловой скорости относительно оси X
//  SerialPC.print(gyro.readX_DegPerSec());
//  SerialPC.print("\t");
  // вывод угловой скорости относительно оси Y
  //SerialPC.print(gyro.readY_DegPerSec());
//  SerialPC.print("\t");
  // вывод угловой скорости относительно оси Z
//  SerialPC.print(gyro.readZ_DegPerSec());
//  SerialPC.print("\t\t");
  int x = accel.readX_G();
  // вывод направления и величины ускорения по оси X
  SerialPC.print(x);
  SerialPC.print("\t");
  // вывод направления и величины ускорения по оси Y
  int y = accel.readY_G();
  SerialPC.print(y);
  SerialPC.print("\t");
  // вывод направления и величины ускорения по оси Z
  int z = accel.readZ_G();
  SerialPC.print(z);
  SerialPC.print("\t\t");
 
 /*
  // калибровка компаса
  compass.readXYZ_Calib();
  // выводим азимут относительно оси X
  SerialPC.print(compass.read_Yaw());
  SerialPC.print(" Degrees\t");
 
  // вывод значения абсолютного давления
  SerialPC.print(barometer.readPressureMillibars());
  SerialPC.print("\t");
  // вывод значения температуры окружающей среды
  SerialPC.print(barometer.readTemperatureC());
  SerialPC.print("\t");
 */
  SerialPC.println("");
  delay(300);
}
