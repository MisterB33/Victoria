"""Flask app to control a robot arm - run on a Raspberry Pi."""
import flask
import time
import serial

arduino = serial.Serial(port="/dev/ttyUSB0", baudrate=115200, timeout =.1)



# Initialize the pinModes 

app = flask.Flask(__name__)


@app.route('/')
def index():
    """Index page."""
    return flask.render_template('index.html')


@app.route('/move/<direction>')
def move(direction):
    
    """Move in a direction."""
    if direction == 'forward':
        arduino.write(bytes(str('w;'), 'utf-8'))
    elif direction == 'backward':
        arduino.write(bytes(str('s;'), 'utf-8'))
    elif direction == 'left': 
        arduino.write(bytes(str('d;'), 'utf-8'))
    elif direction == 'right':
        arduino.write(bytes(str('a;'), 'utf-8'))
    elif direction == 'up':
        arduino.write(bytes(str('e;'), 'utf-8'))
    elif direction == 'down':
        arduino.write(bytes(str('t;'), 'utf-8'))
    elif direction == "open":
        arduino.write(bytes(str('o;'), 'utf-8')) 
    elif direction == "close":
        arduino.write(bytes(str('c;'), 'utf-8'))
    elif direction == "Clear":
        f = open("commands.txt", 'w')
        f.write("")
        f.close()
    elif direction == "Record":
        f = open("commands.txt", "a")
        arduino.write(bytes(str('R;'), 'utf-8'))
        msg = arduino.readline()
        f.write(str(msg.decode("utf-8")))
        f.close()
        print(msg.decode("utf-8"))
        print("\n")
    elif direction == "Play":
        f = open("commands.txt","r")
        lines = f.readlines()
        for line in lines:
            temp = line.strip("\n")
            temp = temp.strip("\r")
            if temp != "":
                arduino.write(bytes(str(temp),'utf-8'))
                arduino.flush()
                time.sleep(1)
        f.close()
    else:
        flask.abort(400, 'Unknown command?!?!')

##    time.sleep(1)

    return 'OK'

if __name__ == '__main__':
    app.debug=True
    app.run()
