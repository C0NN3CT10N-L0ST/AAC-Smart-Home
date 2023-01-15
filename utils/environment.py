from controller import SerialController


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


# Gets current environment brightness percentage
def getBrightness(controller: SerialController) -> int:
    controller.sendCommand("#P09")
    return int(controller.receiveCommand("#D09"))


# Gets current flame status
def getFlameStatus(controller: SerialController) -> bool:
    controller.sendCommand("#P10")
    flame_status = controller.receiveCommand("#D10")
    
    if flame_status == 0:
        return True

    return False