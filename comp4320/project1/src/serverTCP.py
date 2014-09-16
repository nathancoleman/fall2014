import SocketServer

HOST 		= 'localhost'
PORT_NUMBER = 8001

class TCPHandler(SocketServer.BaseRequestHandler):

	def handle(self):
		print "Handling request"
		self.data = self.request.recv(1024).strip()

print "Running ServerTCP on " + HOST + ":" + str(PORT_NUMBER) + " (Ctrl + C to quit)"

server = SocketServer.TCPServer((HOST, PORT_NUMBER), TCPHandler)
server.serve_forever()