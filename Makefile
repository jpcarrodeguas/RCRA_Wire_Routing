# Makefile for RCRA P2 - Wire Routing Puzzle

wirerouting: wirerouting.c
	gcc wirerouting.c -o wirerouting

clean:
	rm -f wirerouting wirerouting.txt Wirerouting_EDB.txt