#include "Hardware.h"

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
