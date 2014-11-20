"""
	This is the UDP client for COMP4320 project3

	Author: Nathan Coleman
"""

import socket, struct, sys

# Constants
MAGIC_NUMBER = 0x1234

# Error Codes
ERR_NO_MAGIC_NUM = 0x00
ERR_INVALID_LGTH = 0x01
ERR_INVALID_PORT = 0x02

# Response Length
LEN_MAG_NUM = 2
LEN_REG_ERR = 5
LEN_IP_PORT = 9

class ClientUDP:

	def __init__(self, host, port, play_port, group_id):
		self.host 		= host
		self.port 		= port
		self.play_port 	= play_port
		self.socket 	= socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.group_id 	= group_id
		self.magic_num 	= MAGIC_NUMBER
		
		self.socket.connect((host, port))
		print "Client init:", host, port, play_port, group_id

	def valid_play_port(self):
		floor = 10010 + (5 * self.group_id)
		ceiling = floor + 4
		return floor <= self.play_port <= ceiling

	def send_request(self):
		print "Request:"
		
		print "\tPacking..."
		print "\t\tMagic #:", self.magic_num
		print "\t\tGroup ID:", self.group_id
		print "\t\tPort:", self.play_port
		pack_format = "!HBH"
		packed_data = struct.pack(pack_format, self.magic_num, self.group_id, self.play_port)

		print "\tSending..."
		self.socket.sendall(packed_data)

	def get_response(self):
		print "Response:"

		# TODO: Replace test reponse data with line below once server works
		# packed_date = self.socket.recv(1024) #We should not need to recv more than this

		# Test Response Data
		# ============================================
		ip_format = "!HBBBBBH"
		ip_response = struct.pack(ip_format, MAGIC_NUMBER, 21, 192, 168, 1, 1, 10111)
		
		reg_format = "!HBH"
		reg_response = struct.pack(reg_format, MAGIC_NUMBER, 21, 10111)
		
		err_format = "!HBBB"
		err_response = struct.pack(err_format, MAGIC_NUMBER, 21, 0x00, ERR_NO_MAGIC_NUM)

		mag_format = "!H"
		mag_response = struct.pack(mag_format, MAGIC_NUMBER)

		packed_data = ip_response
		# ============================================

		if len(packed_data) == LEN_MAG_NUM:
			print "\tStarting TCP server on port", self.play_port
			self.establish_server('', self.play_port)

		elif len(packed_data) == LEN_REG_ERR:
			
			if self.is_error(packed_data):
				print "\tResponse is an error"
				print "\t" + self.get_error_msg(packed_data)

			else:
				print "\tResponse is a registration"

				print "\tUnpacking..."
				pack_format = "!HBH" # ! for network order
				magic, group_id, port = struct.unpack(pack_format, packed_data)

				print "\t\tMagic #:", hex(magic)
				print "\t\tGroup ID:", group_id
				print "\t\tPort:", port

		elif len(packed_data) == LEN_IP_PORT:
			print "\tResponse includes IP address and port #"

			print "\tUnpacking..."
			pack_format = "!HBBBBBH" # ! for network order
			magic, group_id, ip1, ip2, ip3, ip4, port = \
				struct.unpack(pack_format, packed_data)

			print "\t\tMagic #:", hex(magic)
			print "\t\tGroup ID:", group_id
			ip = str(ip1) + "." + str(ip2) + "." + str(ip3) + "." + str(ip4)
			print "\t\tIP:" + ip
			print "\t\tPort:", port

			print "\tConnecting to opponent at", ip, ":", port
			self.connect_to_opponent(ip, port) 

		else:
			print "\t** Response was unexpected length **"

	def is_error(self, packed_data):
		pack_format = "!BBBBB"
		# 4th byte is 0x00 for an error
		byte4 = struct.unpack(pack_format, packed_data)[3]
		return byte4 == 0x00

	def get_error_msg(self, packed_data):
		pack_format = "!BBBBB"
		# 5th byte is error type code
		error_type = struct.unpack(pack_format, packed_data)[4]

		if error_type == ERR_NO_MAGIC_NUM:
			return "ERROR: No magic number!"

		elif error_type == ERR_INVALID_LGTH:
			return "ERROR: Invalid length!"

		elif error_type == ERR_INVALID_PORT:
			return "ERROR: Invalid port!"

		return "** Received error with invalid code **"

	def establish_server(self, host, port):
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.bind((host, port))
		self.socket.listen(True)

		while True:
			try:	
				print "\tWaiting for opponent to connect..."
				self.client, self.addr = self.socket.accept()
				print "\tOpponent connected from:", self.addr

			except KeyboardInterrupt:
				print "\nExiting..."
				exit(0) 
			
			finally:
				if hasattr(self, 'client'):
					self.client.close

	def connect_to_opponent(self, host, port):
		self.socket.connect((host, port))


if __name__ == "__main__":
    
	__, host, port, play_port = sys.argv
	host		= str(host)
	port 		= int(port)
	play_port 	= int(play_port)

	print 'Server is at', host, ':', port
	print 'I would like to play on port', play_port

	try:
		client = ClientUDP(host, port, play_port, 20)
		
		if client.valid_play_port():
			client.send_request()
			client.get_response()
		else:
			print "ERROR: The requested port number is invalid."
	
	except KeyboardInterrupt:
		print "\nExiting..."
		exit(0)