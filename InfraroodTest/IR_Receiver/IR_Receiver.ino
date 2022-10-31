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
  String ampStr = "Amplitude| 0: "+ String(amplitude[0])+ "1:"+String(amplitude[1])+ "2:"+String(amplitude[2])+"3:"+ String(amplitude[3])+"4:"+String(amplitude[4])+"5:"+String(amplitude[5])+"6:"+String(amplitude[6])+"7:"+String(amplitude[7]) ;
  String freqStr = "Frequency| 0: " + String(frequency[0])+ "1:"+String(frequency[1])+ "2:"+String(frequency[2])+"3:"+String(frequency[3])+"4:"+String(frequency[4])+"5:"+String(frequency[5])+"6:"+String(frequency[6])+"7:"+String(frequency[7]) ;
  
  bot.OLEDprint(ampStr, freqStr, "");
  delay(500);
}
