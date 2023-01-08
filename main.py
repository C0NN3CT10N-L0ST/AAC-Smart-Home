import serial
from time import sleep


# Main program
def main():
    welcome()
    arduino = initilizeSerialConnection()
    execSecuritySystem(arduino)

    while True:
        command = input("$ ")
        
        if not executeCommand(arduino, command):
            print("Invalid command! Try 'help' for a list of commands.")
            print()
    

    # Ends program
    endProgram()



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


# Serial Connectoin Setup
def initilizeSerialConnection():
    print("Starting serial connection...")
    serial_conn = serial.Serial('COM3', 9600, timeout=10)
    print("Serial connection established!")
    print()
    sleep(3)
    return serial_conn


def sendSerialData(serial_conn: serial.Serial, op_code: str):
    serial_data = op_code + "\n"
    serial_conn.write(serial_data.encode())


def receiveSerialData(serial_conn: serial.Serial, op_code: str):
    serial_data = serial_conn.readline()
    serial_data = serial_data.decode().split('$')
    op_code = serial_data[0]
    data = serial_data[1]

    if op_code == op_code:
        return data

    return None


def execSecuritySystem(serial_conn: serial.Serial):
    print("Door is locked!")
    print("Press [ENTER] to unlock.")
    input()

    if not deativateSecuritySystem(serial_conn):
        triggerSecurityMeasures()


# Deactivates Security System by entering the pin code
def deativateSecuritySystem(serial_conn: serial.Serial) -> bool:
    tries = 3

    while (tries > 0):
        print("Tries left:", tries)
        print()
        tries -= 1
        print("Enter pin: ", end='')

        pin_input = getPinCodeInput()
        while pin_input is None:
            pin_input = getPinCodeInput()

        pin_code_check = checkPinCode(serial_conn, pin_input)
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
def checkPinCode(serial_conn: serial.Serial, pinCode: int) -> bool:
    sendSerialData(serial_conn, f"#P02${pinCode}")
    pin_code = receiveSerialData(serial_conn, "#D02")
    pin_code = int(pin_code)

    if pin_code == 1:
        return True
    return False


# Triggers Security Measures (i.e. fires alarm, etc)
def triggerSecurityMeasures():
    # TODO
    return


# Checks if the given command is valid and executes it
# Returns False when command is Invalid, True otherwise
def executeCommand(serial_conn: serial.Serial, cmd: str) -> bool:
    if len(cmd) == 0:
        return False

    cmd_data = cmd.lower()
    cmd_data = cmd_data.split()
    cmd_type = cmd_data[0]

    # Ends program on 'exit' command
    if cmd_type == "exit":
        endProgram(serial_conn)
        exit()
    elif cmd_type == "help":
        help()
    elif cmd_type == "info":
        info()
    elif cmd_type == "get":
        try:
            cmd_param1 = cmd_data[1]
            if cmd_param1 == "temperature":
                temp_celsius = getTemp(serial_conn)
                temp_farenheit = temp_celsius * 1.8 + 32
                print("Current Temperature: ", temp_celsius, "ºC / ", temp_farenheit, "ºF", sep='')
            else:
                return False
        except IndexError:
            return
    elif cmd_type == "set":
        # TODO
        return
    else:
        return False
    
    return True


# Gets current temperature in Celsius
def getTemp(serial_conn: serial.Serial) -> float:
    sendSerialData(serial_conn, "#P03")
    return float(receiveSerialData(serial_conn, "#D03"))


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


def endProgram(serial_conn: serial.Serial):
    print("Closing serial connection...")
    # Closes serial connection
    serial_conn.close()
    print("Serial connection closed!")
    print("Ending program...")
    sleep(3)


if __name__ == '__main__':
    main()