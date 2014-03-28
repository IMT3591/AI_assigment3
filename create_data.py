#!/usr/bin/python
# -*- coding: utf-8 -*-

import Image
import string

value = 0
char = "A"
arr  = []
count = 1;
data = open("training.dta", "w")
#data = open("data.dta", "w")
for char in string.ascii_uppercase:
	for count in [1,3,5,7,9,11,13,15,17,19]:
	#for count in [2,4,6,8,10,12,14,16,18,20]:
	#for count in range(1, 21):
		imgArr= []
		iName = char + str(count)  + ".jpg"
		im = Image.open("pics/" + iName)
		imOrg = im.load()
		imNew = Image.new("L", (10,10))
		imNewP = imNew.load()
		for x in range(0,10):
			for y in range(0, 10):
				arr = None
				arr = [
					#First row
					imOrg[x*3 , 	y*3],		imOrg[x*3+1 , y*3],		imOrg[x*3+2 , y*3],
					#Second row
					imOrg[x*3 , 	y*3+1],	imOrg[x*3+1 , y*3+1],	imOrg[x*3+2 , y*3+1],
					#Third row
					imOrg[x*3 , 	y*3+2],	imOrg[x*3+1 , y*3+2],	imOrg[x*3+2 , y*3+2]
				]
				imNewP[x, y] = sum(arr)/len(arr)
				imgArr.append( sum(arr)/float(len(arr)) )
		imNew.save("con/" + iName)
		for i in imgArr:
			data.write( str( float(i) ) + " " )
		data.write( "\n" )
