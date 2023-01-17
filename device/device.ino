#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <IRremote.hpp>
#include <Wire.h>
#include <DHT.h>

/*
	### AUTHORS ###
  Alexandre Marques - a22202130
  João Serralha - a22202133
  António Antunes - a22202276
  Henrique Aleixo - a22103544
*/

// Buzzer sound
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int buzzerSequence[] = {
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_AS4, NOTE_AS4, NOTE_AS4, NOTE_AS4,
  NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D5,
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, 
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5,
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, 
  NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, 
  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, 
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4, 0, NOTE_G4, NOTE_D5,
  NOTE_C5, 0, NOTE_AS4, 0,
  NOTE_A4, 0, NOTE_A4, NOTE_A4,
  NOTE_C5, 0, NOTE_AS4, NOTE_A4, 
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5,
  NOTE_G4,0, NOTE_G4, NOTE_AS5,
  NOTE_A5, NOTE_AS5, NOTE_A5, NOTE_AS5
};

// IR Controller Setup
enum controller_keys {
  K_ON,
  K_OFF,
  K_BRIGHTNESS_UP,
  K_BRIGHTNESS_DOWN,
  K_WHITE,
  K_RED_1,
  K_RED_2,
  K_RED_3,
  K_RED_4,
  K_RED_5,
  K_GREEN_1,
  K_GREEN_2,
  K_GREEN_3,
  K_GREEN_4,
  K_GREEN_5,
  K_BLUE_1,
  K_BLUE_2,
  K_BLUE_3,
  K_BLUE_4,
  K_BLUE_5,
  K_FLASH,
  K_STROBE,
  K_FADE,
  K_SMOOTH
};

// ARDUINO PINS
#define LED1_PIN 13
#define LED2_PIN 12
#define DHT_PIN 4
#define DHT_TYPE DHT11
#define BUZZER_PIN 5
#define DOOR_BUTTON_PIN 7
#define LIGHT_SENSOR_PIN A1
#define FLAME_SENSOR_PIN 8
#define LIGHTS_R_PIN 9
#define LIGHTS_G_PIN 10
#define LIGHTS_B_PIN 11
#define IR_PIN 6
#define SW2_PIN 3
#define PIN_CODE 9999

// Variables
char op_code_data[20];
bool lightsState = false;
uint8_t currentDoorButtonState;
uint8_t currentAlarmStopButtonState = 0;      // 0 - OFF | 1 - ON
uint8_t currentEnvironmentBrightness = 0;
uint16_t currentFlameStatus;
uint8_t currentLightControlMode = 0;          // 0 - Program | 1 - Controller
uint8_t currentLightAnimation = 0;            // 0 - No animation | 1 - Flash | 2 - Strobe | 3 - Fade | 4 - Smooth
bool staticColor = 0;                         // Stores if the current light mode is static (i.e. not an animation) or not


// DHT Sensor Setup
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);                               // Begins serial communication
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);    // Starts the receiver and enables build-in LED feedback upon receiving a signal
  pinMode(LED1_PIN, OUTPUT);                        // LED1 (BLUE) setup
  pinMode(LED2_PIN, OUTPUT);                        // LED1 (BLUE) setup
  pinMode(BUZZER_PIN, OUTPUT);                      // BUZZER setup
  pinMode(DOOR_BUTTON_PIN, INPUT);                  // Door Button setup
  pinMode(LIGHT_SENSOR_PIN, INPUT);                 // Light (photoresistor) sensor setup
  pinMode(FLAME_SENSOR_PIN, INPUT);                 // Flame sensor setup
  pinMode(LIGHTS_R_PIN, OUTPUT);                    // Lights RED setup
  pinMode(LIGHTS_G_PIN, OUTPUT);                    // Lights GREEN setup
  pinMode(LIGHTS_B_PIN, OUTPUT);                    // Lights BLUE setup
  pinMode(SW2_PIN, INPUT);                          // SW2 setup
  dht.begin();                                      // Starts DHT Sensor
}

