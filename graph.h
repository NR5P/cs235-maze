#ifndef GRAPH_H
#define GRPAH_H

#include "vertex.h"
#include "bst.h"
#include "set.h"
#include "list.h"

class Graph {
    private:    
        int numVertices;

    public:
        // non default constructor
        Graph(int numVertices) : numVertices(numVertices){}
        
        // assignment 
        Graph &operator=(const Graph &rhs);

        bool empty();
        int size();
        int capacity();
        void clear();
        void add(Vertex v1, Vertex v2);
        bool isEdge(Vertex v1, Vertex v2);
        custom::set<Vertex> findEdges(Vertex v);
        custom::list<Vertex> findPath(Vertex v1, Vertex v2);



};

#endif