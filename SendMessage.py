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
	owm=pyowm.OWM('097066ffd32d0c0dd25fc703cfec018a')
	acc_sid="AC28ea3e2130e49c67d2fc978034a85762"
	auth_token="4c7180582e1f42b0c2d5890a9f447952"
	client=TwilioRestClient(acc_sid,auth_token);
	message=client.messages.create(body="Weather in Bangalore is  Humidity is "+str(humidity),to="+917204352825",from_="+1916740-1388")
#	print message.sid
	break
