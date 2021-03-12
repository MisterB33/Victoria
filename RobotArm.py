from nanpy import ArduinoApi,SerialManager, Servo
from time import sleep
import readchar

# readchar

basePin = 9
upPin = 10
forPin = 11
PinchPin = 6



try: 
    connection = SerialManager()
    a = ArduinoApi(connection = connection)
except ZeroDivisionError: 
    print("Failed to connect to Arduino")

# Set up the pinModes as if we are in the Arduino IDE 

# Sets Robot to Default Position

a = 90
b = 90
d = 90
pinch = 90

# Sets Zero for variables for saving extra Postion

e = 0
f = 0
g = 0

Duration = 0.0 
distance = 0.0 

stuff = 0.0

# Writes default position to Robot

x = 0

val = 0.0
servoBase = Servo(basePin)
servoUp = Servo(upPin)
servoFor = Servo(forPin)
servoPinch = Servo(PinchPin)

pos1 = (30, 30, 30, 30)


positions = []

servoBase.write(a)
servoUp.write(b)
servoFor.write(d)
servoPinch.write(pinch)

# Running of Program

try:
    while True :
        c = readchar.readchar()

        # Quits Program
        
        if c == 'q':
            print("Exited Program")
            a = 0
            b = 0
            d = 0
            pinch = 0

            ServoBase.write(a)
            ServoUp.write(b)
            ServoFor.write(d)
            ServoPinch.write(pinch)

            break

        # Keyboard Shortcuts for Program

        if c == 'a':
            a = a+5
            print("+5 degrees (base)")

        if c == 'd':
            a = a-5
            print("-5 degrees (base)")

        if c == 'w':
            b = b+5
            print("+5 degrees (up)")

        if c == 's':
            b = b-5
            print("-5 degrees (down)")

        if c == 'r':
            d = d+10
            print("+10 degrees (forward)")

        if c == 'f':
            d = d-10
            print("-10 degrees (backward)")

        if c == 'p':
            e = a
            f = b
            g = d
            print("position saved")

        if c == 'o':
            a = e
            b = f
            d = g
            print("moving to saved position")

        if c == 'e':
            pinch = 0 
            print('pinched!')

        if c == 't':
            pinch = 180
            print('un-pinched')

        if c == 'k':
            for pos in positions:
                print("moving to position sequence")
                servoBase.write(pos[0])
                servoUp.write(pos[1])
                servoFor.write(pos[2])
                servoPinch.write(pos[3])
                print("sleeping")
                sleep(2)

        if c =='l':
            temp1 = (a, b, d, pinch)
            positions.append(temp1)

            print("saved position timeframe")

        if a <= 0:
            a = 0

        if a >= 180:
            a = 180
            
        if b <= 0:
            b = 0

        if b >= 180:
            b = 180

        if d <= 0:
            d = 0

        if d >= 180:
            d = 180

        # Writes Command to Robot

        servoBase.write(a)
        servoUp.write(b)
        servoFor.write(d)
        servoPinch.write(pinch)

        sleep(0.001)
except:
    print("You failed for some reason")

        
