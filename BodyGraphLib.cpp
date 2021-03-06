// BodyGraphLib.cpp : Defines the entry point for the console application.
//


//todo class
//Attack  Type
//List of vertices the attack type can propagrate to

#include "Bodygraph/BodyGraphReader.hpp"
#include "Bodygraph/BodyGraph.hpp"

#include "Material/Forces.hpp"
#include "DamageTraverser.hpp"
#include "Material/ForceCalculator.hpp"
#include "UnitTests/ForceApplication/ForceUnitTestHeaders.h"
#include "Bodygraph/WoundHandler.hpp"
#include "Material/AppliedForceEffect.hpp"
#include "Material/Forces.hpp"

void PrintVertexInfo(int i, AnatomyGraph &g)
{
	std::cout << "Body Part: " << g[i].getBodyPartName() << std::endl;
}

void printVertexVector(std::vector<int> vec, AnatomyGraph g)
{
	std::cout << "Printing a new vector: " << std::endl;
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		PrintVertexInfo(*it, g);
		std::cout << std::endl;
	}

}



void testWoundProperties()
{

	CreatureBody body = CreatureBody();
	BodyGraphReader reader = BodyGraphReader("gamedata\\bodydata\\BasicHumanoidBody.xml");
	reader.read_wound_properties("gamedata\\bodydata\\BBasicHumanoidBody_WoundCharacteristics.csv");
	AnatomyGraph gr = reader.anatomyGraph;


	body.setBodyGraph(gr);
	body.prepareEdgeMap();
	int a = 3;

	




}

void test_wound_possible_setup()
{

}


int main()
{

	//RunAppliedForceUnitTests();
	//RunDamageTraversalUnitTests();
	testWoundProperties();




    return 0;
}

