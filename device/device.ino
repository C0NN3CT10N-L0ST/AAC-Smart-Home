#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

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

// ARDUINO PINS
#define IR_PIN 2
#define NEOPIXEL_PIN 3
#define NEOPIXEL_LED_COUNT 7
#define LED1_PIN 13
#define LED2_PIN 12
#define DHT_PIN 4
#define DHT_TYPE DHT11
#define BUZZER_PIN 5
#define SW1_PIN 2
#define LIGHT_SENSOR_PIN A1
#define FLAME_SENSOR_PIN 7
#define PIN_CODE 9999

// Variables
char controller_input[20];
char op_code_data[20];
bool lightsON = false;
uint8_t currentDoorButtonState;
uint8_t currentEnvironmentBrightness = 0;
uint8_t currentFlameStatus;

// IR Controller Setup
enum controller_keys {
  K_0,
  K_1,
  K_2,
  K_3,
  K_4,
  K_5,
  K_6,
  K_7,
  K_8,
  K_9,
  K_ONOFF,
  K_VOLUP,
  K_VOLDOWN,
  K_FUNCSTOP,
  K_LEFT,
  K_RIGHT,
  K_CONTINUE,
  K_ARROWUP,
  K_ARROWDOWN,
  K_EQ,
  K_STREPT
};

// DHT Sensor Setup
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);                               // Begins serial communication
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);    // Starts the receiver and enables build-in LED feedback upon receiving a signal
  pinMode(LED1_PIN, OUTPUT);                        // LED1 (BLUE) setup
  pinMode(LED2_PIN, OUTPUT);                        // LED1 (BLUE) setup
  pinMode(BUZZER_PIN, OUTPUT);                      // BUZZER setup
  pinMode(SW1_PIN, INPUT);                          // SW1 setup
  pinMode(LIGHT_SENSOR_PIN, INPUT);                 // Light (photoresistor) sensor setup
  pinMode(FLAME_SENSOR_PIN, INPUT);                 // Flame sensor setup
  dht.begin();                                      // Starts DHT Sensor
}

void loop() {
  receiveOpCode();
  getDoorButtonState();
  getCurrentEnvironmentBrightness();
  getCurrentFlameState();
}

/* FUNCTIONS */
void switchLights() {
  lightsON = !lightsON;
  if (lightsON) {
    setLightsRGBColor(254, 254, 254);
  } else {
    setLightsRGBColor(0, 0, 0);
  }
}

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
  return lightsON;
}

// Sets curent lights state
bool setLightsState(char input) {
  if (input == '1') {
    lightsON = true;
    setLightsRGBColor(254,254,254);
    return true;
  }

  if (input == '0') {
    lightsON = false;
    setLightsRGBColor(0,0,0);
    return true;
  }
  return false;
}

// Gets the current state of door button
void getDoorButtonState() {
  currentDoorButtonState = digitalRead(SW1_PIN);
}

// Gets the current percentage of environment light brightness
void getCurrentEnvironmentBrightness() {
  uint16_t sensor_value = analogRead(LIGHT_SENSOR_PIN);
  // Maps sensor value to percentage (0-100)
  currentEnvironmentBrightness = map(sensor_value, 0, 1023, 0, 100);
}

// Gets the current flame state
void getCurrentFlameState() {
  uint8_t currentFlameStatus = digitalRead(FLAME_SENSOR_PIN);
}

// Fires alarm
void activateAlarm() {
  for (int current_note = 0; current_note < 112; current_note++) {

    int note_duration = 750 / 4;
    tone(BUZZER_PIN, buzzerSequence[current_note], note_duration);

    // Activates red LED
    if ((current_note + 10) % 10 == 0) {
      digitalWrite(LED2_PIN, HIGH);      
    } else if (current_note > 1 && ((current_note + 10) % 5 == 0)) {
      digitalWrite(LED2_PIN, LOW);
    }

    int idle = note_duration * 1.30;
    delay(idle);
    
    noTone(BUZZER_PIN);
  }
  digitalWrite(LED2_PIN, LOW);
}

// Triggers Security Measures
void triggerSecurityMeasures() {
  // TODO
}

// Sets Lights Color
void setLightsRGBColor(uint8_t red, uint8_t green, uint8_t blue) {
  // TODO
  /*analogWrite(COLORLED_R, red);
  analogWrite(COLORLED_G, green);
  analogWrite(COLORLED_B, blue);*/
}

void receiveOpCode() {
  if (Serial.available()) {
    Serial.readBytesUntil('\n', op_code_data, 20);
    char op_code[4];
    char data[10];
    getOpCode(op_code_data, op_code);
    getData(op_code_data, data);

    Serial.print(data);

    if (strcmp(op_code, "P02") == 0) {
      // Returns whether the given pin code is correct
      int pin_input = atoi(data);
      Serial.print("#D02$");
      if (pin_input == PIN_CODE) {
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
      activateAlarm();
    } else if (strcmp(op_code, "P08") == 0) {
      // TODO
    } else if (strcmp(op_code, "P09") == 0) {
      // Returns current door button state
      Serial.print("#D09$");
      Serial.println(currentDoorButtonState);
    } else if (strcmp(op_code, "P10") == 0) {
      // Returns current light brightness percentage
      Serial.print("#D10$");
      Serial.println(currentEnvironmentBrightness);
    } else if (strcmp(op_code, "P11") == 0) {
      // Returns current flate state
      Serial.print("#D11$");
      Serial.println(currentFlameStatus);
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
