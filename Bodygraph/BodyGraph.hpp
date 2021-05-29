#pragma once

#include "BodyGraphDefs.hpp"
//#include "BodyPart.hpp"

#include <map>
#include <vector>
/*
A map of a vertex index and the edges that vertex is connected to. This is used to help with graph traversals for calculating combat related things. The index here is an index into the vertex of the Anatomygraph, where the element at each index is a Bodypart. 
Maybe later down the line I will replace index references with something else, but for now I'm not worrried with that detail.

Been a while, so I can't remember exactly why I used edges instead of vertices here. I think that may have been due to how I traversed the graph. The way the graph iterators
worked made using edges easier. Not sure if that's the reason, that is mostly just me thinking back after commenting the code months after writing it. 

*/
typedef std::map<int, std::vector<AnatomyEdge>> EdgeMap;

class BodyPart;

/*
The AnatomyGraph data field uses the boost graph library. Each vertex in the graph is a body part, and the edges between the vertices is a connection.
I.E, a lung and a chest are vertices. A lung can be reached through the chest, so they're connected by edges in the anatomyGraph.
*/
class CreatureBody
{
public:
  
	CreatureBody();
	
	AnatomyGraph anatomyGraph;//Make this private after testing todo
	EdgeMap edgeConnections; 

	AnatomyGraph& getAnatomyGraphRef();
	BodyPart *getBodyPartPointer(int index);
	BodyPart& getBodyPartRef(int index);
	
	void setBodyGraph(AnatomyGraph graph);
	int getNumVertices();
	void prepareEdgeMap();
};






















