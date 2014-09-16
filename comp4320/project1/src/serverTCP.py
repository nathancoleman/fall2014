import socket, SocketServer

HOST = 'localhost'
PORT = 8001

#	ThreadingMixIn allows a new thread to be created
#	for every request instead of servicing one request
#	at a time

class MultiThreadTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
	
	daemon_threads = True
	allow_reuse_address = True

	def __init__(self, server_address, RequestHandlerClass):
		SocketServer.TCPServer.__init__(self, server_address, RequestHandlerClass)



class TCPHandler(SocketServer.BaseRequestHandler):

	def handle(self):
		self.data = self.request.recv(1024)
		print "Received: " + str(self.data)



if __name__ == "__main__":
    
    server = MultiThreadTCPServer((HOST, PORT), TCPHandler)
    
    try:
    	print "Running server at " + HOST + ":" + str(PORT)
    	print "(Ctrl+C to terminate)"
        server.serve_forever()

    except KeyboardInterrupt:
        sys.exit(0)