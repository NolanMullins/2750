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

def getRead(filename, user):
	if (len(user) == 0):
		return -1
	file = open("messages/"+filename+"StreamUsers", "r")
	for line in file:
		if (user == getUser(line)):#user == line[0:len(user)]):
			arr = line.split(" ")
			return int(arr[-1])
	return 0

def setRead(filename, user, postNum):
	return 0

def loadStream(streamName, user):
	if (not os.path.isfile("messages/"+streamName+"Stream")):
		return []
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
			#print(line2)
			#print(postBytes+numBytes)
			if (postBytes+numBytes >= int(line)):
				break
		#print(postlines)
		numBytes += postBytes
		posts.append([])
		posts[numposts] = postlines
		numposts+=1
	#exit(0)
	return sortPosts(posts)

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
	posts = []
	index = 0
	nameIndex = ""
	dateIndex = ""
	if (stream.decode() == "all"):
		print("all")
		for f in os.listdir("messages"):
			if (f.endswith("Stream") and signin(f+"Users", user) == 1):
				streamPosts = (list(loadStream(f[:-6], name)))
				
				if (len(dateIndex) != 0):
					tmpIndex = getRead(f[:-6], name)
					#Tmp is older than index
					if (cmp(dateIndex, streamPosts[tmpIndex][2]) == 0):
						index = tmpIndex
						nameIndex = f[:-6]
						dateIndex = streamPosts[tmpIndex][2]
				else:
					index = getRead(f[:-6], name)
					nameIndex = f[:-6]
					dateIndex = streamPosts[index][2]
				posts += streamPosts
	else:
		if (not os.path.isfile("messages/"+stream.decode()+"Stream")):
			return []
		posts = list(loadStream(stream.decode(), name))
		index = getRead(stream.decode(), name)
		nameIndex = stream.decode()
	posts = sortPosts(posts)
	return posts

def displayBar(csr, yMax):
	csr.addstr(yMax-1, 0, "↑   ↓   O-order toggle   M-mark all   S-stream  C-check for new")

def displayScreen(csr, posts, size, index):
	csr.addstr(0,0,"ID: "+name)
	displayBar(csr, size[0])
	c = 1
	numPosts = 0
	#curses.endwin()
	for i in range(index,len(posts)):
		numPosts+=1
		if (c > size[0]-2):
			return numPosts-1
		csr.addstr(c,0,"************")
		c+=1
		if (c > size[0]-2):
			return numPosts-1
		csr.addstr(c,0,"Stream: "+posts[i][0])
		c+=1
		if (c > size[0]-2):
			return numPosts-1
		csr.addstr(c,0,"User: "+posts[i][1])
		#print("************")
		c+=1
		for j in range(2,len(posts[i])):
			if (c > size[0]-2):
				return numPosts-1
			csr.addstr(c,0,posts[i][j])
			c+=1
		#for line in posts[i]:
		#	csr.addstr(c,0,line)
		#	#print(line)
		#	c+=1
	#exit(0)
	
	return numPosts

def cmpIntList(a, b):
	for i in range(len(a)-1, -1, -1):
		if (int(a[i]) > int(b[i])):
			return 0
		elif (int(a[i]) < int(b[i])):
			return 1
	return -1

#will return 1 if 2 is newer than 1
def cmp(date1, date2):
	time1 = date1.split(":")
	time2 = date2.split(":")
	r = -1
	tmp = cmpIntList(time1[1].split("/"), time2[1].split("/"))
	tmp2 = cmpIntList(time1[0].split("/"), time2[0].split("/"))
	if (tmp!=-1):
		r = tmp
	elif (tmp2!=-1):
		r = tmp2
	#if (r == 1):
	#	print(date1+ " > " + date2)
	#else:
	#	print(date1+ " < " + date2)
	return r
	
def sortPosts(posts):
	for a in range(0,len(posts)-1):
		for b in range(0, len(posts)-1):
			if (cmp(posts[b][2], posts[b+1][2]) == 0):
				tmp = posts[b]
				posts[b] = posts[b+1]
				posts[b+1] = tmp
	return posts

def pageUpShift(posts, index, csr):
	if (index == 0):
		return 0
	screenFill = 0
	offset = 1
	while screenFill < 24 and index-offset >= 0:
		#csr.addstr(28+offset, 0, "index up: "+str(index)+"  lines: "+str(len(posts[index-offset])+1))
		screenFill += len(posts[index-offset])+1
		offset += 1
	offset-=1
	if (screenFill > 24):
		offset -= 1

	return offset

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
	size = list(csr.getmaxyx())
	if (size[0] > 24):
		size[0] = 24

	#stream = changeStream(csr, name)
	posts = list(changeStream(csr,name))

	index = 0
	numPostsOnScreen = displayScreen(csr, posts, size, index)
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
			#stream = changeStream(csr, name)
			#posts = list(loadStream(stream, name))
			posts = (list(changeStream(csr,name)))
			index = 0
		elif (c == 65): #up
			if (index > 0):
				index -= pageUpShift(posts, index, csr)
		elif (c == 66): #down
			if (index+numPostsOnScreen < len(posts)):
				index += numPostsOnScreen
		#refresh page
		numPostsOnScreen = displayScreen(csr, posts, size, index)
	curses.endwin()

