#define SerialPC Serial
#define SerialHard Serial1

#define SERVO_MID 90
#define SERVO_LEFT SERVO_MID - 30
#define SERVO_RIGHT SERVO_MID + 30
#define EG_PIN 5
#define TIMEOUT 1000
//Библиотеки для работы с NRF24
#include <SPI.h>
#include "RF24.h"

#include <Servo.h>
Servo servo;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
//RF24 radio(7, 8);
RF24 radio(9, 10);
const uint64_t pipe = 0xF0F1F2F3F4LL;
byte data[] = {0, 0, 0};
unsigned long lastCommand;
void setup() {
  servo.attach(9);
  servo.write(SERVO_MID);


  pinMode(EG_PIN, OUTPUT);
  // put your setup code here, to run once:
  SerialPC.begin(115200);
  radio.begin();
  delay(20);
  radio.setChannel(9); // канал (0-127)  // скорость, RF24_250KBPS, RF24_1MBPS или RF24_2MBPS// RF24_250KBPS на nRF24L01 (без +) неработает.// меньше скорость, выше чувствительность приемника.
  radio.setDataRate(RF24_1MBPS);   // мощьность передатчика RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM,
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1, pipe); // открываем первую трубу с индитификатором "pipe"
  radio.startListening(); // включаем приемник, начинаем слушать трубу
  lastCommand = millis();
}

void loop() {
  if (radio.available()) { // проверяем не пришло ли чего в буфер.
    radio.read(data, sizeof(data)); // читаем данные, указываем сколько байт читать
    for(int i = 0; i < 3; i++) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println("");
    
  }
  /*
  if (millis() - lastCommand > TIMEOUT) {
    stoped();
  }
  if (radio.available()) { // проверяем не пришло ли чего в буфер.
    radio.read(data, sizeof(data)); // читаем данные, указываем сколько байт читать
    //Вперед
    if (data[0] == 1 ) {
      //analogWrite(EG_PIN,160);
      //delay(50);
      analogWrite(EG_PIN, 172);
      delay(100);
      analogWrite(EG_PIN, 174);
    }//Назад
    else if(data[0] == 255) {
      //analogWrite(EG_PIN,160);
      //delay(50);
      analogWrite(EG_PIN, 145);
      delay(100);
      analogWrite(EG_PIN, 140);
    }
    else {
      analogWrite(EG_PIN, 160);
    }
    
    if (data[2] == 0) {
      servo.write(SERVO_MID);
    }
    else if (data[2] == 1) {
      servo.write(SERVO_LEFT);
    }
    else {
      servo.write(SERVO_RIGHT);
    }
    
    lastCommand = millis();
  }*/
}

void stoped() {
  servo.write(SERVO_MID);
  analogWrite(EG_PIN, 160);
}
