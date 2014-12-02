"""
	This is the UDP client for COMP4320 project3

	Author: Nathan Coleman
"""

import os, socket, struct, sys

# Constants
GROUP_ID = 20
MAGIC_NUMBER = 0x1234

# Error Codes
ERR_NO_MAGIC_NUM = 0x00
ERR_INVALID_LGTH = 0x01
ERR_INVALID_PORT = 0x02

# Response Length
LEN_MAG_NUM = 2
LEN_REG_ERR = 5
LEN_IP_PORT = 9

# Game
GAME = [1, 3, 5, 7]

class ClientUDP:

	def __init__(self, host, port, play_port, group_id):
		self.host 		= host
		self.port 		= port
		self.play_port 	= play_port
		self.socket 	= socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.group_id 	= group_id
		self.magic_num 	= MAGIC_NUMBER
		self.is_server 	= False
		
		self.socket.connect((host, port))
		print "Client init:", host, port, play_port, group_id

	
	def send_request(self):
		print "Request:"
		
		print "\tPacking..."
		print "\t\tMagic #:", hex(self.magic_num)
		print "\t\tGroup ID:", self.group_id
		print "\t\tPort:", self.play_port
		pack_format = "!HBH"
		packed_data = struct.pack(pack_format, self.magic_num, self.group_id, self.play_port)

		print "\tSending..."
		self.socket.sendall(packed_data)

	
	def get_response(self):
		print "Response:"

		packed_data = self.socket.recv(1024) #We should not need to recv more than this

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

				print "\tStarting TCP server on port", port
				self.establish_server('', port)

		elif len(packed_data) == LEN_IP_PORT:
			print "\tResponse includes IP address and port #"

			print "\tUnpacking..."
			pack_format = "!HBBBBBH" # ! for network order
			magic, group_id, ip1, ip2, ip3, ip4, port = \
				struct.unpack(pack_format, packed_data)

			print "\t\tMagic #:", hex(magic)
			print "\t\tGroup ID:", group_id
			ip = str(ip1) + "." + str(ip2) + "." + str(ip3) + "." + str(ip4)
			print "\t\tIP:", ip
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
		self.is_server = True
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.bind((host, port))
		self.socket.listen(True)

		waiting = True

		while waiting:
			try:	
				print "\tWaiting for opponent to connect...\n"
				self.client, self.addr = self.socket.accept()
				print "\tOpponent connected from:", self.addr[0]
				waiting = False

			except KeyboardInterrupt:
				print "\nExiting..."
				exit(0) 
			
			# finally:
			# 	if hasattr(self, 'client'):
			# 		self.client.close

		self.setup_game_on_server()

	
	def connect_to_opponent(self, host, port):
		self.is_server = False
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket.connect((host, port))

		while not self.game_over():

			self.print_game()

			row, num_remove = self.request_move()
			while not self.valid_move(row, num_remove):
				self.print_game()
				print "\n\t\tINVALID MOVE: Please enter a valid move"
				row, num_remove = self.request_move()
			
			# valid_move() assures these are castable to int
			row = int(row)
			num_remove = int(num_remove)
			
			self.update_game(row, num_remove)
			self.send_move(row, num_remove)

			if self.game_over():
				print "\n\t\tYOU WIN!\n\n"

			else:
				print("\n\t\tWaiting for opponent...")
				row, num_remove = self.get_move()
				self.update_game(row, num_remove)

				if self.game_over():
					print "\n\t\tYOU LOSE!\n\n" 


	def setup_game_on_server(self):
		print "\tSetting up game..."

		while not self.game_over():

			self.print_game()

			print("\n\t\tWaiting for opponent...")
			row, num_remove = self.get_move()
			self.update_game(row, num_remove)

			if self.game_over():
				print "\n\t\tYOU LOSE!\n\n"
				self.client.close()
				exit(0)

			row, num_remove = self.request_move()
			while not self.valid_move(row, num_remove):
				self.print_game()
				print "\n\t\tINVALID MOVE: Please enter a valid move"
				row, num_remove = self.request_move()

			# valid_move() assures these are castable to int
			row = int(row)
			num_remove = int(num_remove)
			
			self.update_game(row, num_remove)
			self.send_move(row, num_remove)

			if self.game_over():
				print "\n\t\tYOU WIN!\n\n"
				self.client.close()
				exit(0)

	
	def print_game(self):
		os.system('clear')

		print "\n\t\tRow #\t# Tokens"
		print "\t\t----------------"
		for i in range(0, len(GAME)):
			print "\t\t", (i + 1), "\t", GAME[i]


	def game_over(self):
		return self.tokens_remaining() == 1

	def tokens_remaining(self):
		remaining = 0

		for i in range(0, len(GAME)):
			remaining += GAME[i]

		return remaining

	
	def send_move(self, row, num_remove):
		pack_format = "!HBBB"
		packed_data = struct.pack(pack_format, self.magic_num, self.group_id, row, num_remove)

		if self.is_server:
			self.client.send(packed_data)
		else:
			self.socket.send(packed_data)

	
	def get_move(self):
		pack_format = "!HBBB"
		
		if self.is_server:
			packed_data = self.client.recv(1024)
		else:
			packed_data = self.socket.recv(1024)

		magic, group_id, row, num_remove = struct.unpack(pack_format, packed_data)

		return row, num_remove


	def request_move(self):
		row = raw_input("\n\t\tR:\t")
		num_remove = raw_input("\t\tN:\t")
		return row, num_remove


	def valid_move(self, row, num_remove):
		try:
			row = int(row)
			num_remove = int(num_remove)
		except ValueError:
			return False

		if row not in range(1, len(GAME) + 1):
			return False
		if num_remove not in range(0, GAME[row - 1] + 1):
			return False
		if num_remove >= self.tokens_remaining():
			return False

		return True

	def update_game(self, row, num_remove):
		GAME[row - 1] -= num_remove
		self.print_game()


if __name__ == "__main__":
    
	__, host, port, play_port = sys.argv
	host		= str(host)
	port 		= int(port)
	play_port 	= int(play_port)

	print 'Server is at', host, ':', port
	print 'I would like to play on port', play_port

	try:
		client = ClientUDP(host, port, play_port, GROUP_ID)
		client.send_request()
		client.get_response()
	
	except KeyboardInterrupt:
		print "\nExiting..."
		exit(0)