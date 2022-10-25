#include <config.h>
#include <Swarmy.h>

Swarmy bot(false);
int amplitude;
int frequency;

void setup() {
  // put your setup code here, to run once:
  bot.OLEDprint("", "Waking up", "");
  Serial.begin(115200);
  bot.OLEDprint("", "starting scanning", "");
}

void loop() {
  // put your main code here, to run repeatedly:
  bot.OLEDprint("", "Receive IR", "");
  bot.ReceiveIR();
  bot.OLEDprint("", "get amplitude", "");
  amplitude = bot.getAmplitude(5);
  frequency = bot.getFrequency(5);
  
  bot.OLEDprint(String(amplitude), String(frequency), "");
  delay(500);
}
