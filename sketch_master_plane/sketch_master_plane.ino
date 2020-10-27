
#include <SPI.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

const int POTENTX = A3;
const int POTENTY = A2;
const int POTENTZ = A1;

struct DataPackage{
  byte potValue1;
  byte potValue2;
  byte potValue3;
  byte potValue4;
};

DataPackage data;

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}
void loop() {
  int potValue = analogRead(A0);
  data.potValue1 = map(potValue, 0, 1023, 0, 255);
  int potValueX = analogRead(POTENTX);
  data.potValue2 = map(potValueX, 1023, 0, 45, 135);
  int potValueY = analogRead(POTENTY);
  data.potValue3 = map(potValueY, 0, 1023, 45, 135);
  int potValueZ = analogRead(POTENTZ);
  data.potValue4 = map(potValueZ, 0, 1023, 45, 135);
  radio.write(&data, sizeof(DataPackage));
  delay(20);
}
