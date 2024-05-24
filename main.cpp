#include <string.h>
#include <iostream> 
#include <iomanip>
#include <fstream>
#include <eigen3/Eigen/Dense>
#include <vector>
#include <cmath>
#include <math.h>
#include <ostream>
/*
* @file
* @author Tony Stanell <ts3552@drexel.edu>
* @date May 13th, 2024
* @section CS536
*
* This is a c++ file for solving the problem of computing the union of two 2D convex polygons.
*/

using namespace std;
using namespace Eigen;



class Point {
public:
    double x;
    double y;
    Point* next;

    Point() : x(0), y(0), next(nullptr) {}

    Point(double x, double y) {
        this->x = x;
        this->y = y;
        this->next = nullptr;
    }
};

class Edge {
public:
    Point start;
    Point end;
    Edge* next;

    Edge(Point start, Point end) {
        this->start = start;
        this->end = end;
        this->next = nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
        os << "Edge from (" << edge.start.x << ", " << edge.start.y << ") to (" << edge.end.x << ", " << edge.end.y << ")";
        return os;
    }

   
};


class LinkedList {
private:
    Point* head;

public:
    LinkedList() {
        this->head = nullptr;
    }

    void insertAtBeggining(double x, double y) {
        Point* newPoint = new Point(x, y);
        newPoint->next = head;
        head = newPoint;
        return;
    }

    void insertAtEnd(double x, double y) {
        Point* newPoint = new Point(x, y);
        if (head == nullptr) {
            head = newPoint;
            return;
        }
        Point* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newPoint;
    }

    void printList() {
        Point* temp = head;
        cout << temp->x << " " << temp->y << " moveto" << endl;
        temp = temp->next;
        while (temp != nullptr) {
            cout << temp->x << " " << temp->y << " lineto" << endl;  
            temp = temp->next;
        }
        
       
    }

    Point* getHead() {
        return head;
    }

    Point* getTail() {
        if (head == nullptr) {
            return nullptr;
        }
        Point* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        return temp;
    }

    vector<Point> toVector() {
        vector<Point> points;
        Point* temp = head;
        while (temp != nullptr) {
            points.push_back(*temp);
            temp = temp->next;
        }
        return points;
    }

    void insertPoint(Point* point) {
        insertAtEnd(point->x, point->y);
    }

    vector<Edge> getEdges() {
        vector<Edge> edges;
        if (!head) return edges;
        Point* current = head;
        Point* start = head;

        while (current->next != nullptr) {
            Edge newEdge(*current, *(current->next));
            edges.push_back(newEdge);
            current = current->next;
        }
        Edge closingEdge(*current, *start);
        edges.push_back(closingEdge);

        return edges;
    }

    int size() {
        int count = 0;
        Point* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }
};

void evaluateCommandLine(int argc, char* argv[], char** inFileName);
void readFile(char** filename, LinkedList& PointListA, LinkedList& PointListB);
void printVertices(LinkedList& pointList, const string& polygonName);
pair<Point, double> getIntersectionPoint(Edge edgeOne, Edge edgeTwo);
pair<int, double> intersectionTestInsideOut(Edge edgeOne, Edge edgeTwo);
bool isPointInside(const Point& point, LinkedList& polygon);
LinkedList computeUnionPolygon(LinkedList& polygonA, LinkedList& polygonB, LinkedList& output);
void printToPostScriptFormat(LinkedList& output);




int main(int argc, char* argv[])
{

    char* inFileName = "HW3_a_in.ps";
    LinkedList PointListA;
    LinkedList PointListB;
    LinkedList EdgeIntersections;
    LinkedList output;
    LinkedList unionResult;


 


    evaluateCommandLine(argc, argv, &inFileName);
    readFile(&inFileName, PointListA, PointListB);

  

    unionResult = computeUnionPolygon(PointListA, PointListB, output);
    printToPostScriptFormat(unionResult);

   

}

void evaluateCommandLine(int argc, char* argv[], char** inFileName) {
    int i = 0;
    while (i < argc) {
        if (string(argv[i]) == "-f" && i + 1 < argc) {
            *inFileName = argv[i + 1];
            i += 2;
        }
        else {
            i++;
        }
    }
}

void readFile(char** filename, LinkedList& PointListA, LinkedList& PointListB) {
    string fileText;
    ifstream MyFile(*filename);
    int count = 0;
    Vector2d point(0.0, 0.0);
    if (!MyFile.is_open()) {
        cout << "Failed to open file" << endl;
        return;
    }
    string command;
    while (getline(MyFile, fileText)) {
        istringstream iss(fileText);
        if (fileText == "stroke") {
            count += 1;
        }
        if (iss >> point[0] >> point[1] >> command) {
            if (count < 1) {
                PointListA.insertAtEnd(point[0], point[1]);

            }
            else {
                PointListB.insertAtEnd(point[0], point[1]);

            }
        }
    }

    MyFile.close();
}

bool isPointInside(Point& point, LinkedList& polygon) {
    vector<Edge> polygonEdges = polygon.getEdges();
    int n = polygonEdges.size();
   
    Point pPrime(point.x * 1000, point.y);
    Edge testEdge(point, pPrime);
    int count = 0;
    for (int i = 0; i < n; i++) {
        auto [intersects,t] = intersectionTestInsideOut(testEdge, polygonEdges[i]);
        if (intersects) {
            if (t == 0 || t == 1) {
                count += 0.5;
            }
            else {
                count += intersects;
            }
        }
    }
    if (count % 2 == 0 || count == 0) {
        return false;

    }
    else {
        return true;
    }


}

