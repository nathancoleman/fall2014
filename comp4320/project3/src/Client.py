"""
	This is the UDP client for COMP4320 project3

	Author: Nathan Coleman
"""

import socket, struct, sys

# Error Codes
ERR_NO_MAGIC_NUM = 0x00
ERR_INVALID_LGTH = 0x01
ERR_INVALID_PORT = 0x02

# Response Length
LEN_REG_ERR = 5
LEN_IP_PORT = 9

class ClientUDP:

	def __init__(self, host, port, play_port, group_id):
		self.host 		= host
		self.port 		= port
		self.play_port 	= play_port
		self.socket 	= socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.group_id 	= group_id
		
		self.socket.connect((host, port))
		print "Client init:", host, port, play_port, group_id

	def valid_play_port(self):
		floor = 10010 + (5 * self.group_id)
		ceiling = floor + 4
		return floor <= self.play_port <= ceiling

	def send_request(self):
		print "Request:"
		
		print "\tPacking..."
		pack_format = "!BBBH"
		packed_data = struct.pack(pack_format, 0x12, 0x34, self.group_id, self.play_port)

		print "\tSending..."
		self.socket.sendall(packed_data)

	def get_response(self):
		print "Response:"

		# TODO: Replace test reponse data with line below once server works
		# packed_date = self.socket.recv(1024) #We should not need to recv more than this

		# Test Response Data
		# ============================================
		ip_format = "!BBBBBBBH"
		ip_response = struct.pack(ip_format, 0x12, 0x34, 21, 192, 168, 1, 1, 10111)
		
		reg_format = "!BBBBB"
		reg_response = struct.pack(reg_format, 0x12, 0x34, 21, 0x37, 0x7F)
		
		err_format = "!BBBBB"
		err_reponse = struct.pack(err_format, 0x12, 0x34, 21, 0x00, ERR_NO_MAGIC_NUM)

		packed_data = ip_response
		# ============================================

		if len(packed_data) == LEN_REG_ERR:
			
			if self.is_error(packed_data):
				print "\tResponse is an error"
				print "\t" + self.get_error_msg(packed_data)

			else:
				print "\tResponse is a registration"

				print "\tUnpacking..."
				pack_format = "!BBBH" # ! for network order
				magic1, magic2, group_id, port = struct.unpack(pack_format, packed_data)

				print "\t\tMagic #:", hex(magic1), hex(magic2)
				print "\t\tGroup ID:", group_id
				print "\t\tPort:", port

		elif len(packed_data) == LEN_IP_PORT:
			print "\tResponse includes IP address and port #"

			print "\tUnpacking..."
			pack_format = "!BBBBBBBH" # ! for network order
			magic1, magic2, group_id, ip1, ip2, ip3, ip4, port = \
				struct.unpack(pack_format, packed_data)

			print "\t\tMagic #:", hex(magic1), hex(magic2)
			print "\t\tGroup ID:", group_id
			print "\t\tIP:" + str(ip1) + "." + str(ip2) + "." + str(ip3) + "." + str(ip4)
			print "\t\tPort:", port

		else:
			print "\t** Response was unexpected length **"

	def is_error(self, packed_data):
		pack_format = "!BBBBB"
		# __, __, __, byte4, __ = struct.unpack(pack_format, packed_data)
		# 4th byte is 0x00 for an error
		byte4 = struct.unpack(pack_format, packed_data)[3]
		return byte4 == 0x00

	def get_error_msg(self, packed_data):
		pack_format = "!BBBBB"
		__, __, __, __, error_type = struct.unpack(pack_format, packed_data)

		if error_type == ERR_NO_MAGIC_NUM:
			return "ERROR: No magic number!"

		elif error_type == ERR_INVALID_LGTH:
			return "ERROR: Invalid length!"

		elif error_type == ERR_INVALID_PORT:
			return "ERROR: Invalid port!"

		return "** Received error with invalid code **"


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