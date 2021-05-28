#pragma once

#include "Armor.hpp"
#include "../Material/AppliedForceEffect.hpp"
//#include "../Material/Forces.hpp"

/*
Represents a layer of armor.

std::list<AppliedForceEffect> propagateForce generates the information we need to determine how an ArmorLayer is affected by an ApplyingForce.
propagateForce takes the applyingForce and determines how the material is absorbed by each later. Element[n] in std::list<AppliedForceEffect>
is associated with Element[n] in std::list<Armor> armorList

The force is applied to the Armors ResistingForce. The armor is responsible for its own ResistingForce

*/

class ArmorLayers
{

private:

	std::list<Armor> armorList;

public:
	std::list<Armor>::iterator getLayerIterator();
	std::list<Armor>::iterator getLayerIteratorEnd();
	int getNumberOfLayers() const;
	const std::list<Armor> getArmorList();
	void addArmor(Armor armor);
	std::list<AppliedForceEffect> propagateForce(const ApplyingForce &applyingForce);
};