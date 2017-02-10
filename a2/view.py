#!/usr/bin/python3
import sys

if (len(sys.argv) <= 1):
	print("No arguments specified")
	exit(0)

name = ""
for x in sys.argv[1:]:
	name+=x+" "
name = name[:len(name)-1]
print("'"+name+"'")