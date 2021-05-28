#include "ArmorLayer.h"
#include "../Material/ForceCalculator.hpp"


std::list<Armor>::iterator ArmorLayers::getLayerIterator()
{
	return armorList.begin();
}

std::list<Armor>::iterator ArmorLayers::getLayerIteratorEnd()
{
	return armorList.end();
}

int ArmorLayers::getNumberOfLayers() const
{
	return armorList.size();
}

const std::list<Armor> ArmorLayers::getArmorList()
{
	return armorList;
}

void ArmorLayers::addArmor(Armor armor)
{
	
	armor.setupResistingForce();
	armorList.push_back(armor);
}


/*
Applies a force to the list of armor and determines the MaterialEffect of each layer of armor. 
Returns a list of AppliedForceEffect, where elemen at position [n] is the kind of MaterialEffect for the armor in the armorList at position [n]

The remaining force of each AppliedForceEffect is applying to the next layer as the starting force
*/
std::list<AppliedForceEffect> ArmorLayers::propagateForce(const ApplyingForce &applyingForce)
{
	std::list<AppliedForceEffect> forceAbsorptionList(0);

	if (armorList.empty())
	{
		//This means that there is no armor to be penentrated and we apply the force directly without taking absoroption into account.
		forceAbsorptionList.push_back(ApplyForceWithNoMaterial(applyingForce));
	
	}
	else
	{

		AppliedForceTypes::AppliedForceType appliedForceType = applyingForce.getAppliedForceType();

		ResistingForce resForce = armorList.front().getResistingForce();
		AppliedForceEffect currentForceEffect = ApplyForceToMaterial(armorList.front().getMaterialDeformationStrength(appliedForceType), armorList.front().getMaterialFractureStrength(appliedForceType), applyingForce, resForce);
		forceAbsorptionList.push_back(currentForceEffect);

		//Start the iterator at the next element. We need to handle the first element differently because the ApplyForceToMaterial takes an ApplyingForce. The remaining ones take a AppliedForceEffect instead. 
		//The remaining force of each AppliedForceEffect is applying to the next layer as the starting force
		auto it = getLayerIterator();
		std::advance(it, 1);
		for (; it != getLayerIteratorEnd(); ++it)
		{

			resForce = it->getResistingForce(); //This gives us the amount of resistance the current layer of armor provides. 
			appliedForceType = currentForceEffect.getAppliedForceType(); //To let us know what kind of force to apply to the next layer
			currentForceEffect = ApplyForceToMaterial(it->getMaterialDeformationStrength(appliedForceType), it->getMaterialFractureStrength(appliedForceType), currentForceEffect, resForce); //And apply it to the layer
			forceAbsorptionList.push_back(currentForceEffect);

		}
	}

	return forceAbsorptionList;	
}

