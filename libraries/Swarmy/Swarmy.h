#ifndef Swarmy_h
#define Swarmy_h

#include "config.h"
#include "Arduino.h"

#include <U8g2lib.h>
#include <Wire.h>
#include <FastLED.h>



class Swarmy {
  public:

    Swarmy(bool displayMSG = false);

    void setMotorSpeed(int index = 0, int speed = 0);
    void OLEDprint(String row0 = "", String row1 = "", String row2 = "");
    void setInfraredFrequency(int frequency = 0);
    void ReceiveIR();
    int getFrequency(int index);
    int getAmplitude(int index);
    void setRGBColor(int index, int r, int g, int b);
    void UpdateRGB();
    void setRGBBrightness(int brightness = 10);
    int getButton(int index = 0);
    void decreaseButton(int index = 0);


  private:
    //for the motor control
    int _motorPin[4] = {MOTOR_PIN_R1, MOTOR_PIN_R2, MOTOR_PIN_L1, MOTOR_PIN_L2}; // R1 , R2 , L1 , L2
    int _motor[4] = {0, 1, 2, 3};
    const int _motorFrequency = 5000;
    const int _motorResolution = 8;
    void Motor_setup();
    //end motor

    //OLED
    void OLED_setup();
    //end OLED

    //for IR
    void Infrared_setup();
    void InfreredReceive_setup();
    int _InfraredPwmChannel = 4;
    int _Receivedfrequency[8];
    int _Receivedamplitude[8];
    //end IR

    //RGB
    void RGB_setup();
    //end RGB

    //Button
    void Button_setup();
    //end Button



};





#endif
