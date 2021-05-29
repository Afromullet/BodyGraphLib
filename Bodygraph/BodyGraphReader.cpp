#include "BodyGraphReader.hpp"



/*
The constructor takes the fileName for a bodyGraph definition and calls the functions to process it
*/
BodyGraphReader::BodyGraphReader(const std::string &fileName)
{

	pt::read_xml(fileName, tree);
	readBodyTokens();
	GenerateVertices();
	GenerateEdges();
	GenerateOrganVertices();
	GenerateOrganEdges();
}



/* 
Reads all the tokens for the bodyPart and stores them in a list. 
Each token identifies a bodypart in the file
*/
void BodyGraphReader::readBodyTokens()
{

	BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("bptokenlist"))
	{
		// The data function is used to access the data stored in a node.
		bodyTokenList.push_back(v.second.data());
	}

}

/*
Builds all the non-organ vertices, one for each bodytoken. Does not generate the edges
*/
void BodyGraphReader::GenerateVertices()
{
	//Declaring the variables to make this more readable
	std::string bptoken, bpname, section;
	int holdsWeapon, holdsArmor, canInteract, canSee, canSmell, canBreathe, canMove, isOrgan;
	float relativeSize;
	BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("bodyparts"))
	{
		if (v.first == "bpdescription")
		{
			bptoken = v.second.get<std::string>("bptoken");



			bpname = v.second.get<std::string>("bodypartname");
			section = v.second.get<std::string>("section");

			

			try
			{
				holdsArmor = convertTruthStringValue(v.second.get<std::string>("canholdarmor"));
			}
			catch (pt::ptree_bad_path)
			{
				holdsArmor = false;
			}


			try
			{
				holdsWeapon = convertTruthStringValue(v.second.get<std::string>("canholdweapon"));
			}
			catch (pt::ptree_bad_path)
			{
				holdsWeapon = false;
			}


			try
			{
				canSee = convertTruthStringValue(v.second.get<std::string>("cansee"));
			}
			catch (pt::ptree_bad_path)
			{
				canSee = false;
			}

			try
			{
				canSmell = convertTruthStringValue(v.second.get<std::string>("cansmell"));
			}
			catch (pt::ptree_bad_path)
			{
				canSmell = false;
			}

			try
			{
				canBreathe = convertTruthStringValue(v.second.get<std::string>("canbreathe"));
			}
			catch (pt::ptree_bad_path)
			{
				canBreathe = false;
			}

			try
			{
				canSmell = convertTruthStringValue(v.second.get<std::string>("cansmell"));
			}
			catch (pt::ptree_bad_path)
			{
				canSmell = false;
			}


			try
			{
				relativeSize = std::stof(v.second.get<std::string>("relativesize"));
			}
			catch (pt::ptree_bad_path)
			{
				relativeSize = 10;
			}

			try
			{
				canInteract = convertTruthStringValue(v.second.get<std::string>("caninteract"));
			}
			catch (pt::ptree_bad_path)
			{
				canInteract = false;
			}

			try
			{
				canMove = convertTruthStringValue(v.second.get<std::string>("canmovecreature"));
			}
			catch (pt::ptree_bad_path)
			{
				canMove = false;
			}

			try
			{
				isOrgan = convertTruthStringValue(v.second.get<std::string>("isorgan"));
			}
			catch (pt::ptree_bad_path)
			{
				isOrgan = false;
			}

			BodyPart bp(bptoken, bpname);
			bp.setRelativeSize(relativeSize);

			bp.setCanSmell(canSmell);
			bp.setCanSee(canSee);
			bp.setSection(section);

			bp.setCanBreathe(canBreathe);

			bp.setCanMoveCreature(canMove);
			bp.setIsOrgan(isOrgan);
			bp.setBodyPartName(bpname);


			boost::add_vertex(bp, anatomyGraph);

		}

	}

}

/*
Generates the edge connections for all non-organ vertices
*/
void BodyGraphReader::GenerateEdges()
{
	//BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("main.cats"))


	int bp1Index, bp2Index;
	GraphConnection conType;
	AnatomyIndexMap indMap = get(vertex_index, anatomyGraph); //Getting a proeprty map.
	BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("bodypartconnections"))
	{
		if (v.first == "bodypart")
		{

			bp1Index = GetVerticesWithBPToken(v.second.get<std::string>("bptoken"), anatomyGraph);
			bp2Index = GetVerticesWithBPToken(v.second.get<std::string>("connectsto"), anatomyGraph);

			//conType = convertConnectionType(v.second.get<std::string>("connectiontype"));
			conType.connection = convertStringToConnectionType(v.second.get<std::string>("connectiontype"));
			conType.connectionProperty = convertStringToConnectionProperty(v.second.get<std::string>("connectionproperty"));
		
			conType.weight = 1;
			boost::add_edge(indMap[bp1Index], indMap[bp2Index], conType, anatomyGraph);
		}
		// The data function is used to access the data stored in a node.

	}


}

