#include "graph.h"
#include "vertex.h"
#include "queue.h"
#include <vector>

using namespace std;
using namespace custom;

/**********************************************
* GRAPH :: FIND PATH
* Find the set of verticies representing a path
* from vFrom to vTo in the graph
***********************************************/
vector <Vertex> Graph :: findPath(const Vertex & vFrom, const Vertex & vTo)
{
// create an array representing the shortest path to each item. Mark
// the distance to vFrom as zero
int distance = 0;
int * distances = new int[numNode];
Vertex * predecessor = new Vertex[numNode];
for (int i = 0; i < numNode; i++)
distances[i] = -1;
distances[vFrom.index()] = distance;

// create a queue of the ones to visit. Start with vFrom
queue <Vertex> toVisit;
toVisit.push(vFrom);

// while we have not found the destination and the list of places
// we still have to go is not empty
while (!toVisit.empty() && distances[vTo.index()] == -1)
{
// grab the next item off the head of the queue
Vertex v = toVisit.front();
toVisit.pop();

// increment the distance as necessary
if (distances[v.index()] > distance)
distance++;

// go through all the edges and add them as necessary
set <Vertex> s(findEdges(v));
set <Vertex> :: const_iterator it;
for (it = s.cbegin(); it != s.cend(); ++it)
if (distances[(*it).index()] == -1)
{
distances[(*it).index()] = distance + 1;
predecessor[(*it).index()] = v;
toVisit.push((*it));
}
}
distance++;

// now reconstruct the shortest path
vector <Vertex> path;
if (distances[vTo.index()] == -1)
{
std::cout << "No path found\n";
return path;
}

path.push_back(vTo);
for (int i = 1; i <= distance; i++)
path.push_back(predecessor[path[i - 1].index()]);

delete [] distances;
delete [] predecessor;
return path;
}