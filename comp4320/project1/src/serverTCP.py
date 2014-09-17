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
			self.process_connection(conn)
		conn.close()

	def process_connection(self, conn):
		# How much data should we take in here?
		data = conn.recv(1023).strip()
	
		pack_format = '!HHB'
		header = struct.unpack(pack_format, data[:5])
		message = data[5:]
		form_header = [hex(x) for x in header]

		print "Header:", form_header
		print "Message:", message



if __name__ == "__main__":
    
	try:
		server = ServerTCP('localhost', 8001)

	except KeyboardInterrupt:
		print "\nExiting..."
		exit(0)