/*
Generates the verticles for the organs
*/
void BodyGraphReader::GenerateOrganVertices()
{
	//BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("main.cats"))

	//Declaring the variables to make this more readable
	std::string bptoken, bpname, section;
	int holdsWeapon, holdsArmor, canInteract, canSee, canSmell, canBreathe, canMove, isOrgan;
	float relativeSize;
	BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("organs"))
	{
		if (v.first == "bpdescription")
		{
			bptoken = v.second.get<std::string>("bptoken");

			section = v.second.get<std::string>("section");
			bpname = v.second.get<std::string>("bodypartname");

			//These exceptions are here because the user doesn't have to define these for a body part. Defining a body part will be much less painful if some less important parts can be left out

			try
			{
				holdsArmor = convertTruthStringValue(v.second.get<std::string>("canholdarmor"));
			}
			catch (pt::ptree_bad_path)
			{
				holdsArmor = false;
			}

			try
			{
				holdsWeapon = convertTruthStringValue(v.second.get<std::string>("canholdweapon"));
			}
			catch (pt::ptree_bad_path)
			{
				holdsWeapon = false;
			}


			try
			{
				canSee = convertTruthStringValue(v.second.get<std::string>("cansee"));
			}
			catch (pt::ptree_bad_path)
			{
				canSee = false;
			}

			try
			{
				canSmell = convertTruthStringValue(v.second.get<std::string>("cansmell"));
			}
			catch (pt::ptree_bad_path)
			{
				canSmell = false;
			}

			try
			{
				canBreathe = convertTruthStringValue(v.second.get<std::string>("canbreathe"));
			}
			catch (pt::ptree_bad_path)
			{
				canBreathe = false;
			}

			try
			{
				canSmell = convertTruthStringValue(v.second.get<std::string>("cansmell"));
			}
			catch (pt::ptree_bad_path)
			{
				canSmell = false;
			}


			try
			{
				relativeSize = std::stof(v.second.get<std::string>("relativesize"));
			}
			catch (pt::ptree_bad_path)
			{
				relativeSize = 10;
			}

			try
			{
				canInteract = convertTruthStringValue(v.second.get<std::string>("caninteract"));
			}
			catch (pt::ptree_bad_path)
			{
				canInteract = false;
			}

			try
			{
				canMove = convertTruthStringValue(v.second.get<std::string>("canmovecreature"));
			}
			catch (pt::ptree_bad_path)
			{
				canMove = false;
			}

			try
			{
				isOrgan = convertTruthStringValue(v.second.get<std::string>("isorgan"));
			}
			catch (pt::ptree_bad_path)
			{
				isOrgan = false;
			}


			BodyPart bp(bptoken, bpname);
			bp.setRelativeSize(relativeSize);
			bp.setCanSmell(canSmell);
			bp.setCanSee(canSee);
			bp.setSection(section);
			//   bp.setArmor(NO_ARMOR);
			bp.setCanBreathe(canBreathe);
			bp.setCanMoveCreature(canMove);
			bp.setIsOrgan(isOrgan);
			boost::add_vertex(bp, anatomyGraph);

		}

	}

}

/*
Generates the edges for the organ vertices
*/
void BodyGraphReader::GenerateOrganEdges()
{
	//BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("main.cats"))


	int bp1Index, bp2Index;
	GraphConnection conType;
	AnatomyIndexMap indMap = get(vertex_index, anatomyGraph); //Getting a proeprty map.
	BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("organsconnections"))
	{
		if (v.first == "organconnection")
		{


			bp1Index = GetVerticesWithBPToken(v.second.get<std::string>("bptoken"), anatomyGraph);
			bp2Index = GetVerticesWithBPToken(v.second.get<std::string>("connectsto"), anatomyGraph);



			conType.connection = convertStringToConnectionType(v.second.get<std::string>("connectiontype"));

			boost::add_edge(indMap[bp1Index], indMap[bp2Index], conType, anatomyGraph);








		}
		// The data function is used to access the data stored in a node.

	}

}

