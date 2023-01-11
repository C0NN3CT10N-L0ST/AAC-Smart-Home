from controller import SerialController
from time import sleep


# Main program
def main():
    welcome()
    arduino = initializeController()
    execSecuritySystem(arduino)

    while True:
        command = input("$ ")
        
        if not executeCommand(arduino, command):
            print("Invalid command! Try 'help' for a list of commands.")
            print()


# Welcome message
def welcome():
    # Prints welcome message
    print("""
        
     ____                       _     _   _                      
    / ___| _ __ ___   __ _ _ __| |_  | | | | ___  _ __ ___   ___ 
    \___ \| '_ ` _ \ / _` | '__| __| | |_| |/ _ \| '_ ` _ \ / _ \\
     ___) | | | | | | (_| | |  | |_  |  _  | (_) | | | | | |  __/
    |____/|_| |_| |_|\__,_|_|   \__| |_| |_|\___/|_| |_| |_|\___|
                                                                

    """)
    print("Initializing Smart Home System...")


# Controller Setup
def initializeController(device_port = None):
    print("Starting serial connection...")
    controller = SerialController(device_port, 9600, 4)
    print("Serial connection established!")
    print()
    return controller


# Executes security system which unlocks the door and the internal system
def execSecuritySystem(controller: SerialController):
    print("Door is locked!")
    print("Press [ENTER] to unlock.")
    input()

    if not deativateSecuritySystem(controller):
        activateSecurityMeasures()


# Deactivates Security System by entering the pin code
def deativateSecuritySystem(controller: SerialController) -> bool:
    tries = 3

    while (tries > 0):
        print("Tries left:", tries)
        print()
        tries -= 1
        print("Enter pin: ", end='')

        pin_input = getPinCodeInput()
        while pin_input is None:
            pin_input = getPinCodeInput()

        pin_code_check = checkPinCode(controller, pin_input)
        if (pin_code_check):
            print("Pin Correct! Access acquired.")
            print()
            return True
        else:
            print("Pin Incorrect! Try again.")
            print()

    return False


# Gets and sanitizes pin input
def getPinCodeInput():
    pin_input = input()

    if len(pin_input) != 4:
        print("Pin must have 4 digits!")
        return None

    try:
        pin_input = int(pin_input)
    except ValueError:
        print("Pin must be an integer!")
        return None

    return pin_input


# Checks whether Pin Code is correct
def checkPinCode(controller: SerialController, pinCode: int) -> bool:
    controller.sendCommand(f"#P02${pinCode}")
    pin_code = controller.receiveCommand("#D02")
    pin_code = int(pin_code)

    if pin_code == 1:
        return True
    return False


# Activates Security Measures (i.e. fires alarm, etc)
def activateSecurityMeasures():
    activateAlarmAndDangerLED()


# Checks if the given command is valid and executes it
# Returns False when command is invalid, True otherwise
def executeCommand(controller: SerialController, cmd: str) -> bool:
    if len(cmd) == 0:
        return False

    cmd_data = cmd.lower()
    cmd_data = cmd_data.split()
    cmd_type = cmd_data[0]

    # Ends program on 'exit' command
    if cmd_type == "exit":
        endProgram(controller)
        exit()
    elif cmd_type == "help":
        help()
    elif cmd_type == "info":
        info()
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
            if cmd_param2 == "on" or cmd_param2 == "off":
                success = setLightsState(controller, True if cmd_param2 == "on" else False)
                if success:
                    print("Lights state successfully set!") 
                else:
                    print("Error occurred while trying to set lights state! Try again.")
            else:
                return False
        elif cmd_param1 == "temperature":
            try: 
                temp = int(cmd_param2)
                if not temp in range(15,31):
                    print("Temperature must be a value between 15 and 30!\n")
                    return True
            except ValueError:
                print("Temperature value must be an integer!\n")
                return True
        else:
            return False
    elif cmd_type == "activate":
        cmd_param1 = None
        try:
            # Tries to get first command parameter
            cmd_param1 = cmd_data[1]
        except IndexError:
            return False

        if cmd_param1 == "alarm":
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


# Gets current temperature in Celsius
def getTemp(controller: SerialController) -> float:
    controller.sendCommand('#P03')
    return float(controller.receiveCommand('#D03'))


# Sets ideal temperature
def setTemp(controller: SerialController, temp: int) -> bool:
    controller.sendCommand(f"#P08${temp}")
    return


# Gets current humidity percentage
def getHumidity(controller: SerialController) -> float:
    controller.sendCommand('#P04')
    return float(controller.receiveCommand('#D04'))


# Gets current lights state
def getLightsState(controller: SerialController) -> bool:
    controller.sendCommand('#P05')
    data = int(controller.receiveCommand('#D05'))
    return True if data == 1 else False

# Sets current lights state ON/OFF
def setLightsState(controller: SerialController, value: bool) -> bool:
    payload = '#P06$1' if value else '#P06$0'
    controller.sendCommand(payload)
    data = int(controller.receiveCommand('#D06'))
    return True if data == 1 else False


# Fires alarm
def activateAlarmAndDangerLED(controller: SerialController):
    controller.sendCommand('#P07')
    data = int(controller.receiveCommand('#D07'))
    return data


def info():
    # TODO
    return


def help():
    print(
        """
Command List
--------------------
help                    Shows this help menu
info                    Shows info about all sensors
get lights              Gets lights status
get temperature         Gets temperature info
get humidity            Gets humidity info
        """
    )
    # TODO: finish this


def endProgram(controller: SerialController):
    print("Closing serial connection...")
    # Closes serial connection
    controller.close()
    print("Serial connection closed!")
    print("Ending program...")
    sleep(3)


if __name__ == '__main__':
    main()