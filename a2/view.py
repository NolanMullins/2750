#!/usr/bin/python3
import sys
import curses

if (len(sys.argv) <= -1):
	print("No arguments specified")
	exit(0)

name = ""
for x in sys.argv[1:]:
	name+=x+" "
name = name[:-1]

csr = curses.initscr()

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
		csr.addstr(1,0,"S cmd")
	#refresh page
	csr.addstr(0,0,"ID: "+name)

curses.endwin()