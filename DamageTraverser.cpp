

#include "DamageTraverser.hpp"
#include <boost/graph/adjacency_list.hpp>

#include <iostream>

//Access the last element of the AppliedForceEffect list. The last element is the remaining force after it has been absorbed by ArmorLayers and other things
//AppliedForceEffect GetFinalForce(AppliedForceBodypartPair &appliedForcePair)
//{
	//return appliedForcePair.second.back();
//}

DamageTraverser::DamageTraverser(std::string _traverserName, ApplyingForce _applyingForce, GraphTraversalTypes::TraversalTypes _traversalType) :
	traverserName(_traverserName),applyingForce(_applyingForce),traversalType(_traversalType)
{

}

/*
This returnns a ForceEffectHandler, which tells us how every bodypart in the CreatureBody that's specified by path (vertex index), will have a force applied to it.
Every bodypart has no or x number of pieces of armored, layered over one another (the next element in the list) The ForceEffect handlers tells us how the layer absorbed the armor, and which bodypart had that layer

A DamageTrasverser is initialized with an ApplyForce. (The amount of force we produce, regardless of anything on another creature/objects body. 
This handles the interaction between the force and that body, based on the targets in path
*/
ForceEffectHandler DamageTraverser::ApplyForceToPath(std::vector<int>& path, CreatureBody &body)
{

	std::list<AppliedForceEffect> appliedForces;
	ForceEffectHandler forceffecthandler;


	for (auto it = path.begin(); it != path.end(); ++it)
	{
		ArmorLayers &armorlayers = body.getBodyPartRef(*it).getArmorLayerRef();
		appliedForces = armorlayers.propagateForce(applyingForce);

		for (auto forceIt = appliedForces.begin(); forceIt != appliedForces.end(); ++forceIt)
		{
			//forceIt->setPossibleWoundTypesCaused(woundtypesPossible);
		}

		//Now we add an element to the map in forceeffectHandler. We don't care if it propagates yet or not, we just need to store teh data.
		BodyPart *bp = body.getBodyPartPointer(*it);
		forceffecthandler.addElementToForceEffectMap(bp, appliedForces);
	}
	return forceffecthandler;
}


/*
For vertex n, Get a random path of length depth. If there aren't enough vertices left to equal depth, then it returns everything up to that point.
*/
std::vector<int> DamageTraverser::getRandomDepthPath(int n, int depth, CreatureBody &body)
{
	std::vector<int> path;
	int currentVertex;
	EdgeMap::iterator currentEntry = body.edgeConnections.find(n);
	path.push_back(n); //A path will always start with the vertex

	if (n >= body.getNumVertices())
	{
		return std::vector<int>();
	}

	for (int i = 0; i < depth; i++)
	{

		//The conditional handles the case for when the depth is greater than the possible path length 
		if (currentEntry != body.edgeConnections.end() && currentEntry->second.size() > 0)
		{
			currentVertex = target(currentEntry->second.at(rand() % currentEntry->second.size()), body.anatomyGraph); //Select a random vertex. We then add the target to the list
			path.push_back(currentVertex);
			currentEntry = body.edgeConnections.find(currentVertex); //Search the edgemap for the vertex we just found. Will select a vertex from this the next iteration
		}
	}
	return path;
}

/*
void DamageTraverser::setCanCauseWounds(WoundTypes::CanCauseWounds _canCauseWounds)
{
	canCauseWounds = _canCauseWounds;
}
*/

