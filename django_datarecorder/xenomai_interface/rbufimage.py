'''
Created on 12.02.2013

@author: ihl
'''
import cv, random
mat = cv.CreateMat(600,600, cv.CV_8UC1)

i = 0
blub = True
for x in xrange(mat.cols):
    for y in xrange(mat.rows):
        mat[y,x] = random.randint(0,255)
        #if blub:
        #    mat[y,x] = False
        #    blub = False
        #else:
        #    mat[y,x] = 0
        #   blub = True
        #if i+1>255:
        #    i = 0
        #i = i+1
        #if i == 0:
        #    i = 255
        #else:
        #    i = 0
        #mat[y,x] = i 

cv.SaveImage("test3.png", mat)
