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
  SerialUSB.begin(115200);
  delay(3000);
  // пока не появились данные с USB
  // ждём
  //while (!SerialUSB.available()) {}
  // выводим сообщение о начале инициализации
  SerialUSB.println("Begin init...");
 
  // инициализация гироскопа
  //gyro.begin();
  // инициализация акселерометра
  accel.begin();
  // инициализация компаса
  //compass.begin();
  // инициализация барометра
  //barometer.begin();
  // выводим сообщение об удачной инициализации
  SerialUSB.println("Init completed");
  //SerialUSB.println("Gyroscope\t\t\tAccelerometer\t\t\tCompass\t\tBarometer");
}
 
void loop()
{
  // вывод угловой скорости относительно оси X
//  SerialUSB.print(gyro.readX_DegPerSec());
//  SerialUSB.print("\t");
  // вывод угловой скорости относительно оси Y
  //SerialUSB.print(gyro.readY_DegPerSec());
//  SerialUSB.print("\t");
  // вывод угловой скорости относительно оси Z
//  SerialUSB.print(gyro.readZ_DegPerSec());
//  SerialUSB.print("\t\t");
 
  // вывод направления и величины ускорения по оси X
  SerialUSB.print(accel.readX_G());
  SerialUSB.print("\t");
  // вывод направления и величины ускорения по оси Y
  SerialUSB.print(accel.readY_G());
  SerialUSB.print("\t");
  // вывод направления и величины ускорения по оси Z
  SerialUSB.print(accel.readZ_G());
  SerialUSB.print("\t\t");
 
 /*
  // калибровка компаса
  compass.readXYZ_Calib();
  // выводим азимут относительно оси X
  SerialUSB.print(compass.read_Yaw());
  SerialUSB.print(" Degrees\t");
 
  // вывод значения абсолютного давления
  SerialUSB.print(barometer.readPressureMillibars());
  SerialUSB.print("\t");
  // вывод значения температуры окружающей среды
  SerialUSB.print(barometer.readTemperatureC());
  SerialUSB.print("\t");
 */
  SerialUSB.println("");
  delay(300);
}