void ForceEffectHandler::processAppliedForceEffects()
{
	std::list<AppliedForceEffect>::iterator appliedForceIt; //Having this as a local variable makes it more clear than refering to mapIt->second

	/*
	For every iteration of the outer loop, we get a bodypart and a list of AppliedForceEffects
	The bodypart has an ArmorLayer. armorLayer[i] is associated with AppliedForceEffectList[i]
	That is why we check if the number of armor layer size is equal to the AppliedForceEffectList size
	*/
	for (auto mapIt = forceEffectMap.begin(); mapIt != forceEffectMap.end(); ++mapIt)
	{
		if ((mapIt->first != nullptr) && (mapIt->first->getNumberOfArmorLayers() == 0))
		{
			processForceAppliedToBodypart(*mapIt); //If there's no armor Layer, apply the force directly to the body part. 
		}
		if (mapIt->first == nullptr || mapIt->first->getNumberOfArmorLayers() != mapIt->second.size())
		{
			//TODO think of how to handle error condition
		}
		else
		{
			appliedForceIt = mapIt->second.begin();
			for (auto armorIt = mapIt->first->getLayerIterator(); armorIt != mapIt->first->getLayerIteratorEnd(); ++armorIt)
			{
				if (ForceEffecectTypes::isDeformationEffectType(appliedForceIt->getMaterialEffectType()))
				{
					processDeformationEffect(*armorIt, *appliedForceIt);
				}
				else if (ForceEffecectTypes::isFractureEffectType(appliedForceIt->getMaterialEffectType()))
				{
					processFractureEffect(*armorIt, *appliedForceIt);
					//todo
				}
				//Since the armorLayer and appliedForceList are equal in size, we can move the appliedForceIterator one step ahead for every armorIt iteration
				std::advance(appliedForceIt, 1);
			}

			//Next we apply getForceAppliedToBodypart the mapIt->first, which it the bodyPart we're currently processing with the main loop that iterates over the ForcEffectMap
			processForceAppliedToBodypart(*mapIt);
		}
	}
}

void ForceEffectHandler::processDeformationEffect(Armor &armor, AppliedForceEffect appliedForceEffect)
{

	//Although I check the AppliedForceEffect materialEffect type before calling this function currently, that may not always be the case.
	//So be safe and check it here. 
	if (ForceEffecectTypes::isDeformationEffectType(appliedForceEffect.getMaterialEffectType()))
	{
		if (armor.getDeformationPercentage() >= armor.getMaxDeformationAllowed())
		{
			armor.fractureMaterial(appliedForceEffect.getDamageAmount());
		}
		else
		{
			armor.deformMaterial(appliedForceEffect.getDamageAmount());
		}
	}
}

void ForceEffectHandler::processFractureEffect(Armor &armor, AppliedForceEffect appliedForceEffect)
{
	//Although I check the AppliedForceEffect materialEffect type before calling this function currently, that may not always be the case.
	//So be safe and check it here. 
	if (ForceEffecectTypes::isFractureEffectType(appliedForceEffect.getMaterialEffectType()))
	{
		armor.fractureMaterial(appliedForceEffect.getDamageAmount());
	}
}

/*
This is meant to be called after processAppliedForceEffects has been called. It uses the forceEffectMap

(which is currently this type: std::pair <BodyPart*, std::list<AppliedForceEffect>> AppliedForceBodypartPair;)

Force application to a layer creates a list, where each elemenet tells us the startForce and the endForce, the endForce being the amount of force remaining after absorbing some part of it. (Handled through material)
The last element in the list will be after the force was absorbed by all layers covering the bodypart. The endForce of the last element is what we will apply to the bodypar (which is the first of the pair of the forceEffectMap)

Basically, the endForce of teh last AppliedForceEffect in the list will be the force applied to teh body part 
The last element in the list will 

For now, I am just returning the damageAMount of the last appliedForceEffect. 

Takes a std::pair <BodyPart*, std::list<AppliedForceEffect>> &AppliedForceBodypartPair as input because it's closely tied to a ForceEffectMap, meant to be processed on each element of the forceEffectMap

This method gets called if there are armorLayers on the body

*/
void ForceEffectHandler::processForceAppliedToBodypart(std::pair <BodyPart*, std::list<AppliedForceEffect>> &appliedForceBodypartPair)
{
	float endDamage = appliedForceBodypartPair.second.back().getDamageAmount(); //The last enforce is the amount of remaining force after being absorbed by the ArmorLayers on the bodypart.
	appliedForceBodypartPair.first->addToDamageAmount(endDamage); //For now we just apply the last damageAMount
}

/*
This adds an element to the ForceEffectMap. Each element has a bodypart, and a list of how the layers of armor on the bodypart absorbed the map. This method assumes that the force has already been calculated. 

*/
void ForceEffectHandler::addElementToForceEffectMap(BodyPart *bp, std::list<AppliedForceEffect> forceEffectList)
{
	forceEffectMap.push_back(std::pair(bp, forceEffectList));
}

int ForceEffectHandler::getforceEffectMapSize()
{
	return forceEffectMap.size();
}

ForceEffectMap ForceEffectHandler::getForceEffectMap()
{
	return forceEffectMap;
}

ForceEffectMap & ForceEffectHandler::getForceEffectMapRef()
{
	return forceEffectMap;
}





