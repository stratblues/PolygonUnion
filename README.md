@main.cpp
@author Tony Stanell 
@date May 24, 2024
@section CS536


The Program: 

The program is fully housed in main.cpp for simplicity sake.

This is a c++ file for solving the problem of computing the union of two 2D convex polygons.

The program reads in two polygons from a Postscript file, as specified by the -f filename argument.
An example file will look like:
%%%BEGIN
200 100 moveto
300 100 lineto
300 400 lineto
200 400 lineto
200 100 lineto
stroke
250 150 moveto
350 150 lineto
350 450 lineto
250 450 lineto
250 150 lineto
stroke
%%%END
The "moveto" keyword starts a polygon and the "stroke" keyword specifies the end of a polygon.
The two polygons will be bracketted by the "%%%BEGIN" and "%%%END" keywords and contain any number (at least three) of vertices.
Default filename value: HW3_a_in.ps

Writes the resulting union'ed polygon in Postscript format to standard out.
Program can be run with command "./CG_hw5 -f filename > out.ps


Overview:
This program was built using C++ for Linux. The development environment I used was Visual Studio 2022 with a remote SSH connection to Drexel's Tux servers for compilation. I used the standard C++ compiler associated with visual studio. 


Features:
I built this program utilized a linked list data structure, as well as classes for both Point and Edge. The linked list data structure was also implemented as its own class with methods to insert Points into the structure as well as retrieve points, convert the structure to a vector, convert the points in the structure to edges, retireve the size, and print the list. The Point class holds x and y points as well as a key to the next point which is utilized within the linked list. Edge contains start and end points as well as a link to the next edge, which is also utilized within the linked list.

Once inside my program the main functions are isPointInside(), getINtersectionPoint(), and computeUnionPolygon(). 

isPointInside() takes in a point and polygon linked list. I get the edges of said polygon and loop through them while testing for intersection points with a new "testEdge" ( contains the point read into function as well as point prime which pushes the X value "off the charts" so to speak). If I find an intersection I make sure that if it's also a vertex (t value 0 or 1) that I increment by .5 instead of 1 because a vertex would cause it to think there are two intersection instead of 1. If even or 0 I return false otherwise the function evaluates to true.

getIntersectionPoint() takes in two edges and calculates the math required to compute if there is an interestion point and where that intersection point occurs. This function outputs two values, the intersection point as well as the corresponding t value. If no intersection point returns an arbirary point and -1 t value.

computeUnionPolygon() takes it the Linkedlist points for two polygons as well as the output list for the new unionized polygon. First I copy the Linkedlist structures and then find the starting point which is a point outside of polygonB. Once I have that point I start to populate the output list and form my new union polygon. I set the nextVertex to either the startpoint+1 or P0's head and then I set the initial currentEdge. The while loop marches around the edges and vertecies while checking for interesction points and adding any to the output list. At each intersection the loop switches to the other polygon and continues its march around until it ends up back at the beginning with my completed new polygon set to output.
