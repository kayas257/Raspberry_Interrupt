#
#Author Kayas Ahmed
#

import socket
import sys
from twilio.rest import TwilioRestClient
import pyowm
import time
	
# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = 'echo_socket'
print >>sys.stderr, 'connecting to %s' % server_address
try:
    sock.connect(server_address)
except socket.error, msg:
    print >>sys.stderr, msg
    sys.exit(1)

while True:
	data = sock.recv(16)
	owm=pyowm.OWM('xxxxxxxx')
	acc_sid="xxxxxxxx"
	auth_token="xxxxxxxxx"
	client=TwilioRestClient(acc_sid,auth_token);
	message=client.messages.create(body="Weather in Bangalore is  Humidity is "+str(humidity),to="+917204352825",from_="+1916740-1388")
#	print message.sid
	break
