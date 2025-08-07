#include "Hardware.h"
#include <MD5Builder.h>
#include "config.h"

#define ERROR -1

const int LED_PIN = 2;
const int OUTPUT_1_PIN = 25;
const int OUTPUT_2_PIN = 26;
const int DINPUT_1_PIN = 12;
const int DINPUT_2_PIN = 13;
const int MODE_PIN = 14;

static bool output_1_state;
static bool output_2_state;

static int dig_input_1_state;
static int dig_input_2_state;


void HardwareManager::init() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(OUTPUT_1_PIN, OUTPUT);
  pinMode(OUTPUT_2_PIN, OUTPUT);

  pinMode(DINPUT_1_PIN, INPUT_PULLUP);
  pinMode(DINPUT_2_PIN, INPUT_PULLUP);

  pinMode(MODE_PIN, INPUT_PULLUP);

  output_1_state = 0;
  output_2_state = 0;
  
  dig_input_1_state = 0;
  dig_input_2_state = 0;
}


int HardwareManager::getMode(){
  return digitalRead(MODE_PIN);
}


bool HardwareManager::getOutput(int output) {
  switch(output){
    case 1:
      return output_1_state;
      break;
    
    case 2:
      return output_2_state;
      break;

    default:
      return ERROR;
  };
}


void HardwareManager::setOutput(bool value) {
  
  Serial.print("Setoutput:");
  Serial.println(value);

  if(value==true){
    output_1_state = true;
    digitalWrite(OUTPUT_1_PIN,  HIGH);
    Serial.println("Switch true");
  }  
  else{
    output_1_state = false;
    digitalWrite(OUTPUT_1_PIN,  LOW);
    Serial.println("Switch false");
  }
    
}


bool HardwareManager::getDInput(int dinput) {
  switch(dinput){
    case 1:
      dig_input_1_state = digitalRead(DINPUT_1_PIN);
      return dig_input_1_state;
      break;
    
    case 2:
      dig_input_2_state = digitalRead(DINPUT_2_PIN);
      return dig_input_2_state;
      break;

    default:
      return ERROR;
  };
}


void HardwareManager::runBlinkingLed() {
    static unsigned long previousMillis = 0;
    static bool ledState = false;
    const unsigned long interval = 500;

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}


String HardwareManager::generateID() {
  // Obtener chipId único (64 bits)
  uint64_t chipId = ESP.getEfuseMac();

  // Convertir chipId a string hexadecimal
  char chipIdStr[17];
  sprintf(chipIdStr, "%016llX", chipId);

  // Calcular hash MD5
  MD5Builder md5;
  md5.begin();
  md5.add(chipIdStr);
  md5.calculate();

  uint8_t hash[16];         // ✅ Reservar buffer local
  md5.getBytes(hash);       // ✅ Llenar el buffer con el hash

  // Tomar los primeros 4 bytes del hash como un número de 32 bits
  uint32_t hash32 = (hash[0] << 24) | (hash[1] << 16) | (hash[2] << 8) | hash[3];

  // Convertir ese número a base36
  return base36(hash32, 8);
}


String HardwareManager::base36(uint32_t value, uint8_t minLength) {
  const char* chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  String result = "";

  do {
    result = chars[value % 36] + result;
    value /= 36;
  } while (value > 0);

  // Padding con '0' si es más corto que minLength
  while (result.length() < minLength) {
    result = "0" + result;
  }

  return result;
}

String HardwareManager::getBoardName(){
  return BOARD_NAME;
}

String HardwareManager::getFirmwareVersion(){
  return String(MAJOR_VERSION) + MINOR_VERSION + SUBVERSION;
}
