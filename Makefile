all: accumSim

accumSim:
	g++ accumCode.cpp -o accumSim

clean:
	rm accumSim
