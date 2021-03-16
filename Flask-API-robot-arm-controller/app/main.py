"""Flask app to control a robot arm - run on a Raspberry Pi or a general computer."""
import flask
from flask import render_template,url_for,request,redirect, make_response
import json 

import RPi.GPIO as GPIO #I do not need this pin
import time #for time keeping 
import serial #I use this for Serial Communication with the arduino

Rot = 0
Main_Arm_Pos = 0 
Raise_Arm_Pos = 0 
Base_Pos = 0 
Claw_Pos = 0 
Positions = [] # Not sure if I need this or not

print("Establishing Connecting to Arduino...")
arduino = serial.Serial(port="/dev/ttyUSB0",baudrate=115200,timeout =.1)
print("Successful Connection to Arduino!...")

def UpdatePosition(ser,Pos):
    ser.write(bytes(str(Pos),'utf-8'))
    ## need to add a wait connection recieved form the arduino 
    return 0


# Initialize the pinModes 
def create_app(testing: bool = True, VERBOSE: bool = True):
    app = flask.Flask(__name__)
    #print("Establishing Connecting to Arduino...")
    #arduino = serial.Serial(port="/dev/ttyUSB0",baudrate=115200,timeout =.1)
    #print("Successful Connection to Arduino!...")
    if VERBOSE :
        print(Rot)

    @app.route('/')
    def index():
        """Index page."""
        return flask.render_template('index.html')
    @app.route('/',methods = ["GET","POST"])
    def roboteMove(): 
        global Base_Pos 
        global Claw_Pos 
        global Raise_Arm_Pos 
        global Main_Arm_Pos 
        data = [Base_Pos, Claw_Pos, Raise_Arm_Pos, Main_Arm_Pos]
        response = make_response(json.dumps(data)) 
        return positions

    @app.route('/move/<direction>', methods = ["GET","POST"])
    def move(direction):
    
        """Move in a direction."""
        global Rot
        if direction == 'up':
            Rot = Rot+100
            if Rot >= 1000:
                Rot = 1000
            print(Rot)
        elif direction == 'down':
            Rot = Rot - 100
            if Rot <= 0 : 
                Rot = 0
                print(Rot)
        elif direction == 'left':
            pin_number = 12
        elif direction == 'right':
            pin_number = 13
        else:
            flask.abort(400, 'Unknown command?!?!')
        UpdatePosition(arduino,Rot)
        time.sleep(.05)
        return 'OK'
    return app 
