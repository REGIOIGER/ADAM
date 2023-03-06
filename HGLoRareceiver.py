import serial         # Import the serial module to read data from the serial port
import json           # Import the json module to parse JSON data
import requests       # Import the requests module to send HTTP GET requests to ThingSpeak
import time           # Import the time module to add delays to the script
import sys            # Import the sys module to handle system-level operations

# Define the URL of the ThingSpeak channel to which data will be sent
url = 'https://api.thingspeak.com/update?api_key=*******************='

# Set up the serial port by setting its parameters
ser = serial.Serial()
ser.port = '/dev/ttyACM0'
ser.baudrate = 9600
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = 1

# Define the list of commands to send to the CatWAN USB Stick LoRa connected to the serial port
cmds = ["set_sf-12", "set_rx"]	

# Wait for 1 second and then open the serial port
time.sleep(1)
ser.open()

# Initialize the command index
e = 0

# If the serial port is open, send the commands
if ser.is_open:
    cmd = cmds[e]
    print(cmd)
    cmd = cmd + '\r\n'

    # Send each character in the command with a small delay between characters
    for i in cmd:
        ser.write(i.encode())
        time.sleep(0.001)		
		
    # Increment the command index and send the next command
    e = e + 1
    cmd = cmds[e]
    print(cmd)
    cmd = cmd + '\r\n'

    for i in cmd:
        ser.write(i.encode())
        time.sleep(0.001)				

# Enter an infinite loop to read data from the serial port and send it to ThingSpeak
while True:				
    # Wait for 10 seconds
    time.sleep(10)
		
    # If there is data available on the serial port, read it and process it
    if ser.in_waiting > 0:
        # Read the data and decode it as a string
        msg = ser.read(ser.in_waiting)
        cad = msg.decode()
        print (cad)
        print("\n")

        # If the string contains a JSON object, extract the data and send it to ThingSpeak
        if "ASCII: '" in cad:
            # Extract the JSON object from the string and parse it as a Python dictionary
            json_str = cad.split("ASCII: '")[1].split("'")[0]
            data = json.loads(json_str)["data"]
            print(data)
            
            # Construct the URL for the ThingSpeak update and send an HTTP GET request to it
            r = url + str(data[0]) + "&field2=" + str(data[1]) + "&field3=" + str(data[2]) + "&field4=" + str(data[3])
            x = requests.get(r)
            print(x.status_code)
			
        # If the string does not contain a JSON object, print a message indicating that it was ignored
        else:
            print("pass")			
            
# Flush the serial port buffer and wait for 2 seconds before closing the port
ser.flush()
time.sleep(2)
ser.close()
