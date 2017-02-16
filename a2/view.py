#!/usr/bin/python3
import sys
import curses
import os

def signin(filename, user):
	file = open("messages/"+filename, "r")
	for line in file:
		if (user in line):
			return 1
	return -1

def changeStream (csr, user):
	csr.clear()
	c = 1
	for f in os.listdir("messages"):
		if (f.endswith("StreamUsers") and signin(f, user) == 1):
			csr.addstr(c, 0, f[:-11])
			c+=1
	csr.addstr(0,0,"Enter Stream: ")
	stream = csr.getstr(0,14);
	csr.clear()
	return stream

if __name__ == "__main__":
	if (len(sys.argv) <= -1):
		print("No arguments specified")
		exit(0)

	name = ""
	for x in sys.argv[1:]:
		name+=x+" "
	name = name[:-1]

	csr = curses.initscr()
	curses.start_color()

	stream = changeStream(csr, name)

	csr.addstr(0,0,"ID: "+name)

	while (1==1):
		c = csr.getch(0,0)
		if (c == ord('q')):
			break
		elif (c == ord('o')):
			csr.addstr(1,0,"O cmd")
		elif (c == ord('m')):
			csr.addstr(1,0,"M cmd")
		elif (c == ord('c')):
			csr.addstr(1,0,"C cmd")
		elif (c == ord('s')):
			stream = changeStream(csr, name)
		#refresh page
		csr.clear()
		csr.addstr(0,0,"ID: "+name)

	curses.endwin()

