#include <Swarmy.h>



/*  the library includes:

    void setMotorSpeed(int index , int speed );   sets the motor speed to a value, right motor is index = 0, left is index = 1. Speed is between 0-255, negative number means backwards
    void OLEDprint(String row0, String row1, String row2); Prints 3 strings on the oled on the Swarmy. The different strings go in different rows
    void setInfraredFrequency(int frequency); Turns on the infrared lights at the specified frequency
    void ReceiveIR(); Saves the infrared values received at the moment when it is called. These values can be accessed using getFrequency and getAmplitude. Doesn't work properly if called too often minimum delay should be around 200ms
    int getFrequency(int index); outputs the frequency which was acquired with ReceiveIR. index is 0-7, 0 is receiver S1.... 7 is receiver S8. (These are written on the PCB)
    int getAmplitude(int index); same as getFrequency but outputs the strength of the received signal
    void setRGBColor(int index, int r, int g, int b); Sets RGB led to a color specified with r g b. the index is same as with getFrequency.
    void setRGBBrightness(int brightness); sets the brightness of all leds
    void UpdateRGB(); turns the RGB to the color which was set
    int getButton(int index); returns the number of times the button was pressed. index is 0-2, 0 is top button, 1 is middle, 2 is bottom button
    void decreaseButton(int index); decreases the number of times the button was pressed by 1

    make sure the u8g2 and the FastLED library are installed as well


*/
Swarmy bot(false);

int frequency = 1000;
bool started = 0;

void setup() {
  Serial.begin(115200);

  bot.setRGBBrightness(25);
  for (int i = 0; i < 8; i++) {
    bot.setRGBColor(i, 142, 42, 242);
    bot.UpdateRGB();
    delay(50);
  }
  delay(200);
  for (int i = 0; i < 8; i++) {
    bot.setRGBColor(i, 0, 0, 0);
  }
  bot.UpdateRGB();
}

void loop() {
  bot.ReceiveIR();
  if (bot.getButton(0) > 0) {
    bot.decreaseButton(0);
    //this happens when button 1 was pressed
    frequency += 100;
    bot.OLEDprint("Button 1 was pressed", "42", "something");
    Serial.println(1);

  }
  if (bot.getButton(1) > 0) {
    bot.decreaseButton(1);
    //this happens when button 2 was pressed
    frequency -= 100;
    bot.OLEDprint("Button 2 was pressed", "hello", "there");
    Serial.println(2);

  }
  if (bot.getButton(2) > 0) {
    bot.decreaseButton(2);
    //this happens when button 3 was pressed
    started = !started;
    bot.OLEDprint("Button 3 was pressed", "stuff", "something else");
    Serial.println(3);

  }

  if (started) {
    bot.setInfraredFrequency(frequency);
  }
  else {
    bot.setInfraredFrequency(0);
  }

  if (started) {
    bot.OLEDprint("42 Transmitting: ", "Frequency: " + String(frequency) + "Hz", started ? "Started" : "Halted");
  }
  else {
    bot.OLEDprint("42 Receiving: " + String(bot.getFrequency(0)), String(bot.getAmplitude(0)) + " " + String(bot.getAmplitude(1)) + " " + String(bot.getAmplitude(2)) + " " + String(bot.getAmplitude(3)) , String(bot.getAmplitude(4)) + " " + String(bot.getAmplitude(5)) + " " + String(bot.getAmplitude(6)) + " " + String(bot.getAmplitude(7)));
  }


  delay(200);
}
