### NIM Game ###
- UDP Server stores information of first client to connect
- First client starts a TCP server on the port it requested to play on
- Second client to connect to the UDP server is given the first client's information
- Second client connects to first client's TCP server and the game begins

_Player who leaves opponent with only one token on the board wins_
