#include "Hardware.h"

#define OUTPUT_1 1
#define OUTPUT_2 2

#define ERROR -1

const int OUTPUT_1_PIN = 25;
const int OUTPUT_2_PIN = 26;
const int DINPUT_1_PIN = 12;
const int DINPUT_2_PIN = 13;
const int MODE_PIN = 14;

int output_1_state;
int output_2_state;

int dig_input_1_state;
int dig_input_2_state;

void HardwareManager::init() {
  pinMode(OUTPUT_1_PIN, OUTPUT);
  pinMode(OUTPUT_2_PIN, OUTPUT);

  pinMode(DINPUT_1_PIN, INPUT);
  pinMode(DINPUT_2_PIN, INPUT);

  pinMode(MODE_PIN, INPUT_PULLUP);

  output_1_state = 0;
  output_2_state = 0;
  
  dig_input_1_state = 0;
  dig_input_2_state = 0;
}

int HardwareManager::getMode(){
  return digitalRead(MODE_PIN);
}

int HardwareManager::getOutput(int output) {
  
  switch(output){

    case 1:
      return output_1_state;
      break;
    
    case 2:
      return output_1_state;
      break;

    default:
      return ERROR;
  };
}

int HardwareManager::getDInput(int dinput) {
  
  switch(dinput){

    case 1:
      output_1_state = digitalRead(DINPUT_1_PIN);
      return output_1_state;
      break;
    
    case 2:
      output_2_state = digitalRead(DINPUT_2_PIN);
      return output_2_state;
      break;

    default:
      return ERROR;
  };
}

