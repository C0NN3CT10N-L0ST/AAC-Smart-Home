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