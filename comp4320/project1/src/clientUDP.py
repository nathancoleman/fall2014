"""
	This is the UDP client for COMP4320 project1

	Author: Nathan Coleman
"""

import socket, struct, sys

GETVLENGTH = 0x55
DISEMVOWEL = 0xAA

class ClientUDP:

	def __init__(self, host, port):
		self.host = host
		self.port = port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.socket.connect((host, port))
		self.requestID = 1

	def send_request(self, op, message):
		message_length = 5 + len(message) # 5 for required tml, id, and op
		pack_format = "HHB"
		packed_data = struct.pack(pack_format, message_length, self.requestID, op)
		packed_data = packed_data + message
		self.socket.sendall(packed_data)
		self.requestID += 1

		if op == GETVLENGTH:
			self.parse_getvlength()

		elif op == DISEMVOWEL:
			self.parse_disemvowel()

	def get_response(self):
		# 1024 max payload + 5 for header
		data = self.socket.recv(1029)

	def parse_getvlength(self):
		# TODO
		print "TODO: parse_getvlength"

	def parse_disemvowel(self):
		# TODO
		print "TODO: parse_disemvowel"



if __name__ == "__main__":
    
	host, port, op, message = sys.argv[1:]
	host	= str(host)
	port 	= int(port)
	op 		= int(op)
	message = str(message)

	print 'Starting client at ' + host + ':' + str(port)

	try:
		client = ClientUDP(host, port)
		client.send_request(op, message)
	
	except KeyboardInterrupt:
		print "\nExiting..."
		exit(0)