#include <Nintendo.h>

CGamecubeConsole GamecubeConsole(5);
Gamecube_Data_t d = defaultGamecubeData

char chr;

bool ok = false;
void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    chr = Serial.read();
  }

  switch (chr) {
    case 'A':
      d.report.a = HIGH;
      Serial.println(chr);
    break;

    case 'B':
      d.report.b = HIGH;
      Serial.println(chr);
    break;

    case 'X':
      d.report.x = HIGH;
      Serial.println(chr);
    break;

    case 'Y':
      d.report.y = HIGH;
      Serial.println(chr);
    break;

    case 'Z':
      d.report.z = HIGH;
      Serial.println(chr);
    break;

    case 'S':
      d.report.start = HIGH;
      Serial.println(chr);
    break;

    case 'R':
      d.report.r = HIGH;
      Serial.println(chr);
    break;

    case 'L':
      d.report.l = HIGH;
      Serial.println(chr);
    break;

    case 'l':
      d.report.dleft = HIGH;
      Serial.println(chr);
    break;

    case 'r':
      d.report.dright = HIGH;
      Serial.println(chr);
    break;

    case 'u':
      d.report.dup = HIGH;
      Serial.println(chr);
    break;

    case 'd':
      d.report.ddown = HIGH;
      Serial.println(chr);
    break;
  }
  GamecubeConsole.write(d);
  ResetController();
}

void ResetController() {
  d.report.a = LOW;
  d.report.b = LOW;
  d.report.x = LOW;
  d.report.y = LOW;
  d.report.z = LOW;
  d.report.start = LOW;
  d.report.r = LOW;
  d.report.l = LOW;
  d.report.dleft = LOW;
  d.report.dright = LOW;
  d.report.dup = LOW;
  d.report.ddown = LOW;
  d.report.xAxis = 128;
  d.report.yAxis = 128;
}
