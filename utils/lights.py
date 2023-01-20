from controller import SerialController


# Gets current lights state
def get_lights_state(controller: SerialController) -> bool:
    controller.send_command('#P05')
    data = int(controller.receive_command('#D05'))
    return True if data == 1 else False


# Sets current lights state ON/OFF
def set_lights_state(controller: SerialController, value: bool) -> bool:
    payload = '#P06$1' if value else '#P06$0'
    controller.send_command(payload)
    data = int(controller.receive_command('#D06'))
    return True if data == 1 else False


# Gets current light control mode
def get_light_control_mode(controller: SerialController) -> str:
    controller.send_command('#P11')
    data = int(controller.receive_command("#D11"))
    return "App" if data == 0 else "Remote"


# Sets current light control mode
def set_light_control_mode(controller: SerialController, value: str) -> bool:
    payload = '#P12$1' if value.lower() == 'remote' else '#P12$0'
    controller.send_command(payload)
    data = int(controller.receive_command('#D12'))
    return True if data == 1 else False


# Sets current lights color
def set_lights_color(controller: SerialController, color: str) -> bool:
    if color == 'red':
        payload = '#P13$R'
    elif color == 'green':
        payload = '#P13$G'
    elif color == 'blue':
        payload = '#P13$B'
    else:
        payload = '#P13$W'
    
    controller.send_command(payload)
    data = int(controller.receive_command('#D13'))
    return True if data == 1 else False


# Gets current brightness mode
def get_brightness_mode(controller: SerialController) -> str:
    controller.send_command('#P15')
    data = int(controller.receive_command('#D15'))
    return "Manual" if data == 0 else "Auto"


# Sets current brightness mode
def set_brightness_mode(controller: SerialController, value: str) -> bool:
    payload = '#P16$0' if value == 'off' else '#P16$1'
    controller.send_command(payload)
    data = int(controller.receive_command('#D16'))
    return True if data == 1 else False
