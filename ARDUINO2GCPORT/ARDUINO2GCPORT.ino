#include <Nintendo.h>
#include <ArduinoVT.h>

#define HANDSHAKE_SEND "A2GCC_HOST"
#define HANDSHAKE_RECEIVE "A2GCC_AVR"

#define pinLed LED_BUILTIN

Term Term;

CGamecubeConsole GamecubeConsole1(5);
Gamecube_Data_t d = defaultGamecubeData;

void HALT(uint8_t n) {
  while(1) {
    digitalWrite(pinLed, HIGH);
    delay(1000);
    digitalWrite(pinLed, LOW);
    delay(1000);
    for(int i = 0; i < n; i++) {
      digitalWrite(pinLed, HIGH);
      delay(200);
      digitalWrite(pinLed, LOW);
      delay(200);
    }
    digitalWrite(pinLed, HIGH);
    delay(1000);
    digitalWrite(pinLed, LOW);
    delay(1000);
  }
}

void StartHandshake() {
  uint8_t c = 0;
  char receive[sizeof(HANDSHAKE_SEND)];
  while (1) {
    if (Serial.available() && c < sizeof(HANDSHAKE_SEND)) {
      receive[c++] = Serial.read();
    }
    if (c == sizeof(HANDSHAKE_SEND)) {
      if (!strcmp(receive, HANDSHAKE_SEND)) break;
      else HALT(3);
    }
  }
  Serial.print(HANDSHAKE_RECEIVE);
}

void setup() {
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, HIGH);
  StartHandshake();
}

char data1;
char data2;
char data3;
char data4;
char retstat = 0;
bool ReadInput = true;

void ResetController() {
  d.report.a = 0;
  d.report.b = 0;
  d.report.x = 0;
  d.report.y = 0;
  d.report.z = 0;
  d.report.start = 0;
  d.report.r = 0;
  d.report.l = 0;
  d.report.dleft = 0;
  d.report.dright = 0;
  d.report.dup = 0;
  d.report.ddown = 0;
  d.report.xAxis = 128;
  d.report.yAxis = 128;
}

void CheckParity() {
  uint8_t onbits = 0;
  for (int i = 0; i < 8; i++) {
    if ((data1 << i) & 1) onbits++;
  }
  for (int i = 0; i < 4; i++) {
    if ((data2 << i) & 1) onbits++;
  }

  if ((onbits % 2) == 0 && data2 & 8) {
    return;
  }
  if ((onbits % 2) != 0 && !(data2 & 8)) {
    return;
  }
  HALT(onbits);
}

void ReadData() {
  d.report.a = data1 & 1;
  d.report.b = data1 & 2;
  d.report.x = data1 & 4;
  d.report.y = data1 & 8;
  d.report.z = data1 & 16;
  d.report.start = data1 & 32;
  d.report.r = data1 & 64;
  d.report.l = data1 & 128;
  d.report.dleft = data2 & 1;
  d.report.dright = data2 & 2;
  d.report.dup = data2 & 4;
  d.report.ddown = data2 & 8;
}

void loop() {
  //Serial.write(64);
  if (Serial.available()) {
    data1 = Serial.read();
    data2 = Serial.read();
    data3 = Serial.read();
    data4 = Serial.read();
    //CheckParity();
    Serial.write(data1);
    Serial.write(data2);
    Serial.write(data3);
    Serial.write(data4);
    ResetController();
    ReadData();
  }  
  
  if (!GamecubeConsole1.write(d))
  {
    digitalWrite(pinLed, HIGH);
  } else {
    digitalWrite(pinLed, LOW);
  }
}
