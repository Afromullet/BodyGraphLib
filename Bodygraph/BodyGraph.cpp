#include "BodyGraph.hpp"

/*
The default constructor for a body. 

Todo, this will need some values that determine that it can't be used, or remove the constructor and replace it with a constructor that adds a file path as a parameter.
Also, prepareEdgeMap should probably be called during construction


*/
CreatureBody::CreatureBody()
{
    
}

/*
Gets a reference for the graph. There's a lot of data in the graph, so we don't want to return a copy
*/
AnatomyGraph & CreatureBody::getAnatomyGraphRef()
{
	return anatomyGraph;
}

BodyPart * CreatureBody::getBodyPartPointer(int index)
{
	if (index >= getNumVertices())
	{
		return nullptr;
	}
	return &anatomyGraph[index];
}

BodyPart & CreatureBody::getBodyPartRef(int index)
{
	if (index >= getNumVertices())
	{
		//TODO think of how to handle error condition
	}
	return anatomyGraph[index];
}

//Todo check if you need to override the assignment operator. I think this should be alright as long as the Vertex property has the right assignment operator. 
void CreatureBody::setBodyGraph(AnatomyGraph graph)
{
	anatomyGraph = graph;
	
}
/*
Gets the number of vertices in the anatomyGraph. This function is here to keep anatomyGraph hidden, since we'll need to know the number of vertices outside of this class
*/
int CreatureBody::getNumVertices()
{
	return num_vertices(anatomyGraph);
}

/*
We need to be able to find paths of depth or breadth n. That's much easier to do when we have a quick way to access all the vertices something is connected to without 
having to search the graph. prepareEdgeMap constructs a hash table with the key being the vertex index and the values being a list of AnatomyEdges the vertex is connected to. 

*/
void CreatureBody::prepareEdgeMap()
{
	graph_traits<AnatomyGraph>::edge_iterator ei, ei_end;

	std::vector<AnatomyEdge> tempEdges;
	EdgeMap::iterator mapIt;
	//todo add error checking
	
	for (int i = 0; i < num_vertices(anatomyGraph); i++)
	{
		edgeConnections.insert(std::make_pair(i, std::vector<AnatomyEdge>()));
	}

	for (std::tie(ei, ei_end) = edges(anatomyGraph); ei != ei_end; ++ei)
	{
		
		mapIt = edgeConnections.find(source(*ei, anatomyGraph));
		mapIt->second.push_back(*ei);
	}
}