void loop() {
  receiveOpCode();
  getDoorButtonState();
  getCurrentEnvironmentBrightness();
  getCurrentFlameStatus();
  executeColorCommand();
}

/* FUNCTIONS */
// Gets current temperature in Celsius
float getTemp() {
  return dht.readTemperature();
}

// Gets current humidity percentage
float getHumidity() {
  return dht.readHumidity();
}

// Gets current lights state
bool getLightsState() {
  return lightsState;
}

// Sets curent lights state
bool setLightsState(char input) {
  if (input == '1') {
    lightsState = true;
    setLightsRGBColor(255,255,255);
    return true;
  }

  if (input == '0') {
    lightsState = false;
    setLightsRGBColor(0,0,0);
    return true;
  }
  return false;
}

// Gets the current state of door button
void getDoorButtonState() {
  currentDoorButtonState = digitalRead(DOOR_BUTTON_PIN);
}

// Gets the current percentage of environment light brightness
void getCurrentEnvironmentBrightness() {
  uint16_t sensor_value = analogRead(LIGHT_SENSOR_PIN);
  // Maps sensor value to percentage (0-100)
  currentEnvironmentBrightness = map(sensor_value, 0, 1023, 0, 100);
}

// Gets the current flame state
void getCurrentFlameStatus() {
  currentFlameStatus = digitalRead(FLAME_SENSOR_PIN);
}

// Triggers security alarm
void activateSecurityAlarm() {
  int note_duration = 750 / 4;

  for (int current_note = 0; current_note < 112; current_note++) {
    getAlarmSTOPInput();
    // Stops alarm upon pressing SW2 button
    if (!currentAlarmStopButtonState) break;

    tone(BUZZER_PIN, buzzerSequence[current_note], note_duration);

    // Activates red LED
    if ((current_note + 10) % 10 == 0) {
      digitalWrite(LED2_PIN, HIGH);
      setLightsRGBColor(255,0,0);
      lightsState = true;
    } else if (current_note > 1 && ((current_note + 10) % 5 == 0)) {
      digitalWrite(LED2_PIN, LOW);
      setLightsRGBColor(0,0,0);
      lightsState = false;
    }

    int idle = note_duration * 1.30;
    delay(idle);
    
    noTone(BUZZER_PIN);
  }
  digitalWrite(LED2_PIN, LOW);
  setLightsRGBColor(0,0,0);
  lightsState = false;
}

// Triggers fire alarm
void activateFireAlarm() {
  for (uint8_t i = 0; i < 255; i++) {
    getAlarmSTOPInput();
    // Stops alarm upon pressing SW2 button
    if (!currentAlarmStopButtonState) break;

    // Activates RED LED and RGB LED (in yellow)
    digitalWrite(LED2_PIN, HIGH);
    setLightsRGBColor(255,255,0);
    lightsState = true;
    delay(2000);

    // Deactivate LEDs
    digitalWrite(LED2_PIN, LOW);
    setLightsRGBColor(0,0,0);
    lightsState = false;
    delay(1200);
  }
} 

// Triggers Security Measures
void triggerSecurityMeasures() {
  // TODO
}

// Switches lights ON/OFF
void switchLights() {
  lightsState = !lightsState;
  if (lightsState) {
    setLightsRGBColor(255, 255, 255);
  } else {
    setLightsRGBColor(0, 0, 0);
  }
}

// Sets Lights Color
void setLightsRGBColor(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(LIGHTS_R_PIN, red);
  analogWrite(LIGHTS_G_PIN, green);
  analogWrite(LIGHTS_B_PIN, blue);
}

// Gets lights input from IR Controller
int8_t getDecodedControllerInput() {
  int8_t cmd = -1;
  if (IrReceiver.decode()) {
    cmd = decodeControllerInput(IrReceiver.decodedIRData.command);
    IrReceiver.resume();
  }
  return cmd != -1 ? cmd : -1;
}

