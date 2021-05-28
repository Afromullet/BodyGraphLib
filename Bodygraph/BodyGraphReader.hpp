#pragma once




#include <stdio.h>
#include <vector>
#include <list>



#include <boost/regex.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/range/irange.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/foreach.hpp"

#include <string>
#include <memory>

#include "BodyGraphDefs.hpp"
#include "BodyPart.hpp"
#include "WoundHandler.hpp"

using namespace boost;
namespace pt = boost::property_tree;

class BodyGraphReader
{

public:
	BodyGraphReader(const std::string &fileName);

	AnatomyGraph anatomyGraph;

	void read_wound_properties(const std::string &filename);
	
private:
	pt::ptree tree;
	std::string m_file;
	std::string bodyName;

	std::vector<std::string> bodyTokenList;

	void readBodyTokens();
	void GenerateVertices();
	void GenerateEdges();
	void GenerateOrganVertices();
	void GenerateOrganEdges();

	




};


bool convertTruthStringValue(std::string truthVal);

//Returns the vertex index
int GetVerticesWithBPToken(std::string bptoken, AnatomyGraph &graph);

BodyGraphEnums::EnConnectionType convertStringToConnectionType(std::string conType);
BodyGraphEnums::EnConnectionProperty convertStringToConnectionProperty(std::string conType);




