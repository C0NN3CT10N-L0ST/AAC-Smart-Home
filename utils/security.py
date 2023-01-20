from controller import SerialController
from utils.ascii import ascii_pin_lock
from maskpass import askpass


# Executes security system which unlocks the door and the internal system
def exec_security_system(controller: SerialController):
    print("Door is locked!")
    print("Press [ENTER] to unlock.")
    input()

    ascii_pin_lock()
    if not unlock_security_system(controller):
        activate_security_measures()

    print("Press [BUTTON] to open door!")
    print()

    # Gets button input to unlock the door
    get_door_button_input(controller)

    print("Door open! You're in.")


# Deactivates Security System by entering the pin code
def unlock_security_system(controller: SerialController) -> bool:
    tries = 3

    while tries > 0:
        print("Tries left:", tries)
        print()
        tries -= 1

        pin_input = get_pin_code_input()
        while pin_input is None:
            pin_input = get_pin_code_input()

        pin_code_check = check_pin_code(controller, pin_input)
        if pin_code_check:
            print("Pin Correct! Access acquired.")
            print()
            return True
        else:
            print("Pin Incorrect! Try again.")
            print()

    return False


# Gets and sanitizes pin input
def get_pin_code_input():
    pin_input = askpass("Enter pin: ")

    if len(pin_input) != 4:
        print("Pin must have 4 digits!")
        print()
        return None

    try:
        pin_input = int(pin_input)
    except ValueError:
        print("Pin must be an integer!")
        print()
        return None

    return pin_input


# Gets button input which gets door open
def get_door_button_input(controller: SerialController):
    open = False
    while not open:
        controller.send_command("#P08")
        cmd_output = controller.receive_command("#D08")
        try:
            if int(cmd_output) == 0:
                return
        except ValueError:
            continue
        except TypeError:
            continue


# Checks whether Pin Code is correct
def check_pin_code(controller: SerialController, pin_code: int) -> bool:
    controller.send_command(f"#P02${pin_code}")
    data = controller.receive_command("#D02")
    data = int(data)
    return True if data == 1 else False


# Activates Security Measures (i.e. fires alarm, etc)
def activate_security_measures():
    activate_security_alarm_and_danger_led()


# Fires security alarm
def activate_security_alarm_and_danger_led(controller: SerialController):
    controller.send_command('#P07')
    data = int(controller.receive_command('#D07'))
    return True if data else False


# Triggers fire alarm
def activate_fire_alarm(controller: SerialController):
    controller.send_command('#P14')
    data = int(controller.receive_command('#D14'))
    return True if data else False


# Gets current fire alarm status
def get_fire_alarm_status(controller: SerialController) -> bool:
    controller.send_command('#P19')
    data = int(controller.receive_command('#D19'))
    return True if data else False


# Sets current fire alarm status
def set_fire_alarm_status(controller: SerialController, value: str) -> bool:
    payload = f"#P20${1 if value == 'auto' else 0}"
    controller.send_command(payload)
    data = int(controller.receive_command('#D20'))
    return True if data else False