// Decodes Controller Input, returns -1 on invalid input
int8_t decodeControllerInput(uint16_t input) {
  switch (input) {
    case 0x7:
      return K_ON;
    case 0x6:
      return K_OFF;
    case 0x5:
      return K_BRIGHTNESS_UP;
    case 0x4:
      return K_BRIGHTNESS_DOWN;
    case 0xB:
      return K_WHITE;
    case 0x9:
      return K_RED_1; 
    case 0xD:
      return K_RED_2;
    case 0x15:
      return K_RED_3;
    case 0x19:
      return K_RED_4;
    case 0x11:
      return K_RED_5;
    case 0x8:
      return K_GREEN_1;
    case 0xC:
      return K_GREEN_2;
    case 0x14:
      return K_GREEN_3;
    case 0x18:
      return K_GREEN_4;
    case 0x10:
      return K_GREEN_5;
    case 0xA:
      return K_BLUE_1;
    case 0xE:
      return K_BLUE_2;
    case 0x16:
      return K_BLUE_3;
    case 0x1A:
      return K_BLUE_4;
    case 0x12:
      return K_BLUE_5;
    case 0xF:
      return K_FLASH;
    case 0x17:
      return K_STROBE;
    case 0x1B:
      return K_FADE;
    case 0x13:
      return K_SMOOTH;
    default:
      return -1;
  }
}

// Receives color input (controller) and executes its desired functionality
void executeColorCommand() {
  int8_t cmd = getDecodedControllerInput();

  if (cmd != -1 && currentLightControlMode == 1) {
    switch (cmd) {
      case K_ON:
        if (!lightsState) {
          setLightsRGBColor(255,255,255);
          lightsState = true;
        }
        break;
      case K_OFF:
        setLightsRGBColor(0,0,0);
        lightsState = false;
        break;
      case K_BRIGHTNESS_UP:
        // TODO
        break;
      case K_BRIGHTNESS_DOWN:
        // TODO
        break;
      case K_WHITE:
        if (lightsState) {
          setLightsRGBColor(255,255,255);
        }
        break;
      case K_RED_1:
        if (lightsState) {
          setLightsRGBColor(255,0,0);
        }
        break;
      case K_RED_2:
        if (lightsState) {
          setLightsRGBColor(255,10,0);
        }
        break;
      case K_RED_3:
        if (lightsState) {
          setLightsRGBColor(255,50,0);
        }
        break;
      case K_RED_4:
        if (lightsState) {
          setLightsRGBColor(255,120,0);
        }
        break;
      case K_RED_5:
        if (lightsState) {
          setLightsRGBColor(255,255,0);
        }
        break;
      case K_GREEN_1:
        if (lightsState) {
          setLightsRGBColor(0,255,0);
        }
        break;
      case K_GREEN_2:
        if (lightsState) {
          setLightsRGBColor(0,255,0);
        }
        break;
      case K_GREEN_3:
        if (lightsState) {
          setLightsRGBColor(0,255,100);
        }
        break;
      case K_GREEN_4:
        if (lightsState) {
          setLightsRGBColor(0,255,120);
        }
        break;
      case K_GREEN_5:
        if (lightsState) {
          setLightsRGBColor(0,255,255);
        }
        break;
      case K_BLUE_1:
        if (lightsState) {
          setLightsRGBColor(0,0,255);
        }
        break;
      case K_BLUE_2:
        if (lightsState) {
          setLightsRGBColor(10,0,255);
        }
        break;
      case K_BLUE_3:
        if (lightsState) {
          setLightsRGBColor(50,0,255);
        }
        break;
      case K_BLUE_4:
        if (lightsState) {
          setLightsRGBColor(120,0,255);
        }
        break;
      case K_BLUE_5:
        if (lightsState) {
          setLightsRGBColor(255,0,255);
        }
        break;
      case K_FLASH:
        // TODO
        break;
      case K_STROBE:
        // TODO
        break;
      case K_FADE:
        // TODO
        break;
      case K_SMOOTH:
        // TODO
        break;
      default:
        break;
    }
  }
}

