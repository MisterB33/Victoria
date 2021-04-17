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
    if direction == 'forward':
        arduino.write(bytes(str('w;'), 'utf-8'))
    elif direction == 'backward':
        arduino.write(bytes(str('s;'), 'utf-8'))
    elif direction == 'left': 
        arduino.write(bytes(str('d;'), 'utf-8'))
    elif direction == 'right':
        arduino.write(bytes(str('a;'), 'utf-8'))
    elif direction == 'up':
        arduino.write(bytes(str('R;'), 'utf-8'))
    elif direction == 'down':
        arduino.write(bytes(str('S 250,;p 90,90,90,90,;p 170,90,90,90,;'), 'utf-8'))
    elif direction == "Record":
        arduino.write(bytes(str('R;'), 'utf-8'))
        msg = arduino.readline()
        print(msg.decode("utf-8"))
        print("\n")
    else:
        flask.abort(400, 'Unknown command?!?!')

##    time.sleep(1)

    return 'OK'

if __name__ == '__main__':
    app.debug=True
    app.run()
