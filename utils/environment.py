from controller import SerialController


# Gets current temperature in Celsius
def get_temp(controller: SerialController) -> float:
    controller.send_command('#P03')
    return float(controller.receive_command('#D03'))


# Sets ideal temperature
def set_temp(controller: SerialController, temp: int) -> bool:
    controller.send_command(f"#P08${temp}")
    return


# Gets current humidity percentage
def get_humidity(controller: SerialController) -> float:
    controller.send_command('#P04')
    return float(controller.receive_command('#D04'))


# Gets current environment brightness percentage
def get_brightness(controller: SerialController) -> int:
    controller.send_command("#P09")
    return int(controller.receive_command("#D09"))


# Gets current flame status
def get_flame_status(controller: SerialController) -> bool:
    controller.send_command("#P10")
    flame_status = int(controller.receive_command("#D10"))
    
    if flame_status == 1:
        return True

    return False


# Gets current brightness level
def get_brightness_level(controller: SerialController) -> str:
    controller.send_command('#P17')
    return controller.receive_command('#D17')


# Sets current brightness level
def set_brightness_level(controller: SerialController, value: int) -> bool:
    payload = f'#P18${value}'
    controller.send_command(payload)
    data = int(controller.receive_command('#D18'))
    return True if data == 1 else False
