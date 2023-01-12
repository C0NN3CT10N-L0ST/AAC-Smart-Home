from controller import SerialController
from utils.art import artPinLock


# Executes security system which unlocks the door and the internal system
def execSecuritySystem(controller: SerialController):
    print("Door is locked!")
    print("Press [ENTER] to unlock.")

    artPinLock()
    if not unlockSecuritySystem(controller):
        activateSecurityMeasures()

    print("Press [BUTTON] (SW1) to open door!")
    print()

    # Gets button input to unlock the door
    getDoorButtonInput(controller)

    print("Door open! You're in.")


# Deactivates Security System by entering the pin code
def unlockSecuritySystem(controller: SerialController) -> bool:
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


# Gets button input which gets door open
def getDoorButtonInput(controller: SerialController):
    open = False
    while not open:
        controller.sendCommand("#P09")
        cmd_output = controller.receiveCommand("#D09")
        try:
            if int(cmd_output) == 0:
                return
        except ValueError:
            continue
        except TypeError:
            continue


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


# Fires alarm
def activateAlarmAndDangerLED(controller: SerialController):
    controller.sendCommand('#P07')
    data = int(controller.receiveCommand('#D07'))
    return data