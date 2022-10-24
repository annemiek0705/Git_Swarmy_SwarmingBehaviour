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
  bot.OLEDprint("Motortest", "Linker Motor vooruit", "Index 0, Speed 100");
  bot.setMotorSpeed(0, 100);
  bot.setMotorSpeed(1, 0);
  delay(1000);

  bot.OLEDprint("Motortest", "Rechter motor vooruit", "Index 1, Speed 100");
  bot.setMotorSpeed(0, 0);
  bot.setMotorSpeed(1, 100);
  delay(1000);

  bot.OLEDprint("Motortest", "Beide motoren", "Speed 100");
  bot.setMotorSpeed(0, 100);
  bot.setMotorSpeed(1, 100);
  delay(1000);
  
}
