import serial
import time

arduino = serial.Serial('/dev/ttyACM0', 9600);

time.sleep(2);

print("connected")

def powerOn():
    arduino.write(b"ON\n")

def powerOff():
    arduino.write(b"OFF\n")

def toggleScoop():
    arduino.write(b"SCOOP\n")

def scoopActive():
    arduino.reset_input_buffer()
    arduino.write(b"STATUS\n")
    response = arduino.readline().decode().strip()

    if response == "ACTIVE":
        return True
    elif response == "INACTIVE":
        return False
    
    return None

def gps():
    arduino.reset_input_buffer()
    arduino.write(b"GPS\n")
    response = arduino.readline().decode().strip()
    lat, longi = response.split(" ")
    return float(lat), float(longi)