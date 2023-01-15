from controller import SerialController


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


# Gets current light control mode
def getLightControlMode(controller: SerialController) -> str:
    controller.sendCommand('#P11')
    data = int(controller.receiveCommand("#D11"))
    return "Program" if data == 0 else "Remote"


# Sets current light control mode
def setLightControlMode(controller: SerialController, value: str) -> bool:
    payload = '#P12$1' if value.lower() == 'remote' else '#P12$0'
    controller.sendCommand(payload)
    data = int(controller.receiveCommand('#D12'))
    return True if data == 1 else False


# Sets current lights color
def setLightsColor(controller: SerialController, color: str) -> bool:
    payload = '#P13'
    if color == 'red':
        payload = '#P13$R'
    elif color == 'green':
        payload =  '#P13$G'
    elif color == 'blue':
        payload =  '#P13$B'
    else:
        payload = '#P13$W'
    
    controller.sendCommand(payload)
    data = int(controller.receiveCommand('#D13'))
    return True if data == 1 else False