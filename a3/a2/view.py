#!/usr/bin/python3
import sys
import curses
import os
import datetime
import traceback

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
	if (len(user) == 0):
		return -1
	#curses.endwin()
	file = open("messages/"+filename+"StreamUsers", "r")
	newFile = []
	for line in file:
		if (user == getUser(line)):#user == line[0:len(user)]):
			arr = line.split(" ")
			newline = ""
			for a in range(0, len(arr)-1):
				newline += arr[a]+" "
			newline += str(postNum)
			newFile.append(newline)
		else:
			newFile.append(line.rstrip("\n"))
	file = open("messages/"+filename+"StreamUsers", "w")
	for line in newFile:
		file.write(line+"\n")
	#print(newFile)
	#exit(0)
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

def indexOfXStream(stream, postNum, posts):
	index = 0
	streamPos = 0
	for a in range(0, len(posts)):
		if (posts[a][0] == stream):
			if (streamPos == postNum):
				return index
			streamPos += 1
		index += 1
	return index

def indexInStream(stream, indexMaster, posts):
	streamPos = 0
	for a in range(0, len(posts)):
		if (posts[a][0] == stream):
			streamPos += 1
		if (a == indexMaster):
			return streamPos
	return streamPos

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
					if (tmpIndex < len(streamPosts)):
						if (cmp(dateIndex, streamPosts[tmpIndex][2]) == 0):
							index = tmpIndex
							nameIndex = f[:-6]
							dateIndex = streamPosts[tmpIndex][2]
				else:
					index = getRead(f[:-6], name)
					nameIndex = f[:-6]
					if (index >= len(streamPosts) and index > 0):
						index = -1
					else: 
						dateIndex = streamPosts[index][2]
				posts += streamPosts
	else:
		if (not os.path.isfile("messages/"+stream.decode()+"Stream")):
			return 0,[], ""
		posts = list(loadStream(stream.decode(), name))
		index = getRead(stream.decode(), name)
		nameIndex = stream.decode()
	posts = sortPosts(posts)
	#get index of first unread post in stream
	if (index == -1):
		index = len(posts)-1
	else:
		index = indexOfXStream(nameIndex, index, posts)
	return index, posts, stream.decode()

def refreshStream (csr, user, stream):
	posts = []
	index = 0
	nameIndex = ""
	dateIndex = ""
	if (stream == "all"):
		print("all")
		for f in os.listdir("messages"):
			if (f.endswith("Stream") and signin(f+"Users", user) == 1):
				streamPosts = (list(loadStream(f[:-6], name)))
				
				if (len(dateIndex) != 0):
					tmpIndex = getRead(f[:-6], name)

					#Tmp is older than index
					if (tmpIndex < len(streamPosts)):
						if (cmp(dateIndex, streamPosts[tmpIndex][2]) == 0):
							index = tmpIndex
							nameIndex = f[:-6]
							dateIndex = streamPosts[tmpIndex][2]
				else:
					index = getRead(f[:-6], name)
					nameIndex = f[:-6]
					if (index >= len(streamPosts)):
						index = -1
					else: 
						dateIndex = streamPosts[index][2]
				posts += streamPosts
	else:
		if (not os.path.isfile("messages/"+stream+"Stream")):
			return 0,[]
		posts = list(loadStream(stream, name))
		index = getRead(stream, name)
		nameIndex = stream
	posts = sortPosts(posts)
	#get index of first unread post in stream
	if (index == -1):
		index = len(posts)-1
	else:
		index = indexOfXStream(nameIndex, index, posts)
	return index, posts

def displayBar(csr, yMax):
	csr.addstr(yMax-1, 0, "↑   ↓   O-order toggle   M-mark all   S-stream  C-check for new")

def getTime(line):
	line = line.split(":")
	date = line[1].split("/")
	time = line[0].split("/")
	dateTime = datetime.datetime(int(date[2]), int(date[1]), int(date[0]))
	strTime = dateTime.strftime("%b %d, %y - ")
	strTime += time[2]+":"+time[1]
	return strTime

