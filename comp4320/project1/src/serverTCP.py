"""
	This is the TCP server for COMP4320 project1

	Author: Nathan Coleman
"""

import socket, struct, sys

GETVLENGTH = 0x55
DISEMVOWEL = 0xAA

class ServerTCP:

	def __init__(self, host, port):
		self.host = host
		self.port = port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.bind((host, port))
		self.socket.listen(1)
		self.conn, self.addr = self.socket.accept()
		print "Client connected from:", self.addr
		while 1:
			#1024 max payload + 5 for header
			data = self.conn.recv(1029).strip()
			self.process_data(data)
		conn.close()

	def process_data(self, data):
		pack_format = 'HHB'
		message_length, request_id, op = struct.unpack(pack_format, data[:5])
		op = int(chr(op))
		message = data[5:]

		if op == GETVLENGTH:
			vowel_count = self.get_vowel_count(message)
			pack_format = 'HHH'
			message_length = 6
			packed_data = struct.pack(pack_format, message_length, request_id, vowel_count)
			self.conn.send(packed_data)

		elif op == DISEMVOWEL:
			disemvoweled_message = self.disemvowel(message)
			pack_format = 'HH'
			message_length = 4 + len(disemvoweled_message)
			packed_data = struct.pack(pack_format, message_length, request_id)
			packed_data = packed_data + disemvoweled_message
			self.conn.send(packed_data)


	def get_vowel_count(self, message):
		vowels = "aeiouAEIOU"
		count = 0

		for character in message:
			if character in vowels:
				count = count + 1

		return count

	def disemvowel(self, message):
		vowels = "aeiouAEIOU"

		for vowel in vowels:
			message = message.replace(vowel, '')

		return message



if __name__ == "__main__":
    
	host, port = sys.argv[1:]
	host = str(host)
	port = int(port)

	print 'Starting server at ' + host + ':' + str(port)

	try:
		server = ServerTCP(host, port)

	except KeyboardInterrupt:
		print "\nExiting..."
		exit(0)