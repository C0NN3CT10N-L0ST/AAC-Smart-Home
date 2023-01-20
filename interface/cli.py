import os
from time import sleep
from utils.security import *
from utils.environment import *
from utils.lights import *
from utils.help import help_commands
from utils.ascii import ascii_welcome


class CliInterface:
    arduino = None

    def __init__(self, device_port=None):
        # 'Clears' terminal on startup
        self.clear_terminal()
        ascii_welcome()
        self.arduino = self.initialize_controller(device_port=device_port)

    # Main program
    def run(self):
        exec_security_system(self.arduino)

        while True:
            command = input("$ ")
            
            if not self.execute_command(self.arduino, command):
                print("Invalid command! Try 'help' for a list of commands.")
                print()

    # Controller Setup
    @staticmethod
    def initialize_controller(device_port=None):
        print("Starting serial connection...")
        controller_obj = SerialController(device_port, 9600, 3)
        print("Serial connection established!")
        print()
        return controller_obj

    # Checks if the given command is valid and executes it
    # Returns False when command is invalid, True otherwise
    def execute_command(self, controller: SerialController, cmd: str) -> bool:
        if len(cmd) == 0:
            return False

        cmd_data = cmd.lower()
        cmd_data = cmd_data.split()
        cmd_type = cmd_data[0]

        # Ends program on 'exit' command
        if cmd_type == "exit" or cmd_type == "quit":
            self.end_program(controller)
            exit()
        elif cmd_type == "help":
            self.help()
        elif cmd_type == "info":
            self.info()
        elif cmd_type == "get":
            try:
                # Tries to get first command parameter
                cmd_param1 = cmd_data[1]
            except IndexError:
                return False
            
            if cmd_param1 == "temperature":
                temp_celsius = get_temp(controller)
                temp_fahrenheit = temp_celsius * 1.8 + 32
                print(f"Current Temperature: {temp_celsius:.1f}ºC / {temp_fahrenheit:.1f}ºF")
            elif cmd_param1 == "humidity":
                print(f"Current Humidity Percentage: {get_humidity(controller):.2f}%")
            elif cmd_param1 == "lights":
                print("Current lights state: {}".format("ON" if get_lights_state(controller) else "OFF"))
            elif cmd_param1 == "brightness":
                print(f"Current brightness percentage: {get_brightness(controller)}%")
            elif cmd_param1 == "brightnesscontrol":
                print("Current brightness control: {}".format(
                    "Potentiometer" if get_brightness_control_mode(controller) else "App")
                )
            elif cmd_param1 == "flames":
                print("Current flame status: {}".format(
                    "Flames detected!" if get_flame_status(controller) else "No flames")
                )
            elif cmd_param1 == "firealarmstatus":
                print("Current fire alarm status is: {}".format("Auto" if get_fire_alarm_status(controller) else "OFF"))
            elif cmd_param1 == "lightcontrol":
                print(f"Current light control mode: {get_light_control_mode(controller)}")
            elif cmd_param1 == "brightnessmode":
                print(f"Current brightness mode: {get_brightness_mode(controller)}")
            elif cmd_param1 == "brightnesslevel":
                print("Current brightness level: {}".format(get_brightness_level(controller).strip("\n")))
            else:
                return False
        elif cmd_type == "set":
            try:
                # Tries to get first and second command parameters
                cmd_param1 = cmd_data[1]
                cmd_param2 = cmd_data[2]
            except IndexError:
                return False

            if cmd_param1 == "lights":
                if get_light_control_mode(controller) == "Remote":
                    print("Invalid action! Current light control mode is: Remote")

                # Sets lights ON/OFF
                elif cmd_param2 == "on" or cmd_param2 == "off":
                    success = set_lights_state(controller, True if cmd_param2 == "on" else False)
                    if success:
                        print("Lights state successfully set!") 
                    else:
                        print("Error occurred while trying to set lights state! Try again.")

                # Sets lights color
                elif cmd_param2 == "white" or cmd_param2 == "red" or cmd_param2 == "green" or cmd_param2 == "blue":
                    lights_on = get_lights_state(controller)
                    if not lights_on:
                        print("Invalid action! Current lights state is: OFF")

                    success = set_lights_color(controller, cmd_param2)
                    if success:
                        print(f"Lights color successfully set to {cmd_param2}!")
                    else:
                        print("Error occurred while trying to set lights color! Try again.")

                else:
                    return False

            elif cmd_param1 == "lightcontrol":
                if cmd_param2 == "app" or cmd_param2 == "remote":
                    success = set_light_control_mode(controller, cmd_param2)
                    if success:
                        print(f"Light control mode successfully set to {cmd_param2}!")
                    else:
                        print("Error occurred while trying to set light control mode! Try again.")

            elif cmd_param1 == "brightnessmode":
                if cmd_param2 == "off" or cmd_param2 == "auto":
                    success = set_brightness_mode(controller, cmd_param2)
                    if success:
                        print(f"Brightness mode successfully set to {cmd_param2}!")
                    else:
                        print("Error occurred while trying to set brightness mode! Try again.")

            elif cmd_param1 == "brightnesslevel":
                try:
                    brightness_level = int(cmd_param2)

                    if not 20 <= brightness_level <= 80:
                        print("Brightness value must be a number between 20 and 80!")

                    success = set_brightness_level(controller, brightness_level)
                    if success:
                        print(f"Brightness level successfully set to {brightness_level}!")
                    else:
                        print("Error occurred while trying to set brightness level! Try again.")
                except ValueError:
                    print("Brightness value must be an integer!")

            elif cmd_param1 == "firealarmstatus":
                if cmd_param2 == "auto" or cmd_param2 == "off":
                    success = set_fire_alarm_status(controller, cmd_param2)
                    if success:
                        print(f"Fire alarm status successfully set to {cmd_param2}!")
                    else:
                        print("Error occurred while trying to set fire alarm status! Try again.")

            elif cmd_param1 == "brightnesscontrol":
                if cmd_param2 == "app" or cmd_param2 == "potentiometer":
                    success = set_brightness_control_mode(controller, cmd_param2)
                    if success:
                        print(f"Brightness control successfully set to {cmd_param2}!")
                    else:
                        print("Error occurred while trying to set brightness control! Try again.")

            else:
                return False
        elif cmd_type == "trigger":
            try:
                # Tries to get first command parameter
                cmd_param1 = cmd_data[1]
            except IndexError:
                return False

            if cmd_param1 == "securityalarm":
                success = activate_security_alarm_and_danger_led(controller)
                if success:
                    print("Alarm successfully activated!")
                else:
                    print("Failed to activate alarm! Try again.")
            elif cmd_param1 == "firealarm":
                success = activate_fire_alarm(controller)
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
    @staticmethod
    def help():
        print("Command List")
        print("--------------------")
        for cmd in help_commands:
            print(cmd)

    @staticmethod
    def clear_terminal():
        os.system('cls' if os.name == 'nt' else 'clear')

    def end_program(self, controller: SerialController):
        print("Closing serial connection...")
        # Closes serial connection
        controller.close()
        print("Serial connection closed!")
        print("Ending program...")
        sleep(3)
        self.clear_terminal()


if __name__ == '__main__':
    controller = CliInterface()
    controller.run()
