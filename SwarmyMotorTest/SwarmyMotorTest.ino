#include <config.h>
#include <Swarmy.h>


Swarmy bot(false);

void setup() {
  // put your setup code here, to run once:
   bot.OLEDprint("", "Waking up", "");
   Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  bot.OLEDprint("Index = 0", "Speed = 255", "");
  bot.setMotorSpeed(0, 255);
  bot.setMotorSpeed(1, 0);
  delay(10000);

  delay(10000);
  bot.OLEDprint("Index = 1", "Speed = 255", "");
  bot.setMotorSpeed(1, 255);
  bot.setMotorSpeed(0, 0);
  delay(10000);
  bot.OLEDprint("Index = both", "Speed = 255", "");
  bot.setMotorSpeed(1, 255);
  bot.setMotorSpeed(0, 255);
  delay(10000);
  
}
