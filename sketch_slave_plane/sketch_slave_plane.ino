#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

const int pwm_max = 2000;
const int pwm_min = 1000;

Servo motor;
Servo servo1;
Servo servo2;
Servo servo3;

int pulseWidth;
int targetPulseWidth;

int servo1PulseWidth;
int servo2PulseWidth;
int servo3PulseWidth;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

struct DataPackage{
  byte potValue1;
  byte potValue2;
  byte potValue3;
  byte potValue4;
};

DataPackage data;

void setup() {
  motor.attach(4, pwm_min, pwm_max);//pin number, min pulse width, max pulse width
  servo1.attach(9);
  servo2.attach(5);
  servo3.attach(6);
  servo1PulseWidth = 90;
  servo2PulseWidth = 90;
  servo3PulseWidth = 90;
  
  pulseWidth = pwm_min;
  targetPulseWidth = pwm_min;
  motor.writeMicroseconds(pulseWidth);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
    byte value = 0;
    radio.read(&data, sizeof(DataPackage));
    targetPulseWidth = map(data.potValue1, 0, 255, 1200, 2000);
    servo1PulseWidth = data.potValue2;
    servo2PulseWidth = data.potValue3;
    servo3PulseWidth = data.potValue4;
  }
  if (pulseWidth < targetPulseWidth) {
    pulseWidth += 1;
  } else if (targetPulseWidth < pulseWidth) {
    pulseWidth -= 1;
  }
  motor.writeMicroseconds(pulseWidth);
  servo1.write(servo1PulseWidth);
  servo2.write(servo2PulseWidth);
  servo3.write(servo3PulseWidth);
  delay(20);
}
