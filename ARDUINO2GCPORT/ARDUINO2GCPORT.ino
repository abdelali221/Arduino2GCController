#include <Nintendo.h>
#include <ArduinoVT.h>

Term Term;
char chr;
bool ReadInput = true;
#define pinLed LED_BUILTIN
CGamecubeConsole GamecubeController1(6);
CGamecubeConsole GamecubeConsole1(5);
Gamecube_Data_t d = defaultGamecubeData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Term.Clear();
  pinMode(pinLed, OUTPUT);
  Serial.print("Arduino GC Controller Emulator");
  delay(1000);
  digitalWrite(pinLed, HIGH);
}

void loop() {
  if (Serial.available() && ReadInput) {
    chr = Serial.read();
  }
  
  switch (chr) {
    case ' ':
      d.report.start = 1;
      Term.SetCursPos(10, 10);
      Serial.print("Start is being pressed     ");
    break;
    
    case 'z':
      d.report.dup = 1;
      Term.SetCursPos(10, 10);
      Serial.print("D-PAD U is being pressed");
    break;

    case 's':
      d.report.ddown = 1;
      Term.SetCursPos(10, 10);
      Serial.print("D-PAD D is being pressed");
    break;

    case 'q':
      d.report.dleft = 1;
      Term.SetCursPos(10, 10);
      Serial.print("D-PAD L is being pressed");
    break;

    case 'd':
      d.report.dright = 1;
      Term.SetCursPos(10, 10);
      Serial.print("D-PAD R is being pressed");
    break;

    case 'm':
      d.report.a = 1;
      Term.SetCursPos(10, 10);
      Serial.print("A is being pressed       ");
    break;

    case ':':
      d.report.b = 1;
      Term.SetCursPos(10, 10);
      Serial.print("B is being pressed       ");
    break;

    case 'l':    
      d.report.x = 1;
      Term.SetCursPos(10, 10);
      Serial.print("X is being pressed       ");
    break;

    case ';':
      d.report.y = 1;
      Term.SetCursPos(10, 10);
      Serial.print("Y is being pressed       ");
    break;

    case 'p':
      d.report.z = 1;
      Term.SetCursPos(10, 10);
      Serial.print("Z is being pressed       ");
    break;

    case '$':
      d.report.r = 1;
      Term.SetCursPos(10, 10);
      Serial.print("R is being pressed       ");
    break;

    case 'a':
      d.report.l = 1;
      Term.SetCursPos(10, 10);
      Serial.print("L is being pressed       ");
    break;

    case NULL:
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
      Term.SetCursPos(10, 10);
      Serial.print("                         ");
    break;
  }
  if (!GamecubeConsole1.write(d))
  {
    Term.SetCursPos(20,20);
    Serial.println(F("Error writing Gamecube controller."));
    digitalWrite(pinLed, HIGH);
    delay(1000);
    ReadInput = false;
  } else {
    Term.SetCursPos(20,20);
    Serial.println(F("   Connection established.        "));
    ReadInput = true;
    chr = NULL;
  }
}
