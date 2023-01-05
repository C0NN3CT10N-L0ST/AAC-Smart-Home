#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
	### AUTHORS ###
    Alexandre Marques - a22202130
    João Serralha - a22202130
    António Antunes - a22202276
    Henrique Aleixo - a22103544
*/

#define IR_PIN 2
#define MENU_SIZE 3

// Variables
char controller_input[20];

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

// I2C LCD Display Setup
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x20, 16, 2);

void setup() {
	Serial.begin(9600);  // Begins serial communication
	IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);  // Starts the receiver and enables build-in LED feedback upon receiving a signal
  	lcd.init();  // Starts LCD Display
  	lcd.backlight();  // Enables LCD backlight
  
  	// Welcome message
    welcome();
}

void loop() {
  	menu();
  	// getControllerInput();
}

/* FUNCTIONS */
void welcome() {
    lcd.setCursor(3,0);
    lcd.print("Smart Home");
    lcd.setCursor(2,1);
    lcd.print("AAC -- 22/23");
    delay(5000);
    lcd.clear();
}

// Main menu
void menu() {
  	int currentMenu = 0;
  
  	// Menu Header
  	lcd.setCursor(6,0);
  	lcd.print("Menu");
  
    while (1) {
        switch (currentMenu) {
            case 0:
                // Menu 1 - Lights
                lcd.setCursor(0,1);
                lcd.print("<    Lights    >");
                getMenuInput(&currentMenu);
                break;
          	
          	case 1:
          		// Menu 2 - Temperature
          		lcd.setCursor(0,1);
          		lcd.print("<     Temp     >");
          		getMenuInput(&currentMenu);
          		break;
          	case 2:
          		// Menu 3 - TODO
          		lcd.setCursor(0,1);
          		lcd.print("<    Menu 3    >");
          		getMenuInput(&currentMenu);
          		break;
        }
    }
}

void getMenuInput(int *currentMenuIndex) {
	int input_cmd;
    while (1) {
      	if (IrReceiver.decode()) {
            // Stores received cmd
            input_cmd = decodeInput(IrReceiver.decodedIRData.command);
            switch (input_cmd) {
            	case K_LEFT:
              		*currentMenuIndex = (*currentMenuIndex - 1) < 0 ? MENU_SIZE - 1 : (*currentMenuIndex - 1) % MENU_SIZE;
              		IrReceiver.resume();
              		return;
              	case K_RIGHT:
              		*currentMenuIndex = (*currentMenuIndex + 1) % MENU_SIZE;
              		IrReceiver.resume();
              		return;
              	case K_CONTINUE:
              		Serial.println("YOLO");
              		IrReceiver.resume();
              		return;
            }
		}
    }
}

// Gets Input from IR Controller
void getControllerInput() {
  	int input_cmd;
    while (input_cmd != K_CONTINUE) {
        if (IrReceiver.decode()) {
			// Stores received cmd
          	input_cmd = decodeInput(IrReceiver.decodedIRData.command);
            IrReceiver.resume();
        }
    }
  	// TODO - finish this
}

// Decodes Controller Input, return -1 on invalid input
int decodeInput(uint16_t input) {
    switch (input) {
      	case 0x0:
      		return K_ONOFF;
      	case 0x1:
      		return K_VOLUP;
      	case 0x2:
      		return K_FUNCSTOP;
      	case 0x4:
      		return K_LEFT;
      	case 0x5:
      		return K_CONTINUE;
      	case 0x6:
      		return K_RIGHT;
      	case 0x8:
      		return K_ARROWDOWN;
      	case 0x9:
      		return K_VOLDOWN;
      	case 0xA:
      		return K_ARROWUP;
      	case 0xC:
      		return K_0;
      	case 0xD:
      		return K_EQ;
      	case 0xE:
      		return K_STREPT;
      	case 0x10:
      		return K_1;
      	case 0x11:
      		return K_2;
      	case 0x12:
      		return K_3;
      	case 0x14:
      		return K_4;
      	case 0x15:
      		return K_5;
      	case 0x16:
      		return K_6;
      	case 0x18:
      		return K_7;
      	case 0x19:
      		return K_8;
      	case 0x1A:
      		return K_9;
      	default:
      		return -1;
    }
}