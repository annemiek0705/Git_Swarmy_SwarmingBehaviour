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
  bot.OLEDprint("Motortest", "Draai naar links", "Index 0, Speed 100");
  bot.setMotorSpeed(0, 100);
  bot.setMotorSpeed(1, 0);
  delay(50);
  bot.setMotorSpeed(0, 100);
  bot.setMotorSpeed(1, 100);
  delay(1000);

  bot.OLEDprint("Motortest", "Draai naar rechts", "Index 1, Speed 100");
  bot.setMotorSpeed(0, 0);
  bot.setMotorSpeed(1, 100);
  delay(50);
  bot.setMotorSpeed(0, 100);
  bot.setMotorSpeed(1, 100);
  delay(1000);

  bot.OLEDprint("Motortest", "Beide motoren vooruit", "Speed 100");
  bot.setMotorSpeed(0, 100);
  bot.setMotorSpeed(1, 100);
  delay(1000);
  
  bot.OLEDprint("Motortest", "Beide motoren achteruit", "Speed 100");
  bot.setMotorSpeed(0, -100);
  bot.setMotorSpeed(1, -100);
  delay(1000);
}
