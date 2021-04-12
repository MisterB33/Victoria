"""Flask app to control a robot arm - run on a Raspberry Pi."""
import flask
import RPi.GPIO as GPIO
import time
import serial

Rot = 0 
arduino = serial.Serial(port="/dev/ttyUSB0", baudrate=115200, timeout =.1)
print(Rot)



# Initialize the pinModes 

app = flask.Flask(__name__)


@app.route('/')
def index():
    """Index page."""
    return flask.render_template('index.html')


@app.route('/move/<direction>')
def move(direction):
    
    """Move in a direction."""
    global Rot
    if direction == 'up':
        Rot = Rot+100
        if Rot >= 1000:
            Rot = 1000
        print(Rot)
        arduino.write(bytes(str('w'), 'utf-8'))
        #data = arduino.readline()
        #print(data)
    elif direction == 'down':
        if Rot <= 0 : 
            Rot = 0
        print(Rot)
        Rot = Rot-100
        arduino.write(bytes(str('s'), 'utf-8'))
        data = arduino.readline()
        print(data)
    elif direction == 'left':
        pin_number = 12
    elif direction == 'right':
        pin_number = 13
    else:
        flask.abort(400, 'Unknown command?!?!')

    time.sleep(1)

    return 'OK'

if __name__ == '__main__':
    app.debug=True
    app.run()
