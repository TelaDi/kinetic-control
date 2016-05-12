//Serial для соединения с ПК
#define SerialPC Serial
//Serial на 0 и 1 пинах
#define SerialHard Serial1
#define TIMEOUT 1000
//Библиотеки для работы с NRF24
#include <SPI.h>
#include "RF24.h"
/* CE и CSN на 7 и 8 пинах */
RF24 radio(7, 8);
//Адрес трубы
const uint64_t pipe = 0xF0F1F2F3F4LL;
//Пины для управления моторами In1, In2, ENA(PWM)
const byte motor1[] = {2, 3, 9};
//Пины для управления моторами In3, In4, ENB (PWM)
const byte motor2[] = {4, 5, 10};
//Формат передаваемых данных
byte data[] = {0, 0, 0};
//Время прихода последней команды
unsigned long lastCommand;
void setup() {
  //Выставляем пины на выход
  for (int i = 0; i < 3; i++) {
    pinMode(motor1[i], OUTPUT);
    pinMode(motor2[i], OUTPUT);
    digitalWrite(motor1[i], LOW);
    digitalWrite(motor2[i], LOW);
  }

  //Инициализация модуля NRF24
  radio.begin();
  delay(20);
  //Настройки модуля
  radio.setChannel(9); // канал (0-127)
  // скорость, RF24_250KBPS/RF24_1MBPS/RF24_2MBPS.
  radio.setDataRate(RF24_1MBPS);
  //мощность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);
  //Открываем трубу на чтение
  radio.openReadingPipe(1, pipe);
  //Начинаем слушать эфир
  radio.startListening();
  //устанавливанм время прихода последней команды
  lastCommand = millis();
}

void loop() {
  //Если время прошедшее с момента последней команды больше чем TIMEOUT, останавливаем машинку
  if (millis() - lastCommand > TIMEOUT) {
    stoped();
  }

  //Если что-то пришло по радио
  if (radio.available()) {
    //Читаем данные
    radio.read(data, sizeof(data));
    //Если 1 - едем вперед, если 255 - едем назад, иначе останавливаемся
    (data[0] == 1) ? forward() : (data[0] == 2) ? back() : stoped();
    //Если разворачиваемся 1 - налево, 255 - направо, 0 ничего не меняем
    if (data[2] == 1) {
      left();
    }
    else if (data[2] == 2) {
      right();
    }

    //Фиксируем время прихода команды
    lastCommand = millis();
  }

}
//Останавливаемся и блокируем моторы
void stoped() {
  setMotorParam(false, false, false, false, 0, 0);
}

void forward() {
  setMotorParam(true, false, true, false, data[1], data[1]);
}

void back() {
  setMotorParam(false, true, false, true, data[1], data[1]);
}

void left() {
  setMotorParam(true, false, false, true, 255, 255);
}

void right() {
  setMotorParam(false, true, true, false, 255, 255);
}

void setMotorParam(bool d11, bool d12, bool d21, bool d22, int s1, int s2) {
  //Устанавливаем направление движения моторов
  digitalWrite(motor1[0], d11);
  digitalWrite(motor1[1], d12);

  digitalWrite(motor2[0], d21);
  digitalWrite(motor2[1], d22);

  //Устанавливаем скорость вращения моторов
  analogWrite(motor1[2], s1);
  analogWrite(motor2[2], s2);
}
