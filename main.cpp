#include <stdint.h>
#include <stdbool.h>
#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

/*
	### AUTHORS ###
    Alexandre Marques - a22202130
    João Serralha - a22202133
    António Antunes - a22202276
    Henrique Aleixo - a22103544
*/

#define IR_PIN 2
#define NEOPIXEL_PIN 3
#define NEOPIXEL_LED_COUNT 7
#define MENU_SIZE 3

// Variables
char controller_input[20];
bool lightsON = false;


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

// NeoPixel Jewel Setup
Adafruit_NeoPixel lights = Adafruit_NeoPixel(NEOPIXEL_LED_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
	Serial.begin(9600);  // Begins serial communication
	IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);  // Starts the receiver and enables build-in LED feedback upon receiving a signal
	lcd.init();  // Starts LCD Display
	lcd.backlight();  // Enables LCD backlight
	lights.begin();  // Starts Lights
	lights.clear();

	setLightsRGBColor(254,254,254);
	// Welcome message
	welcome();
}

void loop() {
	mainMenu();
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
void mainMenu() {
	int8_t currentMenuIndex = 0;
	int8_t currentSubmenuSelected = -1;

	resetMainMenu(&currentMenuIndex);
  
	while (1) {
		switch (currentMenuIndex) {
			case 0:
				// Menu 1 - Lights
				lcd.setCursor(0,1);
				lcd.print("<    Lights    >");
				break;
			
			case 1:
				// Menu 2 - Temperature
				lcd.setCursor(0,1);
				lcd.print("<     Temp     >");
				break;
			case 2:
				// Menu 3 - TODO
				lcd.setCursor(0,1);
				lcd.print("<    Menu 3    >");
				break;
		}

		getMenuInput(&currentMenuIndex, &currentSubmenuSelected);
		executeSubmenu(&currentSubmenuSelected, &currentMenuIndex);
	}
}

void resetMainMenu(int8_t *currentMenuIndex) {
	lcd.clear();
	// Menu Header
	lcd.setCursor(6,0);
	lcd.print("Menu");
	// Resets current menu index
	lcd.setCursor(0,1);
	lcd.print("<    Lights    >");
	*currentMenuIndex = 0;
}

// Main menu navigation
void getMenuInput(int8_t *currentMenuIndex, int8_t *currentSubmenu) {
	int8_t input_cmd;
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
					/* Selects current submenu */
					*currentSubmenu = *currentMenuIndex;
					IrReceiver.resume();
					return;
			}
		}
	}
}

// Executes selected submenu
void executeSubmenu(int8_t *submenu, int8_t *currentMenuIndex) {
	// Checks which subemnu to execute
	switch (*submenu) {
		case 0:
			lightsMenu();
			resetMainMenu(currentMenuIndex);
			break;
		case 1:
			// TODO
			resetMainMenu(currentMenuIndex);
			break;
		case 2:
			// TODO
			resetMainMenu(currentMenuIndex);
			break;
	}
	// Resets current submenu
	*submenu = -1;
}

// Lights Menu
void lightsMenu() {
	uint8_t cmd_input;
	lcd.clear();
	lcd.setCursor(5,0);
	lcd.print("Lights");
	lcd.setCursor(2,1);
	lcd.print("Status:");
  
    while (1) {
      	lcd.setCursor(10,1);
        if (lightsON) {
        	lcd.print(" ON");
        } else {
        	lcd.print("OFF");
        }
      
      	// Checks for controller input
      	cmd_input = getControllerInput();
      	// Turns lights ON/OFF based on input
        if (cmd_input == K_CONTINUE) {
        	lightsON = !lightsON;
        }
      
        if (cmd_input == K_FUNCSTOP) {
			return;
        }
    }
}

// Gets Input from IR Controller
int8_t getControllerInput() {
	uint8_t cmd;
    if (IrReceiver.decode()) {
        cmd = decodeInput(IrReceiver.decodedIRData.command);
        IrReceiver.resume();
      	return cmd;
    }
	return -1;
}

// Decodes Controller Input, return -1 on invalid input
uint8_t decodeInput(uint16_t input) {
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

// Sets Lights Color
void setLightsRGBColor(uint8_t red, uint8_t green, uint8_t blue) {
    lights.clear();
	for(int i = 0; i < NEOPIXEL_LED_COUNT; i++) {
		lights.setPixelColor(i, lights.Color(red, green, blue));
		lights.show();
	}
}