def displayScreen(csr, posts, size, index, user, flag, lineStart):
	csr.addstr(0,0,"ID: "+name)
	displayBar(csr, size[0])
	c = 1
	numPosts = 0
	#curses.endwin()
	if (index >= len(posts) and index > 0):
		index -= 1
	linesIn = 0
	for i in range(index,len(posts)):
		numPosts+=1
		linesIn = 0
		if (c > size[0]-2):
			return numPosts-1, linesIn
		csr.addstr(c,0,"************")
		c+=1
		if (c > size[0]-2):
			return numPosts-1, linesIn
		csr.addstr(c,0,"Stream: "+posts[i][0])
		c+=1
		if (c > size[0]-2):
			return numPosts-1, linesIn
		csr.addstr(c,0,"User: "+posts[i][1])
		c+=1
		#date
		if (c > size[0]-2):
			return numPosts-1, linesIn

		date = getTime(posts[i][2])
		csr.addstr(c,0,"Date: "+date)
		c+=1
		#text section
		linesIn += lineStart
		for j in range(3+lineStart,len(posts[i])):
			if (len(posts[i][j]) > 80):
				lines = posts[i][j]
				lines = [lines[i:i+80] for i in range(0, len(lines), 80)]
				for a in range(0,len(lines)):
					if (c > size[0]-2):
						return numPosts-1, linesIn
					csr.addstr(c,0,lines[a])
					c+=1
					linesIn+=1
			else:
				if (c > size[0]-2):
					return numPosts-1, linesIn
				csr.addstr(c,0,posts[i][j])
				c+=1
				linesIn+=1
		lineStart = 0
		linesIn = 0
		#mark post as read
		indexIn = indexInStream(posts[i][0], i, posts)
		if (flag == 1 and getRead(posts[i][0], user) < indexIn):
			setRead(posts[i][0], user,  indexIn)
		#for line in posts[i]:
		#	csr.addstr(c,0,line)
		#	#print(line)
		#	c+=1
	#exit(0)
	
	return numPosts, linesIn

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

def sortPostsAuthor(posts):
	for a in range(0,len(posts)-1):
		for b in range(0, len(posts)-1):
			if (posts[b][1] > posts[b+1][1]):
				tmp = posts[b]
				posts[b] = posts[b+1]
				posts[b+1] = tmp
	return posts

def pageUpShift(posts, index, csr, lineStart):
	if (index == 0):
		return 0, 0
	screenFill = 0
	offset = 1
	while screenFill < 24 and index-offset >= 0:
		#csr.addstr(28+offset, 0, "index up: "+str(index)+"  lines: "+str(len(posts[index-offset])+1))
		screenFill += len(posts[index-offset])+1
		offset += 1
	offset-=1
	if (screenFill > 24):
		offset -= 1
		if (offset == 0):
			offset = 1
			lenOfPost = len(posts[index])-3
			lineStart = 0
		else:
			lineStart = 0
	else:
		lineStart = 0

	return offset, lineStart

def markAllRead(posts, user):
	stream = {}
	for a in range(0,len(posts)):
		stream[posts[a][0]] = stream.get(posts[a][0], 0)+1
	for key, value in stream.items():
		setRead(key, user, value)
		#print(key +" : "+ str(value))
	#print(stream)

if __name__ == "__main__":
	if (len(sys.argv) <= 1):
		exit(0)

	if (sys.argv[1] == "stream")
"""
	name = ""
	for x in sys.argv[1:]:
		name+=x+" "
	name = name[:-1]

	csr = curses.initscr()

	try:
		curses.start_color()
		size = list(csr.getmaxyx())
		if (size[0] > 24):
			size[0] = 24

		#stream = changeStream(csr, name)
		flag = 1
		index = 0
		index, posts, curStream = list(changeStream(csr,name))
		numPostsOnScreen, lineStart = displayScreen(csr, posts, size, index, name, flag, 0)
		while (1==1):
			c = csr.getch(0,0)
			csr.clear()
			if (c == ord('q')):
				break
			elif (c == ord('o')):
				flag = flag*-1
				if (flag == 1):
					posts = sortPosts(posts)
				else:
					posts = sortPostsAuthor(posts)

			elif (c == ord('m')):
				markAllRead(posts, name)
			elif (c == ord('c')):
				index, posts = refreshStream(csr, name, curStream)
				if (flag == -1):
					posts = sortPostsAuthor(posts)
			elif (c == ord('s')):
				#stream = changeStream(csr, name)
				#posts = list(loadStream(stream, name))
				index, posts, curStream = (list(changeStream(csr,name)))
				if (flag == -1):
					posts = sortPostsAuthor(posts)

			elif (c == 65): #up
				if (index > 0):
					if (index >= len(posts)):
						index -= 1
					tmp, lineStart = pageUpShift(posts, index, csr, lineStart)
					index-=tmp
				else:
					lineStart = 0
			elif (c == 66): #down
				if (index+numPostsOnScreen < len(posts)):
					index += numPostsOnScreen
			#refresh page
			numPostsOnScreen, lineStart = displayScreen(csr, posts, size, index, name, flag, lineStart)
	except Exception:
		curses.endwin()
		traceback.print_exc()
		exit(0)
	curses.endwin()

"""