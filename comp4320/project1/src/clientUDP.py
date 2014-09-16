import socket, struct

class ClientUDP:

	def __init__(self, host, port):
		self.host = host
		self.port = port
		self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.socket.connect((host, port))
		self.requestID = 0



	def getVLength(input):
		# TODO
		print "TODO: getVLength"



	def disemvowel(input):
		# TODO
		print "TODO: disemvowel"



if __name__ == "__main__":
    
    client = ClientUDP('localhost', 8001)