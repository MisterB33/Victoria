import serial
import time
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=.1)
def read():
    time.sleep(0.05)
    data = arduino.readline()
    return data
while True:
    value = read()
    print(value) # 
