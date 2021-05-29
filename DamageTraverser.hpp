#pragma once
#include "Item/ArmorLayer.h"
#include "Material/ForceEnums.hpp"
#include "Material/ForceCalculator.hpp"
#include "Material//Forces.hpp"
#include "Bodygraph/BodyGraph.hpp"
//#include "Bodygraph/BodyPart.hpp"
#include "Material/AppliedForceEffect.hpp"
#include "Item/Armor.hpp"
#include <memory>
#include <string>
#include <vector>





namespace GraphTraversalTypes	
{
	enum TraversalTypes
	{
		depthTraversal,
		breadthTraversal

	};


}




/*
The purpose of this class is to apply effects to the body part and the armor based off their material effects

We get list a list of AppliedForceEffects by applying a Force to a bodypart.
There will be effects both on the Armors that are part of the layer, and the bodyparts themselves
This class handles that

The BodyPart is a pointer because that way we can directly affect the layers of armor and the body parts themselves.

A material can only one AppliedForceEffect applied to it at a time. 

An object of this class is created by a DamageTraverser. Once we have a ForceEffetHandler, we can apply the damage with processAppliedForceEffects.
Also uses the WoundTypes::WoundtypeMapping possibleWoundTypes of the AppliedForceEffect to determine what kind of damage it can cause to teh body

*/

//Can't use a reference for BodyPart becase we're incrementally adding things to it

class BodyPart;

typedef std::pair <BodyPart*, std::list<AppliedForceEffect>> AppliedForceBodypartPair;
typedef std::vector <AppliedForceBodypartPair> ForceEffectMap;

//AppliedForceEffect GetFinalForce(AppliedForceBodypartPair &appliedForcePair);



class ForceEffectHandler
{



private:

	ForceEffectMap forceEffectMap;
	void processDeformationEffect(Armor &armor,AppliedForceEffect appliedForceEffect);
	void processFractureEffect(Armor &armor, AppliedForceEffect appliedForceEffect);
	void processForceAppliedToBodypart(std::pair <BodyPart*, std::list<AppliedForceEffect>> &appliedForceBodypartPair);






public:

	//The body part continues to exist. The AppleidForceEffect stops to exist. That's why we're only passing the body part as a pointer
	void addElementToForceEffectMap(BodyPart *bp, std::list <AppliedForceEffect> forceEffectList);

	int getforceEffectMapSize();
	ForceEffectMap getForceEffectMap();
	ForceEffectMap& getForceEffectMapRef();
	void processAppliedForceEffects();


};




/*

_______________________________________

TOdo determine if this part of the comment is still seful

(BluntAttack, CompressionForce, DepthTraversal)
(PierceAttack, ShearForce, depthtraveral)
(SlashAttack, ShearForce, breadthTraversal)

AtttackFracture = (AttackType, FractureEffectEnum, TraversalType)
AttackDeformation = (AttackType, DeformationEffectEnum, TraversalType)

(AttackType,ApplyingForceType,TraversalType,TargetSelector)


_______________________________________

ApplyingForceType gives us the amount of force applied
TraversalType tells us how we traverse the vertex, which uses the results form AppliedForceType to determine whether there is deformation or fracture

(string AttackType, ApplyingForceType *a, TraversalType,TargetSelector)

(TargetSelector BodyGraph TargetingParameters)

DamageTraverser uses an ApplyingForce to determine what kind of material effect occurs (deformation or fracture) on a ResistingForce. 
It handles the interaction between the ApplingForce used to construct the class, and whatever ResistingForce we want to use, passed to the ApplyForceToMaterial function.
I don't want to tie specific kinds of weapons or attacks to a specific material effect with anything more than a string name

Previous I an enumerated types like enBlunt,enPierce, etc. The downside with that is that we would be limiting ourselves by those enums. The AppliedForceEffect still uses
enums for now, because I don't want to generalize forces beyond the shear, tension, etc.

TraversalTypes traversalType also currently uses enums. It determines 

The traverserName would just be the kind of attack [I.E, blunt attack, slashing attack]

ApplyingForce contains the data fields and does the calculation for a DamageTraverser

Creates a ForceEffectHandler

Any sort of attack will have a damageTraveser. 

A thrusting attack for example will have piercing, pucture, etc as woundTypesPossible
A slashing attack would have dismember, cut etc
A blunt attack would have bruise, fracture etc.

So a sword for example would have a DamageTraverser for a thrust, and another for a slash
The WoundTypes::WoundtypeMapping determines what kind of damage a wound of a severity causes. So a light damage thrust would cause bleeding, and a heavy damage thrust a rupture for example
A light damage slash would also cause bleeding, and a serious one dismemberment for example

*/
class DamageTraverser
{

private:

	std::string traverserName;
	GraphTraversalTypes::TraversalTypes traversalType;
	ApplyingForce applyingForce;
	int numTargets; //How many vertices the DamageTraverser targets
	int maxTraversalDistance; //How far the DamageTraverser can travel on a path when a fracture material effect occurs. 
	//WoundTypes::EnWoundProperties canCauseWounds; //Thge 


public:

	DamageTraverser(std::string _traverserName,ApplyingForce _applyingForce, GraphTraversalTypes::TraversalTypes _traversalType);
	ForceEffectHandler ApplyForceToPath(std::vector<int> &path, CreatureBody &body);
	std::vector<int> getRandomDepthPath(int n, int depth, CreatureBody &body);

	//void setCanCauseWounds(WoundTypes::CanCauseWounds _canCauseWounds);
	

	



};





