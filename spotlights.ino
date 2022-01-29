#include <SoftwareWire.h>  // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>

#define DAY 0
#define NIGHT 1

int pwmPin = 10;
unsigned char i=0;
int pot;
int target;
int dusk = 18;
int dawn = 7;
int mode = DAY;

SoftwareWire myWire(16, 17);
RtcDS3231<SoftwareWire> Rtc(myWire);

void setup() {
  Serial.begin(115200);
  
  pinMode(pwmPin, OUTPUT);

  analogWrite(pwmPin, 0);

  pot = analogRead(1) / 4;

  Rtc.Begin();

  RtcDateTime now = Rtc.GetDateTime();

  Serial.println(now.Hour());
  Serial.println(millis());

  if(now.Hour() > dusk || now.Hour() < dawn) { // night time
    mode = NIGHT;
    target = pot;
  } else {
    mode = DAY;
    target = 255;  
  }

  Serial.print("target: ");
  Serial.println(target);

  while(i < target) {
    analogWrite(pwmPin, i++);
    Serial.print("i: ");
    Serial.print(i);
    Serial.println();
    delay(10);
  }
  analogWrite(pwmPin, target);
}

void loop() {
  // put your main code here, to run repeatedly:

  int newtarget = analogRead(1) / 4;

  Serial.print("target: ");
  Serial.print(target);
  
  Serial.print("\tnewtarget: ");
  Serial.println(newtarget);
  
  if(mode == NIGHT && abs(target - newtarget) > 10) {
    analogWrite(pwmPin, newtarget);
    target=newtarget;
  }

  delay(100);
}

