"""
	This is the UDP server for COMP4320 project2

	Run from command line $ python serverUDP.py "PORT#"

	Author: Lucas Saltz
"""


"""
ii.	Port# is a port number. While developing your code, Port# must be set to 10010+GID. 
	When the server receives a request, it checks whether the request is VALID. 
	If the request is not valid because of the checksum, the server must return a message with the header 
	(Checksum, GID, RequestID) followed by 2 bytes set each to 0x00. If the request is not valid because it
	is too short/lomgth (length mistmach), the server must return a message with the header (Checksum, 127, 127) 
	followed by 2 bytes set each to 0x00. If the message is a VALID request, the server must resolve the IP address 
	for each hostname and send back a message that contains: 
1)	Length:  Length is stored on two bytes (Byte0, Byte1). 
	Length is the number of bytes making the packet (including the length field)
2)	Checksum:  Checksum is stored on the third byte (Byte2). 
	Checksum is computed as follows: consider the message as an array of bytes. 
	Add all those these bytes WITHOUT CARRY (i.e, do addition of bytes). 
	Let S be the ONE BYTE sum of these numbers. Checksum is the bitwise complement of S.
3)	GID: GID is the group ID of the group owner of the server. It is stored on one byte (Byte3).
4)	RequestID: RequestID is stored on the third byte (Byte4).
5)	all IPs stored as four byte numbers stored in the network byte order (without delimiters). 
	If the server does not resolve a hostname, it must return the IP address 255.255.255.255 (0xffffffff)

"""

import socket
import sys

class serverUDP:
	"""docstring for serverUDP"""
	def __init__(self, arg): ##auto created method
		super(serverUDP, self).__init__()
		self.arg = arg
		
	def checkCheckSum(self, message):
		sumNum = int(0);
		for i in message:
			sumNum += int(i)
			sumNum = (sumNum & 0xff) + (sumNum >> 8)
			sumNum = sumNum & 0xff

		return sumNum & 0xff

	def checkSum(self, valueList):
		sumNum = int(i)
		for i in valueList:
			sumNum += int(i)
			sumNum = (sumNum & 0xff) + (sumNum >> 8)
			sumNum = sumNum & 0xff

			
		return ~sumNum & 0xff

if __name__ == "__main__":
	HOST = '' # Symbolic name meaning all available interfaces
	PORT = int(sys.argv[1]) # Port number from arguments

	print 'Starting server'

	# Datagram (udp) socket
	try:
		s =  socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	except socket.error, msg:
		print 'Failed to create the socket'
	msg[1]
		sys.exit()
	# Bind socket to local host and port
	try:
		s.bind((socket.gethostname(), PORT))
	except socket.error, msg:
		print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
		sys.exit()

	while 1:
		d = s.recfrom(1024)
		data = bytearray(d[0])
		addr = d[1]

		if not data:
			break

		checkValue = checkCheckSum(data)

		if checkValue == 255:
			print 'invalid message = ' + bytes(checkValue)
			lengthOfPacket = int((data[0]<< 8) + data[1]) & 0xffff
			if lengthOfPacket != len(data):
				break
			actualMessage = bytearray(data[6:lengthOfPacket])

			stringOfActualMessage = actualMessage.decode('latin-1')
			listOfHosts = stringOfActualMessage.split(chr(data[5]))

			length = len(listOfHosts) * 4 + 5

			sendingMessage = bytearray()
			sendingMessage.append(length>>8)
			sendingMessage.append(length & 0xff)
			sendingMessage.append(0)
			sendingMessage.append(data[3])
			sendingMessage.append(data[4])
			for host in listOfHosts:
				try: 
					address = socket.gethostname(host)
					print 'got the address --> socket.gethostname(host)'
					ip = address.split('.')
					for num in ip:
						sendingMessage.append(int(num) & 0xff)
				except socket.error as error:
					print 'error in for host in listOfHosts'
					for i in range(4):
						sendingMessage.append(0xff)
			sendingMessage[2] = checkSum(sendingMessage)

			s.sendto(sendingMessage, addr)
		else:
			print 'message was invalid = ' + bytes(checkValue)
			sendingMessage = bytearray()
			sendingMessage.append(0)
			sendingMessage.append(7)
			sendingMessage.append(checkValue)
			sendingMessage.append(data[3])
			sendingMessage.append(data[4])
			sendingMessage.append(0)
			sendingMessage.append(0)
			
			s.sendto(sendingMessage, addr)
