from interface.cli import CliInterface
import argparse

if __name__ == '__main__':
    description = "This application pretends to simulate a Smart Home system"
    parser = argparse.ArgumentParser(prog='Smart Home', description=description)
    parser.add_argument('-d', '--device_port', help='Device port used to be used for serial connection')
    args = parser.parse_args()

    if args.device_port is not None:
        cli = CliInterface(args.device_port)
    else:
        cli = CliInterface()
    
    cli.run()