// Updates current Alarm STOP button state
void getAlarmSTOPInput() {
  currentAlarmStopButtonState = digitalRead(SW2_PIN);
}

// TODO
void receiveOpCode() {
  if (Serial.available()) {
    Serial.readBytesUntil('\n', op_code_data, 20);
    char op_code[4];
    char data[10];
    getOpCode(op_code_data, op_code);
    getData(op_code_data, data);

    if (strcmp(op_code, "P02") == 0) {
      // Returns whether the given pin code is correct
      int pinInput = atoi(data);
      Serial.print("#D02$");
      if (pinInput == PIN_CODE) {
        Serial.println("1");
      } else {
        Serial.println("0");
      }
    } else if (strcmp(op_code, "P03") == 0) {
      // Returns the current temperature
      Serial.print("#D03$");
      Serial.println(getTemp());
    } else if (strcmp(op_code, "P04") == 0) {
      // Returns the current temperature percentage
      Serial.print("#D04$");
      Serial.println(getHumidity());
    } else if (strcmp(op_code, "P05") == 0) {
      // Returns current lights state
      Serial.print("#D05$");
      Serial.println(getLightsState());
    } else if (strcmp(op_code, "P06") == 0) {
      // Returns whether 'P06' was successful
      Serial.print("#D06$");
      if (setLightsState(data[0])) {
        Serial.println("1");
      } else {
        Serial.println("0");
      }
    } else if (strcmp(op_code, "P07") == 0) {
      // Returns whether 'P07' was successful
      Serial.println("#D07$1");
      activateSecurityAlarm();
    } else if (strcmp(op_code, "P08") == 0) {
      // Returns current door button state
      Serial.print("#D08$");
      Serial.println(currentDoorButtonState);
    } else if (strcmp(op_code, "P09") == 0) {
      // Returns current light brightness percentage
      Serial.print("#D09$");
      Serial.println(currentEnvironmentBrightness);
    } else if (strcmp(op_code, "P10") == 0) {
      // Returns current flate state
      Serial.print("#D10$");
      Serial.println(currentFlameStatus);
    } else if (strcmp(op_code, "P11") == 0) {
      // Returns the current light control mode
      Serial.print("#D11$");
      Serial.println(currentLightControlMode);
    } else if (strcmp(op_code, "P12") == 0) {
      // Returns whether current light mode was successfully changed
      if (data[0] == '1') {
        currentLightControlMode = 1;
        Serial.println("#D12$1");
      } else if (data[0] == '0') {
        currentLightControlMode = 0;
        Serial.println("#D12$1");
      } else {
        Serial.println("#D12$0");
      }
    } else if (strcmp(op_code, "P13") == 0) {
      if (data[0] == 'R') {
        lightsState = true;
        setLightsRGBColor(255,0,0);
        Serial.println("#D13$1");
      } else if (data[0] == 'G') {
        lightsState = true;
        setLightsRGBColor(0,255,0);
        Serial.println("#D13$1");
      } else if (data[0] == 'B') {
        lightsState = true;
        setLightsRGBColor(0,0,255);
        Serial.println("#D13$1");
      } else if (data[0] == 'W') {
        lightsState = true;
        setLightsRGBColor(255,255,255);
        Serial.println("#D13$1");
      } else {
        Serial.println("#D13$0");
      }
    } else if (strcmp(op_code, "P14") == 0) {
        Serial.println("#D14$1");
        activateFireAlarm();      
    }
  }
}

void getOpCode(char *src, char *dest) {
  for (int i = 1; i <= 3; i++) {
    dest[i - 1] = src[i];
  }
  dest[3] = '\0';
}

void getData(char *src, char *dest) {
  int j = 0;
  for (int i = 5; i < strlen(src); i++) {
    dest[j] = src[i];
    j++;
  }
  dest[j] = '\0';
}
