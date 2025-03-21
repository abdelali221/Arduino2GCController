#include <Nintendo.h>

#define pinLed LED_BUILTIN

CGamecubeConsole GamecubeConsole1(5);
CGamecubeController GamecubeController1(7);
Gamecube_Data_t d = defaultGamecubeData;

int buttonAstate;
int buttonBstate;
int buttonUpState;
int buttonDownState;
int buttonStartState;

int lastButtonAstate;
int lastButtonBstate;
int lastButtonUpState;
int lastButtonDownState;
int lastButtonStartState;

unsigned long timer;

void setup() {
  Serial.begin(9600);
  Serial.print("Pressing the D-PAD buttons down...");
}

void loop() {

  d.report.a = 0;
  d.report.b = 0;
  d.report.x = 0;
  d.report.y = 0;
  d.report.z = 0;
  d.report.start = 0;
  d.report.r = 0;
  d.report.l = 0;
  d.report.dleft = 1;
  d.report.dright = 1;
  d.report.dup = 1;
  d.report.ddown = 1;
  d.report.xAxis = 128;
  d.report.yAxis = 128;
  
  GamecubeConsole1.write(d);
   if (!GamecubeConsole1.write(GamecubeController1))
    {
      Serial.println(F("Error writing Gamecube controller."));
      digitalWrite(pinLed, HIGH);
      delay(1000);
    }
  
}
