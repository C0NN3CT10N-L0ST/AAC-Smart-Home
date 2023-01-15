from controller import SerialController
from time import sleep
from utils.security import execSecuritySystem, activateAlarmAndDangerLED
from utils.environment import *
from utils.lights import *
from utils.help import help_commands
import os


class CliInterface:
    arduino = None

    def __init__(self, device_port = None):
        # 'Clears' terminal on startup
        self.clearTerminal()
        self.welcome()
        self.arduino = self.initializeController(device_port=device_port)


    # Main program
    def run(self):
        execSecuritySystem(self.arduino)

        while True:
            command = input("$ ")
            
            if not self.executeCommand(self.arduino, command):
                print("Invalid command! Try 'help' for a list of commands.")
                print()


    # Welcome message
    def welcome(self):
        # Prints welcome message
        print("""
            
         ____                       _     _   _                      
        / ___| _ __ ___   __ _ _ __| |_  | | | | ___  _ __ ___   ___ 
        \___ \| '_ ` _ \ / _` | '__| __| | |_| |/ _ \| '_ ` _ \ / _ \\
         ___) | | | | | | (_| | |  | |_  |  _  | (_) | | | | | |  __/
        |____/|_| |_| |_|\__,_|_|   \__| |_| |_|\___/|_| |_| |_|\___|
                                                             by GOATS

        """)
        print("Initializing Smart Home System...")


    # Controller Setup
    def initializeController(self, device_port = None):
        print("Starting serial connection...")
        controller = SerialController(device_port, 9600, 3)
        print("Serial connection established!")
        print()
        return controller


    # Checks if the given command is valid and executes it
    # Returns False when command is invalid, True otherwise
    def executeCommand(self, controller: SerialController, cmd: str) -> bool:
        if len(cmd) == 0:
            return False

        cmd_data = cmd.lower()
        cmd_data = cmd_data.split()
        cmd_type = cmd_data[0]

        # Ends program on 'exit' command
        if cmd_type == "exit" or cmd_type == "quit":
            self.endProgram(controller)
            exit()
        elif cmd_type == "help":
            self.help()
        elif cmd_type == "info":
            self.info()
        elif cmd_type == "get":
            cmd_param1 = None
            try:
                # Tries to get first command parameter
                cmd_param1 = cmd_data[1]
            except IndexError:
                return False
            
            if cmd_param1 == "temperature":
                temp_celsius = getTemp(controller)
                temp_farenheit = temp_celsius * 1.8 + 32
                print(f"Current Temperature: {temp_celsius:.1f}ºC / {temp_farenheit:.1f}ºF")
            elif cmd_param1 == "humidity":
                print(f"Current Humidity Percentage: {getHumidity(controller):.2f}%")
            elif cmd_param1 == "lights":
                print("Current lights state: {}".format("ON" if getLightsState(controller) else "OFF"))
            elif cmd_param1 == "brightness":
                print(f"Current brightness percentage: {getBrightness(controller)}%")
            elif cmd_param1 == "flames":
                print("Current flame status: {}".format("Flames detected!" if getFlameStatus(controller) else "No flames"))
            elif cmd_param1 == "lightcontrol":
                print(f"Current light control mode: {getLightControlMode(controller)}")
            else:
                return False
        elif cmd_type == "set":
            cmd_param1 = None
            cmd_param2 = None
            try:
                # Tries to get first and second command parameters
                cmd_param1 = cmd_data[1]
                cmd_param2 = cmd_data[2]
            except IndexError:
                return False

            if cmd_param1 == "lights":
                if getLightControlMode(controller) == "Remote":
                    print("Invalid action! Current light control mode is: Remote")
                    print()
                    return True
                    
                # Sets lights ON/OFF
                if cmd_param2 == "on" or cmd_param2 == "off":
                    success = setLightsState(controller, True if cmd_param2 == "on" else False)
                    if success:
                        print("Lights state successfully set!") 
                        print()
                        return True
                    else:
                        print("Error occurred while trying to set lights state! Try again.")

                # Sets lights color
                if cmd_param2 == "white" or cmd_param2 == "red" or cmd_param2 == "green" or cmd_param2 == "blue":
                    lights_on = getLightsState(controller)
                    if not lights_on:
                        print("Invalid action! Current lights state is: OFF")
                        print()
                        return True

                    success = setLightsColor(controller, cmd_param2)
                    if success:
                        print(f"Lights color successfully set to {cmd_param2}!")
                        print()
                        return True
                    else:
                        print("Error occurred while trying to set lights color! Try again.")
                        print()
                        return True
                
                else:
                    return False

            elif cmd_param1 == "lightcontrol":
                if cmd_param2 == "program" or cmd_param2 == "remote":
                    success = setLightControlMode(controller, cmd_param2)
                    if success:
                        print(f"Light control mode successfully set to {cmd_param2}!")
                    else:
                        print("Error occurred while trying to set light control mode! Try again.")
            else:
                return False
        elif cmd_type == "trigger":
            cmd_param1 = None
            try:
                # Tries to get first command parameter
                cmd_param1 = cmd_data[1]
            except IndexError:
                return False

            if cmd_param1 == "securityalarm":
                success = activateAlarmAndDangerLED(controller)
                if success:
                    print("Alarm successfully activated!")
                else:
                    print("Failed to activate alarm! Try again.")
            else:
                return False
        else:
            return False
        
        print()
        return True


    def info(self):
        # TODO
        return


    # Help
    def help(self):
        print("Command List")
        print("--------------------")
        for cmd in help_commands:
            print(cmd)


    # 'Clear' terminal
    def clearTerminal(self):
        os.system('cls' if os.name == 'nt' else 'clear')


    def endProgram(self, controller: SerialController):
        print("Closing serial connection...")
        # Closes serial connection
        controller.close()
        print("Serial connection closed!")
        print("Ending program...")
        sleep(3)
        self.clearTerminal()


if __name__ == '__main__':
    controller = CliInterface()
    controller.run()