"""
	This is the UDP client for COMP4320 project1

	Author: Nathan Coleman
"""

import socket, struct

class ClientUDP:

	def __init__(self, host, port):
		self.host = host
		self.port = port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.socket.connect((host, port))
		self.requestID = 0

	def get_vowel_count(self, message):
		# TODO
		print "TODO: get_vowel_count"

	def disemvowel(self, message):
		# TODO
		print "TODO: disemvowel"

	def send_request(self, op, message):
		message_length = 5 + len(message) # 5 for required tml, id, and op
		pack_format = "HHB"
		packed_data = struct.pack(pack_format, message_length, self.requestID, op)
		packed_data = packed_data + message
		self.socket.sendall(packed_data)
		self.requestID += 1

	def get_response(self):
		# 1024 max payload + 5 for header
		data = self.socket.recv(1029)



if __name__ == "__main__":
    
    try:
    	client = ClientUDP('localhost', 8001)
    	client.send_request(1, "Hello world")

    except KeyboardInterrupt:
		print "\nExiting..."
		exit(0)