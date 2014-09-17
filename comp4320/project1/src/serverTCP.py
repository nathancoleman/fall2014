"""
	This is the TCP server for COMP4320 project1

	Author: Nathan Coleman
"""

import socket, struct

class ServerTCP:

	def __init__(self, host, port):
		self.host = host
		self.port = port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.bind((host, port))
		self.socket.listen(1)
		conn, addr = self.socket.accept()
		print "Client connected from:", addr
		while 1:
			#1024 max payload + 5 for header
			data = conn.recv(1029).strip()
			self.process_data(data)
		conn.close()

	def process_data(self, data):
		pack_format = 'HHB'
		header = struct.unpack(pack_format, data[:5])
		header_hex = [hex(x) for x in header]
		message = data[5:]

		print "Header:", header_hex
		print "Message:", message

	def get_vowel_count(self, message):
		# TODO
		print "TODO: get_vowel_count"

	def disemvowel(self, message):
		# TODO
		print "TODO: disemvowel"

	def respond(self, val):
		# TODO
		print "TODO: respond"



if __name__ == "__main__":
    
	try:
		server = ServerTCP('localhost', 8001)

	except KeyboardInterrupt:
		print "\nExiting..."
		exit(0)