pair<Point, double> getIntersectionPoint(Edge edgeOne, Edge edgeTwo) {
    double deltax0 = edgeOne.end.x - edgeOne.start.x;
    double deltay0 = edgeOne.end.y - edgeOne.start.y;
    double deltax2 = edgeTwo.end.x - edgeTwo.start.x;
    double deltay2 = edgeTwo.end.y - edgeTwo.start.y;

    double denominator = (deltay0 * deltax2 - deltax0 * deltay2);

    if (denominator == 0) {
      
        return { Point(1e+38, 1e+38), -1 };
    }
    double t0 = ((edgeOne.start.x - edgeTwo.start.x) * deltay2 + (edgeTwo.start.y - edgeOne.start.y) * deltax2) / denominator;
    double t2 = -1.00*(((edgeTwo.start.x - edgeOne.start.x) * deltay0 + (edgeOne.start.y - edgeTwo.start.y) * deltax0) / denominator);

    if (t0 >= 0 && t0 <= 1 && t2 >= 0 && t2 <= 1) {
        double intersectionX = edgeOne.start.x + t0 * deltax0;
        double intersectionY = edgeOne.start.y + t0 * deltay0;
      
        return { Point(intersectionX, intersectionY), t0 };
    }

    return { Point(1e+38, 1e+38), -1 };
}

pair<int, double> intersectionTestInsideOut(Edge edgeOne, Edge edgeTwo) {
    double deltax0 = edgeOne.end.x - edgeOne.start.x;
    double deltay0 = edgeOne.end.y - edgeOne.start.y;
    double deltax2 = edgeTwo.end.x - edgeTwo.start.x;
    double deltay2 = edgeTwo.end.y - edgeTwo.start.y;

    double denominator = (deltay0 * deltax2 - deltax0 * deltay2);
    if (denominator == 0) {
        return { 0, -1.00 };
    }
    double t0 = ((edgeOne.start.x - edgeTwo.start.x) * deltay2 + (edgeTwo.start.y - edgeOne.start.y) * deltax2) / denominator;
    double t2 = -1.00 * (((edgeTwo.start.x - edgeOne.start.x) * deltay0 + (edgeOne.start.y - edgeTwo.start.y) * deltax0) / denominator);
    if (t0 >= 0 && t0 <= 1 && t2 >= 0 && t2 <= 1) {
       
        return { 1, t0 };
    }
    return { 0, -1.00 };
}

LinkedList computeUnionPolygon(LinkedList& polygonA, LinkedList& polygonB, LinkedList& output) {
    LinkedList* P0 = &polygonA;  
    LinkedList* P1 = &polygonB;  
    Point* vi = nullptr;

    
    for (Point* current = P0->getHead(); current != nullptr; current = current->next) {
        if (!isPointInside(*current, *P1)) {
            vi = current;
            break;
        }
    }


   
    output.insertAtEnd(vi->x, vi->y);

    Point* nextVertex = vi->next ? vi->next : P0->getHead();
    Edge currentEdge(*vi, *nextVertex);
  

    while ((output.size() < 2) || (output.getHead()->x != output.getTail()->x || output.getHead()->y != output.getTail()->y)) {
        double lowestT = 1.0;
        Point intersectionPoint;
        bool intersectionFound = false;
        Point* lastVertInP1IntersectedEdge = nullptr;
        for (Edge& edge : P1->getEdges()) {
         
            auto result = getIntersectionPoint(currentEdge, edge);
            double t = result.second;
            if (t >= 0 && t < lowestT) {
                intersectionPoint = result.first;
                lowestT = t;
                lastVertInP1IntersectedEdge = &edge.end;
                intersectionFound = true;
            }
        }

        if (intersectionFound) {
           
    
            output.insertAtEnd(intersectionPoint.x, intersectionPoint.y);
            output.insertAtEnd(lastVertInP1IntersectedEdge->x, lastVertInP1IntersectedEdge->y);
            vi = new Point(intersectionPoint.x, intersectionPoint.y);
            currentEdge = Edge(*lastVertInP1IntersectedEdge, *(lastVertInP1IntersectedEdge->next ? lastVertInP1IntersectedEdge->next : P1->getHead()));
           
           
            LinkedList* temp = P1;
            P1 = P0;
            P0 = temp;

        }
        else {
            output.insertAtEnd(currentEdge.end.x,currentEdge.end.y);
            vi = &currentEdge.end;
            Point* next = currentEdge.end.next ? currentEdge.end.next : P0->getHead();
            currentEdge = Edge(*vi, *next);
            
           
        }
       
        
    }
    return output;
}


void printVertices(LinkedList& pointList, const string& polygonName) {
    cout << polygonName << " vertices:" << endl;
    pointList.printList();
    
}

void printToPostScriptFormat(LinkedList& output) {
    cout << "%%%BEGIN" << endl;
    output.printList();
    cout << "stroke" << endl;
    cout << "%%%END" << endl;

}