#include "Swarmy.h"


static void _SwarmyButton1ISR();
static void _SwarmyButton2ISR();
static void _SwarmyButton3ISR();
volatile int _SwarmyButton1;
volatile int _SwarmyButton2;
volatile int _SwarmyButton3;

Swarmy::Swarmy(bool displayMSG) {

  Motor_setup();
  OLED_setup();
  Infrared_setup();
  InfreredReceive_setup();
  RGB_setup();
  Button_setup();
}


void Swarmy::Motor_setup() {
  for (int i = 0; i < 4; i++) {
    ledcSetup(_motor[i], _motorFrequency, _motorResolution);
    ledcAttachPin(_motorPin[i], _motor[i]);
  }
}


void Swarmy::setMotorSpeed(int index, int speed) {
  int cspeed = constrain(abs(speed), 0, 255);

  if (index == 1) {

    if (speed >= 0) {
      ledcWrite(_motor[0], 255);
      ledcWrite(_motor[1], 255 - cspeed);

    } else {
      ledcWrite(_motor[1], 255);
      ledcWrite(_motor[0], 255 - cspeed);
    }
  } else  if (index == 0) {

    if (speed >= 0) {
      ledcWrite(_motor[2], 255);
      ledcWrite(_motor[3], 255 - cspeed);

    } else {
      ledcWrite(_motor[3], 255);
      ledcWrite(_motor[2], 255 - cspeed);
    }
  }
}


U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);
void Swarmy::OLED_setup() {

  u8g2.begin();
  u8g2.setFont(u8g2_font_t0_12_tf);
}

void Swarmy::OLEDprint(String row0, String row1, String row2) {
  u8g2.clearBuffer();

  u8g2.setCursor(0, 8);
  u8g2.print(row0);
  u8g2.setCursor(0, 20);
  u8g2.print(row1);
  u8g2.setCursor(0, 32);
  u8g2.print(row2);

  u8g2.sendBuffer();
}

void Swarmy::Infrared_setup() {
  ledcSetup(_InfraredPwmChannel, 1000, 8);
  ledcAttachPin(INFRAREDPIN, _InfraredPwmChannel);
  ledcWrite(_InfraredPwmChannel, 0);
}

void Swarmy::setInfraredFrequency(int frequency) {
  frequency = constrain(frequency, 0, 10000);
  if (frequency > 0) {
    ledcSetup(_InfraredPwmChannel, frequency, 8);
    ledcWrite(_InfraredPwmChannel, 128);
  }
  else {
    ledcWrite(_InfraredPwmChannel, 0);
  }
}

void Swarmy::InfreredReceive_setup() {
  Serial1.begin(115200, SERIAL_8N1, 4, 5);  //rx = 4, tx = 5
  Serial1.setTimeout(1000);
}

void Swarmy::ReceiveIR() {
  char *ptr = NULL;
  String string = "";
  char *strings[16]; // an array of pointers to the pieces of the above array after strtok()
  char receiveBuffer[50];
  Serial1.print("1");

  if (Serial1.available()) {

    string = Serial1.readStringUntil('\n');
    //Serial.print(string);
    string.toCharArray(receiveBuffer, 50);

    byte index = 0;
    ptr = strtok(receiveBuffer, ",");  // delimiter
    while (ptr != NULL)
    {
      strings[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");
    }

    for (int i = 0; i < 8; i++) {
      _Receivedfrequency[i] = atoi(strings[i]);
    }
    for (int i = 8; i < 16; i++) {
      _Receivedamplitude[i - 8] = atoi(strings[i]);
    }

  }
}

int Swarmy::getFrequency(int index) {
  return 100 * ((_Receivedfrequency[index] + 50) / 100);
}
int Swarmy::getAmplitude(int index) {
  return _Receivedamplitude[index];
}



CRGB leds[NUM_RGBLEDS];
void Swarmy::RGB_setup() {
  FastLED.addLeds<WS2812, RGBLED_PIN, GRB>(leds, NUM_RGBLEDS);
  FastLED.setBrightness(10);
}

void Swarmy::setRGBColor(int index, int r, int g, int b) {
  leds[index] = CRGB(r, g, b);
}

void Swarmy::UpdateRGB() {
  FastLED.show();
}

void Swarmy::setRGBBrightness(int brightness) {
  FastLED.setBrightness(brightness);
}

void Swarmy::Button_setup() {
  pinMode(SwarmyButton1Pin, INPUT);
  pinMode(SwarmyButton2Pin, INPUT);
  pinMode(SwarmyButton3Pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(SwarmyButton1Pin), _SwarmyButton1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SwarmyButton2Pin), _SwarmyButton2ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SwarmyButton3Pin), _SwarmyButton3ISR, RISING);
}



void _SwarmyButton1ISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > DEBOUNCEDELAY)
  {
    _SwarmyButton1++;
    last_interrupt_time = interrupt_time;
  }
}
void _SwarmyButton2ISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > DEBOUNCEDELAY)
  {
    _SwarmyButton2++;
    last_interrupt_time = interrupt_time;
  }
}
void _SwarmyButton3ISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > DEBOUNCEDELAY)
  {
    _SwarmyButton3++;
    last_interrupt_time = interrupt_time;
  }
}

int Swarmy::getButton(int index) {
  if (index == 0) {
    return _SwarmyButton1;
  }else
  if (index == 1) {
    return _SwarmyButton2;
  }else
  if (index == 2) {
    return _SwarmyButton3;
  }
  else{
    return 0;
  }
}
void Swarmy::decreaseButton(int index){
  if (index == 0) {
    _SwarmyButton1--;
  }else
  if (index == 1) {
    _SwarmyButton2--;
  }else
  if (index == 2) {
    _SwarmyButton3--;
  }
}
