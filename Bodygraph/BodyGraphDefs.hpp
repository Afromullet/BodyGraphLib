#pragma once


#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/adjacency_iterator.hpp> guess I don't really need this. Leaving it here just in case
#include "BodyPart.hpp"


using namespace boost;

/*
Internal edge means that it is "inside" of the body part. Don't want to have a graph with both directed and non directed edges. I have a feeling that would be painful to do inside of boost 
The second of the values in the ordered pair is the part that is internal (or external? Can't remember anymore, been a while since I started this and just now refactoring). (Chest,Lung) with enInternal edge property means that the lung is internal. 
TODO check if boost graph always adds edges in the order that the function is called , add_edge(a,b) creating (a,b) and never (b,a). The above statement is only true if that's correct
*/
namespace BodyGraphEnums
{

	//The connection lets us know whether a body part is internal or external
	enum EnConnectionType {
		enSymmetric,
		enDirect,
		enLeftConnection,
		enRightConnection,
		enInvalidConnection,
		enInternal,
		enInternalLeft,
		enInternalRight,
		enExternalLeft,
		enExternalRight,
		enExternal,
		enExternalLeftFront,
		enExternalRightFront
	};

	//This currently isn't being used but is here for future growth. 
	enum EnConnectionProperty
	{
		enBone,
		enJoint,
		enConnectiveTissue
		, enInvalidEdgeConnection
	};
}


/*
Todo, I don't think this is needed anymore. I used this for some graph algorithm in the past which I don't think I use anymore. Will figure out whether I still need it during refactoring
*/
typedef boost::property < boost::edge_weight_t, int> EdgeWeightProp;
typedef struct _GraphConnection
{
	BodyGraphEnums::EnConnectionType connection;
	std::string connectionProperty;
	int weight; //Weight defaults to 1. 
	
	//EdgeWeightProp weightProperty; TODO FIGURE OUT WHY YOU USED THIS
}GraphConnection;

/*
Bodypart is the Vertex Property, meaning that every vertex is an instance of the Bodypart class. 
*/ 
typedef boost::adjacency_list<vecS, vecS, directedS, BodyPart, GraphConnection, EdgeWeightProp> AnatomyGraph; 

typedef boost::graph_traits<AnatomyGraph>::vertex_descriptor AnatomyVertex;
typedef boost::graph_traits<AnatomyGraph>::edge_descriptor AnatomyEdge;

typedef boost::graph_traits <AnatomyGraph>::in_edge_iterator AnatomyInEdgeIt;
typedef boost::graph_traits <AnatomyGraph>::edge_iterator AnatomyEdgeIt;
typedef boost::graph_traits<AnatomyGraph>::vertex_iterator AnatomyVertexIt;
typedef boost::graph_traits<AnatomyGraph>::adjacency_iterator AnatomyAdjacencyIt;
typedef boost::property_map<AnatomyGraph, vertex_index_t>::type AnatomyIndexMap;
