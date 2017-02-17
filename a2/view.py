#!/usr/bin/python3
import sys
import curses
import os

def getUser(line):
	words = line.split(" ")
	user = "";
	if (len(words)==0):
		return ""
	for i in range (0,len(words)-1):
		user+=words[i]+" "
	user = user[:-1]
	return user
		
def signin(filename, user):
	if (len(user) == 0):
		return -1
	file = open("messages/"+filename, "r")
	for line in file:
		if (user == getUser(line)):#user == line[0:len(user)]):
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
	return stream.decode()

def displayBar(csr, yMax):
	csr.addstr(yMax-1, 0, "↑   ↓   O-order toggle   M-mark all   S-stream  C-check for new")

def displayScreen(csr, posts):
	csr.addstr(0,0,"ID: "+name)
	c = 1
	#curses.endwin()
	for i in range(0,len(posts)):
		csr.addstr(c,0,"************")
		print("************")
		c+=1
		for line in posts[i]:
			csr.addstr(c,0,line)
			#print(line)
			c+=1
	#exit(0)
	displayBar(csr, size[0])

def loadStream(streamName):
	#if (os.path.isfile(fname))
	stream = open("messages/"+streamName+"Stream", "r")
	streamData = open("messages/"+streamName+"StreamData", "r")
	
	#curses.endwin()

	numBytes = 0
	posts = []
	numposts = 0
	for line in streamData:
		line = line.rstrip("\n")
		postBytes = 0
		postlines = [streamName]
		#print("*********")
		for line2 in stream:
			line2 = line2.rstrip("\n")
			postBytes += len(line2)
			postlines.append(line2)
			if (postBytes+numBytes >= int(line)):
				break
		#print(postlines)
		numBytes += postBytes
		posts.append([])
		posts[numposts] = postlines
		numposts+=1
	#exit(0)
	return posts

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
	size = csr.getmaxyx()
	csr.addstr(2,0,str(size))

	stream = changeStream(csr, name)
	posts = list(loadStream(stream))

	displayScreen(csr, posts)

	while (1==1):
		c = csr.getch(0,0)
		csr.clear()
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
		displayScreen(csr, posts)

	curses.endwin()

