# CG_hw5 - Union of Convex Polygons

## Author
Tony Stanell

## Date
May 24, 2024

## Course Section
CS536

## Program Overview

This program, fully housed in `main.cpp` for simplicity, is designed to compute the union of two 2D convex polygons. It reads two polygons from a Postscript file specified by the `-f filename` argument and writes the resulting union'ed polygon in Postscript format to standard out.

### Sample Input File

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


- `moveto` starts a polygon.
- `lineto` adds a vertex.
- `stroke` ends a polygon.
- Polygons are bracketed by `%%%BEGIN` and `%%%END` keywords.

### Default Filename

HW3_a_in.ps


### Running the Program

./CG_hw5 -f filename > out.ps



## Development Environment

The program was developed using C++ on Linux. I used Visual Studio 2022 with a remote SSH connection to Drexel's Tux servers for compilation, utilizing the standard C++ compiler.

## Features

- **Data Structures:** Uses a custom linked list for storing points and edges.
- **Classes:**
  - `Point`: Manages coordinates (x, y) and links to the next point.
  - `Edge`: Represents edges between points and links to the next edge.
- **Core Functions:**
  - `isPointInside(Point, Polygon)`: Determines if a point is inside a polygon.
  - `getIntersectionPoint(Edge, Edge)`: Computes the intersection point between two edges.
  - `computeUnionPolygon(LinkedList, LinkedList)`: Constructs the union of two polygons.

## Description of Key Functions

- `isPointInside()`: Checks if a point lies within a polygon using edge intersections.
- `getIntersectionPoint()`: Calculates if and where two edges intersect, returning the intersection point and a t value (parameter).
- `computeUnionPolygon()`: Builds the union polygon from two input polygon lists by tracking intersections and switching between polygons.

## Dependencies

Ensure you have a C++ compiler like g++, configured for your environment.
