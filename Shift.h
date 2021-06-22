#ifndef __SHIFT
#define __SHIFT

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using std::string;

#include "Input.h"
#include "InputInterface.h"
#include "Plane.h"

namespace Shift {

uint8_t changes = 0;
char* currentPlane;

std::vector<std::string> split (std::string &s, char delim) {
  std::vector<string> result;
  std::stringstream ss (s);
  std::string item;

  while (getline (ss, item, delim)) {
    result.push_back (item);
  }

  return result;
}

bool send(const char * msg, const char * value) {
  if (msg != NULL && Serial) {
    std::string str = (std::string)msg;
    std::vector<std::string> v = split (str, ',');

    for (auto i : v) {
      const char * m = i.c_str();
      Serial.write(m); Serial.write(' '); Serial.write(value); Serial.write('\n');
    }

    changes = 1;
  }
  return true;
}

void begin() {
  Serial.begin(256000);
}

void clear() {
  delay(300);
  Input::clearInputs();
}

void loop() {
  changes = 0;
  InputInterface::syncAll();
  Input::pollInputs();
  Plane::loopPlane(currentPlane);
}

bool hasChanges() {
  return changes == 1;
}

int readInterface(int id, int port) {
  return InputInterface::readDigital(id, port);
}

void printBinary(int b, int size) {
  for (int i = size - 1; i >= 0; i-- )
  {
    Serial.print(bitRead(b, i)); //shift and select first bit
  }
  Serial.println();
}

void scan() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);

      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("done");

  delay(500);           
}

// void onFighterChangeForce(char* fighterValue, bool force) {
//
//   if (!strcmp(currentPlane, fighterValue)) {
//     clear();
//     currentPlane = fighterValue;
//     Plane::initPlane(fighterValue);
//   }
//
//   Serial.println(fighterValue);
// Serial.println(currentPlane);
//
// }
//
// void onFighterChange(char* fighterValue) {
//   onFighterChangeForce(fighterValue, FALSE);
// }
//
// DcsBios::StringBuffer<16> fighterChange(0x0000, onFighterChange);
}

#ifdef SHIFT_STM32_ENCODER
  #include "Encoder.h"
#endif
#include "Button.h"
#include "Toggle.h"
#include "Potentiometer.h"

#endif
