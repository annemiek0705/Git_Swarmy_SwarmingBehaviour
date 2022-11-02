#include <config.h>
#include <Swarmy.h>

Swarmy bot(false);
int amplitude[8];
int frequency[8];
int i;

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
  for(i = 0; i < 8; i++){
    amplitude[i] = bot.getAmplitude(i);
    frequency[i] = bot.getFrequency(i);
  }
  String firstStr = String(amplitude[0])+ ":"+String(frequency[0])+ " "+String(amplitude[1])+ ":"+String(frequency[1])+" "+ String(amplitude[2])+":"+String(frequency[2]);
  String secondStr = String(amplitude[3])+":"+String(frequency[3])+" "+String(amplitude[4])+":"+String(frequency[4])+" "+String(amplitude[5])+":"+String(frequency[5]);
  String thirdStr = String(amplitude[6])+":"+String(frequency[6])+ " "+String(amplitude[7])+":"+String(frequency[7]);
  bot.OLEDprint(firstStr, secondStr, thirdStr);
  delay(500);
}