/*
Intended to be called after the Bodygraph has already been constructed. Sure, that's inefficient because we're going to have to search the entire grapha gain after it is already built.
but since we only have to iterate over it one additional time, the tradeoff is worth not having to mess with the existing code...for now.

*/
void BodyGraphReader::read_wound_properties(const std::string &filename)
{

	WoundTypes::WoundPossibleMap woundsPossible = WoundTypes::process_wound_characteristics_file(filename);
	WoundTypes::WoundPossibleMap::iterator woundIt;
	for (int i = 0; i < num_vertices(anatomyGraph); i++)
	{

		woundIt = woundsPossible.find(anatomyGraph[i].getBodyPartToken());

		if (woundIt != woundsPossible.end())
		{
			//anatomyGraph[i].setWoundProperties(woundIt->second);

		}



	}




}





/*
Can't use a boolean because -1 indicates that the truth value being passed is not the correct format. Helps with error checking
Certain properties of the body in the bodygraph definition file are set to true or false. This function is used to convert it to something we can work with
*/
bool convertTruthStringValue(std::string truthVal)
{
	std::transform(truthVal.begin(), truthVal.end(), truthVal.begin(), ::tolower);

	if (truthVal == "false")
	{

		return false;
	}
	else if (truthVal == "true")
	{

		return true;
	}


	return -1; //Don't think does anything, since we'll be checking for booleans of the return type. Todo think of a better way to handle invalid values. Maybe an exception?
}


/*
The bodygraph definition file contains the type of edge connections for a bodypart.
This function converts the string to the enumerated type representing the connection
*/
BodyGraphEnums::EnConnectionType convertStringToConnectionType(std::string conType)
{
	std::transform(conType.begin(), conType.end(), conType.begin(), ::tolower);
	BodyGraphEnums::EnConnectionType con;

	if (conType == "direct")
	{
		con = BodyGraphEnums::enDirect;

	}
	else if (conType == "left")
	{
		con = BodyGraphEnums::enLeftConnection;
	}
	else if (conType == "right")
	{
		con = BodyGraphEnums::enRightConnection;
	}
	else if (conType == "internal")
	{
		con = BodyGraphEnums::enInternal;
	}
	else if (conType == "internalleft")
	{
		con = BodyGraphEnums::enInternalLeft;
	}
	else if (conType == "internalright")
	{
		con = BodyGraphEnums::enInternalRight;
	}
	else if (conType == "external")
	{
		con = BodyGraphEnums::enExternal;
	}
	else if (conType == "externalright")
	{
		con = BodyGraphEnums::enExternalRight;
	}
	else if (conType == "externalleft")
	{
		con = BodyGraphEnums::enExternalLeft;
	}
	else
	{
		con = BodyGraphEnums::enInvalidConnection;
	}

	return con;
}

/*
The bodygraph definition file contains the property of edge connections for a bodypart.
This function converts the string to the enumerated type representing the connection property
*/
BodyGraphEnums::EnConnectionProperty convertStringToConnectionProperty(std::string conType)
{
	std::transform(conType.begin(), conType.end(), conType.begin(), ::tolower);
	BodyGraphEnums::EnConnectionProperty con;

	if (conType == "bone")
	{
		con = BodyGraphEnums::enBone;

	}
	else if (conType == "joint")
	{
		con = BodyGraphEnums::enJoint;
	}
	else if (conType == "connectivetissue")
	{
		con = BodyGraphEnums::enConnectiveTissue;
	}


	return con;

}





/*
Takes as input a bodypart token and an AnatomyGraph and returns the index of that bodypart in the graph
*/
int GetVerticesWithBPToken(std::string bptoken, AnatomyGraph &graph)
{

	AnatomyIndexMap indMap = get(vertex_index, graph);


	for (int i = 0; i < num_vertices(graph); i++)
	{


		if (graph[i].getBodyPartToken() == bptoken)
		{
			return i;

		}
	}

	return -1;
}

/* 
Intended to be called after the Bodygraph has already been constructed. Sure, that's inefficient because we're going to have to search the entire grapha gain after it is already built.
but since we only have to iterate over it one additional time, the tradeoff is worth not having to mess with the existing code...for now. 

*/
void read_wound_properties(const std::string &filename, AnatomyGraph &graph)
{

	WoundTypes::WoundPossibleMap woundsPossible = WoundTypes::process_wound_characteristics_file(filename);
	WoundTypes::WoundPossibleMap::iterator woundIt;
	for (int i = 0; i < num_vertices(graph); i++)
	{

		woundIt = woundsPossible.find(graph[i].getBodyPartToken());

		if (woundIt != woundsPossible.end())
		{
			//graph[i].setWoundProperties(woundIt->second);
		
		}

	

	}




}


