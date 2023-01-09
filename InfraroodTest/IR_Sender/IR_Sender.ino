#include <config.h>
#include <Swarmy.h>

Swarmy bot(false);

void setup() {
  // put your setup code here, to run once:
  bot.OLEDprint("", "Waking up", "");
  //Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  bot.setInfraredFrequency(9000);
  bot.OLEDprint("", "Frequency: "+String(bot.getFrequency(1)), "");
}
